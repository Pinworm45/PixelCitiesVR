// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCitiesVR.h"
#include "BaseBusiness.h"


ABaseBusiness::ABaseBusiness(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxWorkers = 4;
}

void ABaseBusiness::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ABaseBusiness::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (int32 i = 0; i < Workers.Num(); i++)
	{
		Workers[i]->SetWorkplace(nullptr);
	}
}

void ABaseBusiness::AddWorker(ACitizen* Worker)
{
	if (Workers.Num() < MaxWorkers)
	{
		Workers.AddUnique(Worker);
		UE_LOG(LogBusiness, Log, TEXT("Added worker %s to business %s"), *Worker->Name, *GetName())
	}
	else
	{
		UE_LOG(LogBusiness, Warning, TEXT("Tried to add worker %s to business %s, but there was not enough space"), *Worker->Name, *GetName())
	}
}

void ABaseBusiness::RemoveWorker(ACitizen* Worker)
{
	Workers.Remove(Worker);
	UE_LOG(LogBusiness, Log, TEXT("Removed worker %s from business %s"), *Worker->Name, *GetName())
}

