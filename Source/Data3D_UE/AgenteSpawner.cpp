#include "AgenteSpawner.h"
#include "AgenteActor.h"  // Certifique-se de que o nome da classe de agente esteja correto
#include "Engine/World.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"  // Para usar as classes de JSON

#include <iostream>
#include <random>

AAgenteSpawner::AAgenteSpawner()


{

	std::random_device rd;  // Fonte de entropia
	std::mt19937 gen(rd()); // Mersenne Twister (motor de n�meros aleat�rios)

	std::uniform_real_distribution<float> dist(0.0f, 1000.0f); // Distribui��o uniforme entre 1 e 300

	PrimaryActorTick.bCanEverTick = false;

	// Exemplo de posi��es para spawn dos agentes.
	// Voc� pode configur�-las via editor.
	for (int32 i = 0; i < 5; i++)
	{
		AgentPositions.Add(FVector(dist(gen), dist(gen), dist(gen)));

	}
}

void AAgenteSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnAgentsFromJSON();
}

void AAgenteSpawner::SpawnAgentsFromJSON()
{
	// Define o caminho para o arquivo JSON (ajuste conforme a estrutura do seu projeto)
	FString FilePath = FPaths::ProjectContentDir() + "File/agentes.json";
	FString JsonString;

	// Tenta carregar o conte�do do arquivo JSON para a string JsonString
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("N�o foi poss�vel ler o arquivo JSON: %s"), *FilePath);
		return;
	}

	// Cria um leitor de JSON a partir da string lida
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Erro ao desserializar o JSON."));
		return;
	}

	// Verifica se o JSON possui o array "Agentes"
	const TArray<TSharedPtr<FJsonValue>>* AgentesJson;
	if (!JsonObject->TryGetArrayField("Agentes", AgentesJson))
	{
		UE_LOG(LogTemp, Error, TEXT("Campo 'Agentes' n�o encontrado no JSON."));
		return;
	}

	// Itera sobre cada objeto de agente no array
	for (int32 i = 0; i < AgentesJson->Num(); i++)
	{
		TSharedPtr<FJsonObject> AgentObject = (*AgentesJson)[i]->AsObject();
		if (!AgentObject.IsValid())
		{
			continue;
		}

		// Extrai os dados do agente
		FString Id = AgentObject->GetStringField("idAgente");
		FString Nome = AgentObject->GetStringField("Nome");
		float Saldo = AgentObject->GetNumberField("Saldo");
		float Media = AgentObject->GetNumberField("Media");

		// Define a posi��o de spawn para esse agente
		FVector SpawnPosition = FVector::ZeroVector;
		if (AgentPositions.IsValidIndex(i))
		{
			SpawnPosition = AgentPositions[i];
		}
		else
		{
			// Se n�o houver posi��o definida, calcula uma posi��o padr�o
			SpawnPosition = FVector(i * 200.f, 0.f, 0.f);
		}

		// Par�metros de spawn
		FActorSpawnParameters SpawnParams;

		// Realiza o spawn do AAgenteActor
		AAgenteActor* NovoAgente = GetWorld()->SpawnActor<AAgenteActor>(AAgenteActor::StaticClass(), SpawnPosition, FRotator::ZeroRotator, SpawnParams);
		if (NovoAgente)
		{
			// Inicializa os dados do agente no ator spawnado
			NovoAgente->InitializeAgentData(Id, Nome, Saldo, Media);
		}
	}
}
