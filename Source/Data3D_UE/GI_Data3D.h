#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_Data3D.generated.h"



UCLASS(Blueprintable, BlueprintType)
class DATA3D_UE_API UGI_Data3D : public UGameInstance
{
    GENERATED_BODY()

public:
    // Caminho do arquivo JSON
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config")
    FString FilePath;

    // Mês selecionado
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config")
    FString SelectedMonth;
};
