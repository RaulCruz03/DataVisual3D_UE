#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Blueprint/UserWidget.h"
#include "GastoInfoWidget.h"
#include "GastoActor.generated.h"

UCLASS()
class DATA3D_UE_API AGastoActor : public AActor
{
    GENERATED_BODY()

public:
    AGastoActor();

    UFUNCTION(BlueprintCallable, Category = "Gasto")
    void InitializeGastoData(
        const FString& InAgentePagador,
        const FString& InAgenteRecebedor,
        double InValor,
        double InMedia,
        int32 InCodTransacao,
        const FString& InMes
    );

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* TextComponent;

    FString AgentePagador;
    FString AgenteRecebedor;
    double Valor;
    double Media;
    int32 CodTransacao;
    FString Mes;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> GastoInfoWidgetClass;

    UPROPERTY()
    UGastoInfoWidget* GastoInfoWidget;

    UFUNCTION()
    void OnSphereClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

    UFUNCTION()
    void ShowGastoInfo();

    UFUNCTION()
    void HideGastoInfo();
};