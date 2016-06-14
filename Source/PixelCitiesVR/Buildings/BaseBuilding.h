// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseBuilding.generated.h"

// forward declarations
class AGovernerPawn;

UCLASS()
class PIXELCITIESVR_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ABaseBuilding(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Mesh for the building
	UPROPERTY(VisibleDefaultsOnly, Category = "Graphics")
	UStaticMeshComponent* BuildingMesh;

	// Mesh for the building during construction
	UPROPERTY(EditDefaultsOnly, Category = "Graphics")
	UStaticMesh* ScaffoldMesh;

	// Material while building is not yet placed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Graphics")
	UMaterial* PlacingMaterial;

	UPROPERTY()
	UMaterialInterface* DefaultMaterial;

	// Building cost
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Cost;

	// Building construction progress
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float BuildingProgress;

	// Called to place building
	UFUNCTION(BlueprintNativeEvent, Category = "Building")
	void PlaceBuilding();

	// Called to delete building
	UFUNCTION()
	void DestroyBuilding();

	// Used to tell if building is placed or waiting to be placed
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	bool bGhostBuilding;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// Array of actors overlapping building, used to determine whether building is valid to place or not
	UPROPERTY()
	TArray<AActor*> OverlappingActors;

	UFUNCTION(BlueprintCallable, Category = "Building")
	void StartDeletion();
	
};
