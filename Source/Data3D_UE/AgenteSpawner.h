#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AgenteSpawner.generated.h"

UCLASS()
class DATA3D_UE_API AAgenteSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Construtor padr�o
	AAgenteSpawner();

protected:
	// Chamado quando o jogo inicia ou o ator � spawnado
	virtual void BeginPlay() override;

	// Array com posi��es pr�-definidas para spawn dos agentes
	// Esse array pode ser editado via editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agente Spawner")
	TArray<FVector> AgentPositions;

	// Fun��o para ler o JSON e spawnar os agentes
	void SpawnAgentsFromJSON();
};
