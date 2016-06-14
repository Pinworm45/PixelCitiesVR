// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCitiesVR.h"
#include "Citizen.h"


// Sets default values
ACitizen::ACitizen()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACitizen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACitizen::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ACitizen::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ACitizen::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}