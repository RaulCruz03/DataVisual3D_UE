// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteJsonFile.generated.h"


class FJsonObject;

USTRUCT(BlueprintType, Category = "Alex Quevillon2 - Read Write Json File")
struct FS_Agente
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Alex Quevillon2 - Read Write Json File")
    FString idAgente = "038,945.200-94";
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Alex Quevillon2 - Read Write Json File")
    FString Nome = "Raul da Cruz";
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Alex Quevillon2 - Read Write Json File")
    float Saldo= 128.456f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Alex Quevillon2 - Read Write Json File")
    float Media = 123;


};

UCLASS()
class U_ReadWriteJsonFile : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Alex Quevillon2 - Read Write Json File")
    static FS_Agente ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);

    UFUNCTION(BlueprintCallable, Category = "Alex Quevillon|2 - Read Write Json")
    static void WriteStructToJsonFile(FString JsonFilePath, FS_Agente Struct, bool& bOutSuccess, FString& OutInfoMessage);

public:

    /**
     * Open a json file, read its content and convert it to a JsonObject
     *
     * @param JsonFilePath    The path of your json file: "C:/Temp/MyJson.json"
     * @param bOutSuccess     If the action was a success or not
     * @param OutInfoMessage  More information about the action's result
     *
     * @return The JsonObject content of your json file
     */
    static TSharedPtr<FJsonObject> ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);

    /**
     * Convert your JsonObject to a string, create a json file and dump the string in it
     *
     * @param JsonFilePath    The path of your json file: "C:/Temp/MyJson.json"
     * @param JsonObject      The JsonObject to want to write in the file
     * @param bOutSuccess     If the action was a success or not
     * @param OutInfoMessage  More information about the action's result
     */
    static void WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage);
};
