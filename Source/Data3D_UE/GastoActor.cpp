#include "GastoActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/UserWidget.h"
#include "GastoInfoWidget.h"  // Caso você precise usar funções específicas do widget
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputSettings.h"
#include "Components/InputComponent.h"


AGastoActor::AGastoActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // Cria e configura o componente de malha (esfera)
    SphereComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;

    // Procura a malha padrão de esfera da Unreal Engine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMesh.Succeeded())
    {
        SphereComponent->SetStaticMesh(SphereMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/Materials/M_Gasto.M_Gasto"));
    if (MaterialFinder.Succeeded())
    {
        SphereComponent->SetMaterial(0, MaterialFinder.Object);
    }

    
    // Configura a colisão para que a esfera receba cliques
    SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
    SphereComponent->OnClicked.AddDynamic(this, &AGastoActor::OnSphereClicked);

    // Carrega a classe do widget WBP_GastoInfo (certifique-se de que o widget está em Content/BlueprintWidgets/)
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/BlueprintWidgets/WBP_GastoInfo"));
    if (WidgetClassFinder.Succeeded())
    {
        GastoInfoWidgetClass = WidgetClassFinder.Class;
    }
}

void AGastoActor::BeginPlay()
{
    Super::BeginPlay();

    // Cria o widget se a classe estiver definida
    if (GastoInfoWidgetClass)
    {
        GastoInfoWidget = CreateWidget<UGastoInfoWidget>(GetWorld(), GastoInfoWidgetClass);
    }

    // Configura o input para a tecla "O"
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        if (!PC->InputComponent)
        {
            PC->InputComponent = NewObject<UInputComponent>(PC);
            PC->InputComponent->RegisterComponent();
        }
        // Certifique-se de mapear a ação "HideGastoInfo" para a tecla "O" em Project Settings > Input
        PC->InputComponent->BindAction("HideGastoInfo", IE_Pressed, this, &AGastoActor::HideGastoInfo);
    }
}

void AGastoActor::InitializeGastoData(const FString& InAgentePagador,
    const FString& InAgenteRecebedor,
    double InValor,
    double InMedia,
    int32 InCodTransacao,
    const FString& InMes)
{
    AgentePagador = InAgentePagador;
    AgenteRecebedor = InAgenteRecebedor;
    Valor = InValor;
    Media = InMedia;
    CodTransacao = InCodTransacao;
    Mes = InMes;

    // Ajusta a escala da esfera com base no Valor (aplica um fator de conversão)
    float ScaleFactor = static_cast<float>(Valor) * 0.01f;
    SphereComponent->SetWorldScale3D(FVector(ScaleFactor));


    // Cria uma instância dinâmica do material para alterar a cor com base na comparação Valor vs Media
    UMaterialInstanceDynamic* DynamicMaterial = SphereComponent->CreateAndSetMaterialInstanceDynamic(0);
    if (DynamicMaterial)
    {
        if (Valor > Media)
        {
            FLinearColor DarkRed(0.1f, 0.0f, 0.0f, 1.0f); // 50% de intensidade no vermelho
            DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), DarkRed);
        }
        else
        {
            FLinearColor DarkGreen(0.0f, 0.1f, 0.0f, 1.0f); // 50% de intensidade no verde
            DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), DarkGreen);
        }
    }
}

void AGastoActor::OnSphereClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    UE_LOG(LogTemp, Warning, TEXT("Gasto esfera clicada: Agente Pagador = %s, Agente Recebedor = %s"), *AgentePagador, *AgenteRecebedor);
    ShowGastoInfo();
}

void AGastoActor::ShowGastoInfo()
{
    if (GastoInfoWidgetClass)
    {
        // Se o widget ainda não foi criado, crie-o
        if (!GastoInfoWidget)
        {
            GastoInfoWidget = CreateWidget<UGastoInfoWidget>(GetWorld(), GastoInfoWidgetClass);
        }

        // Adiciona o widget à viewport se ainda não estiver
        if (GastoInfoWidget && !GastoInfoWidget->IsInViewport())
        {
            GastoInfoWidget->AddToViewport();
            UE_LOG(LogTemp, Display, TEXT("Widget de gasto adicionado ao viewport!"));

            // Atualiza os campos do widget (cast para UGastoInfoWidget para acessar os EditableText)
            UGastoInfoWidget* InfoWidget = Cast<UGastoInfoWidget>(GastoInfoWidget);
            if (InfoWidget)
            {
                if (InfoWidget->tb_codTransacao)
                    InfoWidget->tb_codTransacao->SetText(FText::FromString(FString::FromInt(CodTransacao)));
                if (InfoWidget->tb_Agente_Pagador)
                    InfoWidget->tb_Agente_Pagador->SetText(FText::FromString(AgentePagador));
                if (InfoWidget->tb_Agente_Recebedor)
                    InfoWidget->tb_Agente_Recebedor->SetText(FText::FromString(AgenteRecebedor));
                if (InfoWidget->tb_Valor)
                    InfoWidget->tb_Valor->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Valor)));
                if (InfoWidget->tb_Media)
                    InfoWidget->tb_Media->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Media)));
                if (InfoWidget->tb_Mes)
                    InfoWidget->tb_Mes->SetText(FText::FromString(Mes));
            }

            // Configura o modo de input para GameAndUI (para garantir que a ação "HideGastoInfo" seja recebida)
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(GastoInfoWidget->TakeWidget());
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
                UE_LOG(LogTemp, Display, TEXT("Input configurado para modo GameAndUI"));
            }
        }
    }
}

void AGastoActor::HideGastoInfo()
{
    if (GastoInfoWidget && GastoInfoWidget->IsInViewport())
    {
        GastoInfoWidget->RemoveFromParent();
        UE_LOG(LogTemp, Warning, TEXT("Widget de gasto fechado!"));

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
            UE_LOG(LogTemp, Display, TEXT("Input restaurado para modo de jogo."));
        }

        if (SphereComponent)
        {
            SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            UE_LOG(LogTemp, Display, TEXT("Colisão da esfera reativada."));
        }
    }
}
