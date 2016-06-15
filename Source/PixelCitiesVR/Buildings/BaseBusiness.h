// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/BaseBuilding.h"
#include "AI/Citizen.h"
#include "BaseBusiness.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCITIESVR_API ABaseBusiness : public ABaseBuilding
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ABaseBusiness(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	// Called when actor is destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// max number of workers for array
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Business")
	int32 MaxWorkers;

	// array of workers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Business")
	TArray<ACitizen*> Workers;

	/** Adds worker to the Workers array */
	UFUNCTION()
	void AddWorker(ACitizen* Worker);

	/** Removes the worker from the workers array */
	UFUNCTION()
	void RemoveWorker(ACitizen* Worker);
};