// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/BaseBuilding.h"
#include "AI/Citizen.h"
#include "BaseHouse.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PIXELCITIESVR_API ABaseHouse : public ABaseBuilding
{
	GENERATED_BODY()

	// Sets default values for this actor's properties
	ABaseHouse(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	// Called when actor is destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:

	// Array for all citizens living in the house
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "House")
		TArray<ACitizen*> Residents;

	// Array for all citizens moving into the house
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "House")
		TArray<ACitizen*> TakenSlots;

	// Variable for max number of citizens that can live in house, and are spawned at build
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "House")
		int32 MaxResidents;
	
	/** Used to add a resident to the house */
	UFUNCTION(BlueprintCallable, Category = "House")
	void AddResident(ACitizen* ResidentToAdd);

	/** Used to remove a resident from the house - also removes from the TakenSlots list */
	UFUNCTION(BlueprintCallable, Category = "House")
	void RemoveResident(ACitizen* ResidentToRemove);

	/** Used to add a potential resident to the house, marking a slot as taken to prevent more people than possible trying to make the house their own */
	UFUNCTION(BlueprintCallable, Category = "House")
	void AddPotentialResident(ACitizen* PotentialResident);
};
