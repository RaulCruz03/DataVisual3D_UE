// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ReadWriteJsonFile.h"
#include "ReadWriteFile.h"

#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

FS_Agente U_ReadWriteJsonFile::ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return FS_Agente();
	}
	FS_Agente RetFS_Agente;

	if (!FJsonObjectConverter::JsonObjectToUStruct<FS_Agente>(JsonObject.ToSharedRef(), &RetFS_Agente))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Failed to deserialize json to struct from file: %s"), *JsonFilePath);
		return FS_Agente();
	}
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Successfully read json to struct from file: %s"), *JsonFilePath);
	return RetFS_Agente;
}
void U_ReadWriteJsonFile::WriteStructToJsonFile(FString JsonFilePath, FS_Agente Struct, bool& bOutSuccess, FString& OutInfoMessage)
{
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
	if (JsonObject == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Failed to serialize struct to json for file: %s"), *JsonFilePath);
		return;
	}
	WriteJson(JsonFilePath, JsonObject, bOutSuccess, OutInfoMessage);
}

TSharedPtr<FJsonObject> U_ReadWriteJsonFile::ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage) {

	FString JsonString = U_ReadWriteFile::ReadStringFromFile(JsonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> RetJsonObject;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Failed to deserialize json from file: %s"), *JsonFilePath);
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Successfully read json from file: %s"), *JsonFilePath);
	return RetJsonObject;

}

void U_ReadWriteJsonFile::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString JsonString;

	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Failed to serialize json to string for file: %s"), *JsonFilePath);
		return;
	}

	U_ReadWriteFile::WriteStringToFile(JsonFilePath, JsonString, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return;
	}
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Successfully wrote json to file: %s"), *JsonFilePath);
}