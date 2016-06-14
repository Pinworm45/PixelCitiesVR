// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Buildings/BaseBuilding.h"
#include "GovernerController.h"
#include "GovernerPawn.generated.h"

UCLASS()
class PIXELCITIESVR_API AGovernerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGovernerPawn(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Movement //
	void MoveForward(float Val);
	void MoveRight(float Val);
	void Zoom(float Val);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float MoveSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float ZoomSensitivity;

	/** Camera arm */
	UPROPERTY()
	USpringArmComponent* CameraArm;

	/** Camera arm max length, used for zoom */
	UPROPERTY()
	float CameraArmMaxLength;

	/** Camera arm min length, used for zoom */
	UPROPERTY()
	float CameraArmMinLength;

	/** Camera arm min length, used for rotate */
	UPROPERTY()
	float CameraMinPitch;

	/** Camera arm min length, used for rotate */
	UPROPERTY()
	float CameraMaxPitch;
	
	/** Camera */
	UPROPERTY()
	UCameraComponent* Camera;
	
	// Called when player left clicks
	UFUNCTION()
	void OnLeftClickPress();

	UFUNCTION()
	void OnLeftClickRelease();

	// Called when player right clicks
	UFUNCTION()
	void OnRightClick();

	// Called when player clicks middle mouse
	UFUNCTION()
	void OnMiddleClickPress();

	// Called when player released middle mouse
	UFUNCTION()
	void OnMiddleClickRelease();

	UFUNCTION()
	void Rotate(float Val);

	// Used to allow player to shift click and add more than 1 building at a time
	void OnBuildModifierPress();
	void OnBuildModifierRelease();
	bool bBuildModifier;

	// Used to determine whether player is holding middle mouse
	bool bCanRotate;

	// Called when building is clicked on interface
	UFUNCTION(BlueprintCallable, Category = "Building")
	void SelectedForPlacement(const TSubclassOf<ABaseBuilding> BuildingToMake);

	// Temporary building used as building ghost actor
	UPROPERTY(BlueprintReadOnly, Category = "Building")
	ABaseBuilding* BuildingGhost;

	// Temporary buildings used as building ghost actors
	UPROPERTY(BlueprintReadOnly, Category = "Building")
	TArray<ABaseBuilding*> BuildingGhosts;

	// Used for telling whether we are trying to place a building
	UPROPERTY(BlueprintReadOnly, Category = "Building")
	bool bPlacingBuilding;

	// Used for allowing us to drag building and build multiple at once
	bool bDraggingBuilding;

	// The player controller
	AGovernerController* PC;

	// Called to cancel building placement
	UFUNCTION(BlueprintCallable, Category = "Building")
		void CancelPlacing();

	UPROPERTY(BlueprintReadOnly, Category = "City")
		int32 CurrentPopulation;

	UPROPERTY(BlueprintReadWrite, Category = "City")
		int32 CurrentMoney;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "City")
		TArray<int32> StoredResources;

	UPROPERTY(BlueprintReadOnly, Category = "City")
		int32 InitialTaxUpgradeCost;

	UPROPERTY(BlueprintReadWrite, Category = "City")
		int32 CurrentTaxUpgradeCost;

	TSubclassOf<class ABaseBuilding> CurrentBuildingType;

	UFUNCTION()
	FHitResult GetMouseHit();

	float DistX;

	float DistY;

	FVector CurrentMouseLoc;

};