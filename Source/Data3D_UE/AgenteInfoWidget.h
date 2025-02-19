// AgenteInfoWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h" //  Adicione esta linha
#include "AgenteInfoWidget.generated.h" // Mantenha por último

UCLASS()
class DATA3D_UE_API UAgenteInfoWidget : public UUserWidget  
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_IdAgente; // Tipo para Text Box

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Nome;

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Saldo;

    UPROPERTY(meta = (BindWidget))
    UEditableText* tb_Media;
};
