#include "AgenteActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/TextRenderComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputSettings.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "AgenteInfoWidget.h"

AAgenteActor::AAgenteActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Inicializa o componente da esfera
	SphereComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	// Carrega a malha padrão da esfera
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMesh.Succeeded())
	{
		SphereComponent->SetStaticMesh(SphereMesh.Object);
	}

	// Carrega o material base
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/Materials/M_Agente.M_Agente"));
	if (MaterialFinder.Succeeded())
	{
		SphereComponent->SetMaterial(0, MaterialFinder.Object);
	}

	// Configura a colisão para que a esfera receba cliques
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
	SphereComponent->OnClicked.AddDynamic(this, &AAgenteActor::OnSphereClicked);

	// Inicializa o componente de texto para exibir o nome
	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	TextComponent->SetupAttachment(SphereComponent);
	TextComponent->SetHorizontalAlignment(EHTA_Center);
	TextComponent->SetWorldSize(50.0f);
	TextComponent->SetTextRenderColor(FColor::White);
	TextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	// Carrega a classe do widget WBP_AgenteInfo a partir do caminho correto
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/BlueprintWidgets/WBP_AgenteInfo"));
	if (WidgetClassFinder.Succeeded())
	{
		AgentInfoWidgetClass = WidgetClassFinder.Class;
	}
}

void AAgenteActor::BeginPlay()
{
	Super::BeginPlay();

	// Cria o widget
	if (AgentInfoWidgetClass)
	{
		AgentInfoWidget = CreateWidget<UAgenteInfoWidget>(GetWorld(), AgentInfoWidgetClass);
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

		// Vincula a ação "HideAgentInfo" à tecla "O"
		PC->InputComponent->BindAction("HideAgentInfo", IE_Pressed, this, &AAgenteActor::HideAgentInfo);
	}
}

void AAgenteActor::InitializeAgentData(const FString& InId, const FString& InNome, float InSaldo, float InMedia)
{
	idAgente = InId;
	Nome = InNome;
	Saldo = InSaldo;
	Media = InMedia;

	TextComponent->SetText(FText::FromString(Nome));

	// Exemplo de mudança de cor:
	// Se idAgente tiver 11 dígitos (CPF), azul; se tiver 14 dígitos (CNPJ), amarelo; senão, branco.
	if (idAgente.Len() == 11)
	{
		SetAgentColor(FLinearColor::Blue);
	}
	else if (idAgente.Len() == 14)
	{
		SetAgentColor(FLinearColor::Yellow);
	}
	else
	{
		SetAgentColor(FLinearColor::White);
	}
}

void AAgenteActor::SetAgentColor(const FLinearColor& Color)
{
	if (SphereComponent->GetMaterial(0))
	{
		UMaterialInstanceDynamic* DynMat = SphereComponent->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMat)
		{
			DynMat->SetVectorParameterValue(FName("BaseColor"), Color);
		}
	}
}

void AAgenteActor::OnSphereClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Esfera clicada: %s"), *idAgente);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShowAgentInfo();
}

void AAgenteActor::ShowAgentInfo()
{
	if (!AgentInfoWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Widget não foi criado!"));
		return;
	}

	if (AgentInfoWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget já está visível!"));
		return;
	}

	AgentInfoWidget->AddToViewport();
	UE_LOG(LogTemp, Display, TEXT("Widget adicionado ao viewport!"));

	if (AgentInfoWidget)
	{
		// Defina os textos como Read-Only se não precisar de edição
		AgentInfoWidget->tb_IdAgente->SetIsReadOnly(true);
		AgentInfoWidget->tb_IdAgente->SetText(FText::FromString(idAgente));

		AgentInfoWidget->tb_Nome->SetIsReadOnly(true);
		AgentInfoWidget->tb_Nome->SetText(FText::FromString(Nome));

		// Formate números para texto (exemplo: Saldo = 1500.50)
		AgentInfoWidget->tb_Saldo->SetText(FText::AsCurrency(Saldo));
		AgentInfoWidget->tb_Media->SetText(FText::AsNumber(Media));
	}
}

void AAgenteActor::HideAgentInfo()
{
	if (AgentInfoWidget && AgentInfoWidget->IsInViewport())
	{
		// Remove o widget do viewport
		AgentInfoWidget->RemoveFromParent();

		// Restaura o input para o modo de jogo
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true; // Esconde o cursor
		}

		// Reativa a colisão da esfera para permitir novos cliques
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}