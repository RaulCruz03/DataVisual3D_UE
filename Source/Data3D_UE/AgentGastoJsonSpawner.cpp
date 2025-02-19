#include "AgentGastoJsonSpawner.h"
#include "AgenteActor.h"
#include "GastoActor.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"
#include "JsonUtilities.h"
#include <random>
#include "DrawDebugHelpers.h"
#include "GI_Data3D.h"

AAgentGastoJsonSpawner::AAgentGastoJsonSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // Define um caminho padrão para o JSON (relativo ao diretório Content)
    JsonFilePath = "File/AgentsGastos.json";
}

void AAgentGastoJsonSpawner::BeginPlay()
{
    Super::BeginPlay();
    LoadAndSpawnActorsFromJson();
}

void AAgentGastoJsonSpawner::LoadAndSpawnActorsFromJson()
{
    // Obter o GameInstance customizado e ler os valores
    UGI_Data3D* GIRef = Cast<UGI_Data3D>(GetGameInstance());
    FString FilterMonth;
    if (GIRef)
    {
        FilterMonth = GIRef->SelectedMonth;
        JsonFilePath = GIRef->FilePath; // Opcional, caso você use o caminho do GameInstance
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance GI_Data3D nao encontrado. Usando o caminho padrao."));
    }

    // Constrói o caminho completo para o arquivo JSON
    FString FullPath = FPaths::ProjectContentDir() + JsonFilePath;
    FString JsonString;

    // Tenta carregar o arquivo para uma string
    if (!FFileHelper::LoadFileToString(JsonString, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Falha ao carregar o arquivo JSON: %s"), *FullPath);
        return;
    }

    // Desserializa o JSON
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Falha ao desserializar o JSON."));
        return;
    }

    // Cria um mapa para associar o ID do agente à posição em que ele foi spawnado
    TMap<FString, FVector> AgentPositionsMap;

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World não encontrado."));
        return;
    }

    FActorSpawnParameters SpawnParams;

    // Configura o gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-3500.0f, 3500.0f);

    // --- Spawn dos Agentes ---
    const TArray<TSharedPtr<FJsonValue>>* AgentesArray;
    if (JsonObject->TryGetArrayField("Agentes", AgentesArray))
    {
        for (int32 i = 0; i < AgentesArray->Num(); i++)
        {
            TSharedPtr<FJsonObject> AgentObj = (*AgentesArray)[i]->AsObject();
            if (!AgentObj.IsValid())
            {
                continue;
            }

            // Lê os dados do agente do JSON
            FString idAgente = AgentObj->GetStringField("idAgente");
            FString Nome = AgentObj->GetStringField("Nome");
            float Saldo = AgentObj->GetNumberField("Saldo");
            float Media = AgentObj->GetNumberField("Media");

            // Gera uma posição aleatória para o agente
            FVector Position(dist(gen), dist(gen), 100.0f);

            // Spawn do AgenteActor
            AAgenteActor* NewAgent = World->SpawnActor<AAgenteActor>(AAgenteActor::StaticClass(), Position, FRotator::ZeroRotator, SpawnParams);
            if (NewAgent)
            {
                NewAgent->InitializeAgentData(idAgente, Nome, Saldo, Media);
                AgentPositionsMap.Add(idAgente, Position);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Campo 'Agentes' não encontrado no JSON."));
    }

    // --- Spawn dos Gastos ---
    const TArray<TSharedPtr<FJsonValue>>* GastosArray;
    if (JsonObject->TryGetArrayField("Gastos", GastosArray))
    {
        for (int32 i = 0; i < GastosArray->Num(); i++)
        {
            TSharedPtr<FJsonObject> GastoObj = (*GastosArray)[i]->AsObject();
            if (!GastoObj.IsValid())
            {
                continue;
            }

            // Lê o mês do gasto e filtra
            FString MesGasto = GastoObj->GetStringField("Mes");
            if (!FilterMonth.IsEmpty() && MesGasto != FilterMonth)
            {
                continue; // Ignora gastos que não correspondem ao mês selecionado
            }

            // Lê os dados do gasto do JSON
            FString AgentePagador = GastoObj->GetStringField("Agente_Pagador");
            FString AgenteRecebedor = GastoObj->GetStringField("Agente_Recebedor");
            double Valor = GastoObj->GetNumberField("Valor");
            double MediaGasto = GastoObj->GetNumberField("Media");
            int32 CodTransacao = GastoObj->GetIntegerField("codTransacao");

            // Obtém as posições dos agentes pagador e recebedor a partir do mapa
            FVector PosPagador = AgentPositionsMap.Contains(AgentePagador) ? AgentPositionsMap[AgentePagador] : FVector::ZeroVector;
            FVector PosRecebedor = AgentPositionsMap.Contains(AgenteRecebedor) ? AgentPositionsMap[AgenteRecebedor] : FVector::ZeroVector;

            // Calcula o ponto médio entre as duas posições
            FVector MidPoint = (PosPagador + PosRecebedor) * 0.5f;

            // Spawn do GastoActor
            AGastoActor* NewGasto = World->SpawnActor<AGastoActor>(AGastoActor::StaticClass(), MidPoint, FRotator::ZeroRotator, SpawnParams);
            if (NewGasto)
            {
                NewGasto->InitializeGastoData(AgentePagador, AgenteRecebedor, Valor, MediaGasto, CodTransacao, MesGasto);

                // Desenha linhas entre as esferas dos agentes e o ponto médio do gasto
                DrawDebugLine(World, PosPagador, MidPoint, FColor::Red, true, -1.0f, 0, 5.0f);
                DrawDebugLine(World, PosRecebedor, MidPoint, FColor::Green, true, -1.0f, 0, 5.0f);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Campo 'Gastos' não encontrado no JSON."));
    }
}
