// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCitiesVR.h"
#include "Player/GovernerPawn.h"
#include "BaseBuilding.h"

// Sets default values
ABaseBuilding::ABaseBuilding(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up mesh
	BuildingMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BuildingMesh"));
	RootComponent = BuildingMesh;

	// On Actor Begin Overlap
	OnActorBeginOverlap.AddDynamic(this, &ABaseBuilding::OnBeginOverlap);

	// On Actor End Overlap
	OnActorEndOverlap.AddDynamic(this, &ABaseBuilding::OnEndOverlap);

	bGhostBuilding = false;

	// Set default costs and stats
	Cost = 0;
	BuildingProgress = 0;

	DefaultMaterial = BuildingMesh->GetMaterial(0);

	BuildingMesh->SetCollisionProfileName("BuildingCollision");

}

// Called when the game starts or when spawned
void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when item is clicked to place down
void ABaseBuilding::PlaceBuilding_Implementation()
{
	//	bGhostBuilding = false;
	BuildingMesh->SetRenderCustomDepth(false);
}

// Called when item is flagged for destruction
void ABaseBuilding::DestroyBuilding()
{

}

// Called when other actor is overlapping
void ABaseBuilding::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bGhostBuilding)
	{
		BuildingMesh->SetRenderCustomDepth(true);
		UE_LOG(LogBuilding, Log, TEXT("Colliding with building"));
		OverlappingActors.AddUnique(OtherActor);
	}
}

void ABaseBuilding::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappingActors.Remove(OtherActor);
	if (OverlappingActors.Num() < 1)
	{
		UE_LOG(LogBuilding, Log, TEXT("Collision end"));
		BuildingMesh->SetRenderCustomDepth(false);
	}
}

void ABaseBuilding::StartDeletion()
{
	//	UE_LOG(LogTemp, Log, TEXT("Start deletion called on basebuilding"));
}