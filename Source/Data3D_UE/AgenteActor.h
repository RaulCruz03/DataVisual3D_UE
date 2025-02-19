#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Blueprint/UserWidget.h"
#include "AgenteInfoWidget.h" //  Deve vir ANTES do .generated.h
#include "AgenteActor.generated.h" // SEMPRE o último include


UCLASS()
class DATA3D_UE_API AAgenteActor : public AActor

	
{
	GENERATED_BODY()

public:
	AAgenteActor();

	// Função para inicializar os dados do agente
	UFUNCTION(BlueprintCallable, Category = "Agente")
	void InitializeAgentData(const FString& InId, const FString& InNome, float InSaldo, float InMedia);

protected:
	virtual void BeginPlay() override;

	// Componente de malha para representar a esfera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* SphereComponent;

	// Componente de texto para exibir o nome
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent* TextComponent;

	// Dados do agente
	FString idAgente;
	FString Nome;
	float Saldo;
	float Media;

	// Função auxiliar para configurar a cor do agente
	void SetAgentColor(const FLinearColor& Color);

	// Classe do widget para exibir as informações do agente
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> AgentInfoWidgetClass;

    // Instância do widget de informações do agente
    UPROPERTY()
	UAgenteInfoWidget* AgentInfoWidget;

	// Funções para exibir e esconder o widget
	UFUNCTION()
	void ShowAgentInfo();

	UFUNCTION()
	void HideAgentInfo();

	// Função para tratar o clique na esfera
	UFUNCTION()
	void OnSphereClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);
};