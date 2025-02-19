#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Blueprint/UserWidget.h"
#include "AgenteInfoWidget.h" //  Deve vir ANTES do .generated.h
#include "AgenteActor.generated.h" // SEMPRE o �ltimo include


UCLASS()
class DATA3D_UE_API AAgenteActor : public AActor

	
{
	GENERATED_BODY()

public:
	AAgenteActor();

	// Fun��o para inicializar os dados do agente
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

	// Fun��o auxiliar para configurar a cor do agente
	void SetAgentColor(const FLinearColor& Color);

	// Classe do widget para exibir as informa��es do agente
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> AgentInfoWidgetClass;

    // Inst�ncia do widget de informa��es do agente
    UPROPERTY()
	UAgenteInfoWidget* AgentInfoWidget;

	// Fun��es para exibir e esconder o widget
	UFUNCTION()
	void ShowAgentInfo();

	UFUNCTION()
	void HideAgentInfo();

	// Fun��o para tratar o clique na esfera
	UFUNCTION()
	void OnSphereClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);
};