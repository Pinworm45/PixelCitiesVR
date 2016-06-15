// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/BaseBusiness.h"
#include "BaseEntertainment.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PIXELCITIESVR_API ABaseEntertainment : public ABaseBusiness
{
	GENERATED_BODY()

	// Sets default values for this actor's properties
	ABaseEntertainment(const FObjectInitializer& ObjectInitializer);

public:

	UPROPERTY(EditDefaultsOnly, Category = "Entertainment")
	int32 HappinessValue;


	
	
};
