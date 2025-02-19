#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "GastoInfoWidget.generated.h"

UCLASS()
class DATA3D_UE_API UGastoInfoWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_codTransacao;

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Agente_Pagador;

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Agente_Recebedor;

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Valor;

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Media;

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Mes;
};