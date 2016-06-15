// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCitiesVR.h"
#include "BaseHouse.h"

ABaseHouse::ABaseHouse(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxResidents = 4;
}

void ABaseHouse::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ABaseHouse::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (int32 i = 0; i < Residents.Num(); i++)
	{
		Residents[i]->SetHouse(nullptr);
	}
}

void ABaseHouse::AddResident(ACitizen* ResidentToAdd)
{
	if (Residents.Num() < MaxResidents)
	{
		Residents.AddUnique(ResidentToAdd);
		UE_LOG(LogHouse, Log, TEXT("Added resident %s to house %s"), *ResidentToAdd->Name, *GetName())
	}
	else
	{
		UE_LOG(LogHouse, Warning, TEXT("Tried to add resident %s to house %s, but there was not enough space"), *ResidentToAdd->Name, *GetName())
	}
}

void ABaseHouse::RemoveResident(ACitizen* ResidentToRemove)
{
	Residents.Remove(ResidentToRemove);
	TakenSlots.Remove(ResidentToRemove);
	UE_LOG(LogHouse, Log, TEXT("Removed resident %s to house %s"), *ResidentToRemove->Name, *GetName())
}

void ABaseHouse::AddPotentialResident(ACitizen* PotentialResident)
{
	if (TakenSlots.Num() < MaxResidents)
	{
		TakenSlots.AddUnique(PotentialResident);
	}
}