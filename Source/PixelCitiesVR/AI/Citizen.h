// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Citizen.generated.h"

UCLASS()
class PIXELCITIESVR_API ACitizen : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACitizen();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Called when actor is destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// CITIZEN STATS //
	UPROPERTY(BlueprintReadWrite, Category = "Citizen Properties")
	float Happiness;

	
};
