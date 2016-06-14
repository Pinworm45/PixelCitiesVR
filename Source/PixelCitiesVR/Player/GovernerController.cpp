// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCitiesVR.h"
#include "GovernerController.h"

AGovernerController::AGovernerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
}


