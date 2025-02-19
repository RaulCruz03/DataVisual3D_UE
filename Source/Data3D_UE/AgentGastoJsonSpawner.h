#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AgentGastoJsonSpawner.generated.h"

UCLASS()
class DATA3D_UE_API AAgentGastoJsonSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Construtor padrão
	AAgentGastoJsonSpawner();

protected:
	// Chamado quando o jogo inicia
	virtual void BeginPlay() override;

	// Caminho relativo para o arquivo JSON (pode ser configurado no editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JSON")
	FString JsonFilePath;

private:
	// Função que carrega e spawn os atores a partir do JSON
	void LoadAndSpawnActorsFromJson();
};
