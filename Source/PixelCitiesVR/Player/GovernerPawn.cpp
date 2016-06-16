// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCitiesVR.h"
#include "GovernerPawn.h"


// Sets default values
AGovernerPawn::AGovernerPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComp"));

	CameraArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraArm"));
	CameraArm->RelativeRotation.Pitch = -60.0f;
	CameraArm->TargetArmLength = 1800;
	CameraArmMaxLength = 3600;
	CameraArmMinLength = 200;
	CameraMaxPitch = 60.0f;
	CameraMinPitch = 15.0f;
	CameraArm->bDoCollisionTest = false;
	CameraArm->ProbeChannel = ECC_GameTraceChannel1;
	CameraArm->AttachToComponent(SceneComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->AttachToComponent(CameraArm, FAttachmentTransformRules::SnapToTargetIncludingScale);

	MoveSensitivity = 30;
	ZoomSensitivity = 150;

	bPlacingBuilding = false;
	bDraggingBuilding = false;
	bCanRotate = false;

	CurrentMoney = 1500;

	bBuildModifier = false;
	bBuildMode = false;
}

// Called when the game starts or when spawned
void AGovernerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	PC = Cast<AGovernerController>(GetController());
}

// Called every frame
void AGovernerPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bPlacingBuilding && !bDraggingBuilding)
	{
		if (BuildingGhost != nullptr)
		{
			// nearest grid size to snap to
			int32 Nearest = 500;
			FVector Loc = FVector::ZeroVector;
			// If we're using HMD (VR) controls
			if (GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHeadTrackingAllowed())
			{
				Loc = GetHMDHit().Location;
			}
			// If we're using non VR controls
			else
			{
				Loc = GetMouseHit().Location;
			}

			// set currently placing building to desired location
			FVector GridLoc = FVector(RoundToNearest(Loc.X, 500), RoundToNearest(Loc.Y, 500), Loc.Z);
			BuildingGhost->SetActorLocation(GridLoc);
		}
	}

	if (bDraggingBuilding)
	{

	}
}

float AGovernerPawn::RoundToNearest(float Value, int32 Nearest)
{
	float OutVal = Value + 50;
	int32 Floor = FMath::FloorToInt(OutVal);
	Floor /= Nearest;
	OutVal = Floor * Nearest;
//	UE_LOG(LogTemp, Log, TEXT("OutVal = %f"), OutVal);
	return OutVal;
}

// Called to bind functionality to input
void AGovernerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AGovernerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGovernerPawn::MoveRight);
	InputComponent->BindAxis("ScrollWheel", this, &AGovernerPawn::Zoom);
	InputComponent->BindAxis("Rotate", this, &AGovernerPawn::Rotate);

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AGovernerPawn::OnLeftClickPress);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AGovernerPawn::OnLeftClickRelease);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AGovernerPawn::OnRightClick);
	InputComponent->BindAction("MiddleMouseClick", IE_Pressed, this, &AGovernerPawn::OnMiddleClickPress);
	InputComponent->BindAction("MiddleMouseClick", IE_Released, this, &AGovernerPawn::OnMiddleClickRelease);
	InputComponent->BindAction("BuildModifier", IE_Pressed, this, &AGovernerPawn::OnBuildModifierPress);
	InputComponent->BindAction("BuildModifier", IE_Released, this, &AGovernerPawn::OnBuildModifierRelease);
}

FHitResult AGovernerPawn::GetMouseHit()
{
	FVector MouseLocation = FVector::ZeroVector;
	FVector MouseDirection = FVector::ZeroVector;
	if (PC)
	{
		PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	}
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	FVector TraceStart = MouseLocation;
	FVector TraceEnd = MouseLocation + (MouseDirection * 100000);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel2, QueryParams);
	return HitResult;
}

FHitResult AGovernerPawn::GetHMDHit()
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceEnd = Camera->GetComponentLocation() + (Camera->GetForwardVector() * 100000);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel2, QueryParams);
	return HitResult;
}

void AGovernerPawn::MoveForward(float Val)
{
	FVector NewLocation = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	Rotation.Yaw = GetControlRotation().Yaw;
	NewLocation = GetActorLocation() + (Val * MoveSensitivity) * Rotation.Vector();
	SetActorLocation(NewLocation);
}

void AGovernerPawn::MoveRight(float Val)
{
	float Movement = Val * MoveSensitivity;
	FVector NewLocation = FVector::ZeroVector;
	FRotator ControlRot = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	NewLocation = Val * MoveSensitivity * (FRotator(0.0f, 90.0f, 0.0f) + ControlRot).Vector();
	SetActorLocation(GetActorLocation() + NewLocation);
}

void AGovernerPawn::Zoom(float Val)
{
	if (!bPlacingBuilding)
	{
		float NewValue = FMath::Clamp(CameraArm->TargetArmLength - Val * ZoomSensitivity, CameraArmMinLength, CameraArmMaxLength);

		CameraArm->TargetArmLength = NewValue;
	}

	if (bPlacingBuilding)
	{
		FRotator NewRotation = FRotator::ZeroRotator;
		NewRotation.Yaw += Val * 90;
		FRotator New = BuildingGhost->GetActorRotation() + NewRotation;
		BuildingGhost->SetActorRotation(New);
	}
}

// called when item is clicked on in interface
void AGovernerPawn::SelectedForPlacement(const TSubclassOf<ABaseBuilding> BuildingToMake)
{
	if (BuildingToMake)
	{
		CurrentBuildingType = BuildingToMake;
		BuildingGhost = GetWorld()->SpawnActor<ABaseBuilding>(BuildingToMake);
		BuildingGhost->BuildingMesh->SetMaterial(0, BuildingGhost->PlacingMaterial);
		BuildingGhost->bGhostBuilding = true;
		bPlacingBuilding = true;
	}
}

// Used to interact with and place buildings
void AGovernerPawn::OnLeftClickPress()
{
	/*
	// If we're placing a building
	if (bPlacingBuilding)
	{
		if (!bDraggingBuilding)
		{
			bDraggingBuilding = true;

			//			for (int32 i = 0; i < DistX; i++)
			{

			}
		}
	}
	*/
}

void AGovernerPawn::OnLeftClickRelease()
{
	// if we're set to build mode
	if (bBuildMode)
	{
		// If we're placing a building
		if (bPlacingBuilding)
		{
			// and if we aren't overlapping an actor
			if (BuildingGhost->OverlappingActors.Num() < 1)
			{
				// and if we can afford to place a building
				if (CurrentMoney >= BuildingGhost->Cost)
				{
					// take away gold that it costs, and place the building
					CurrentMoney -= BuildingGhost->Cost;
					FTransform NewTransform = BuildingGhost->GetTransform();
					ABaseBuilding* NewBuilding = GetWorld()->SpawnActor<ABaseBuilding>(CurrentBuildingType, NewTransform);
					NewBuilding->PlaceBuilding();
					NewBuilding->SetOwner(this);

					// if we're not holding shift, remove building ghost
					if (!bBuildModifier)
					{
						if (BuildingGhost)
						{
							BuildingGhost->Destroy();
							BuildingGhost = nullptr;
							bPlacingBuilding = false;
							CurrentBuildingType = nullptr;
							NewBuilding = nullptr;
						}
					}
				}
			}
		}
		bBuildMode = false;
	}

	bDraggingBuilding = false;
}

// Used to cancel actions
void AGovernerPawn::OnRightClick()
{
	CancelPlacing();
}

void AGovernerPawn::OnMiddleClickPress()
{
	bCanRotate = true;
}

void AGovernerPawn::OnMiddleClickRelease()
{
	bCanRotate = false;
}

void AGovernerPawn::CancelPlacing()
{
	if (bPlacingBuilding)
	{
		bPlacingBuilding = false;
		BuildingGhost->Destroy();
		BuildingGhost = nullptr;
		CurrentBuildingType = nullptr;
	}
}

void AGovernerPawn::Rotate(float Val)
{
	if (bCanRotate)
	{
		AddControllerYawInput(Val);
	}
}

void AGovernerPawn::OnBuildModifierPress()
{
	bBuildModifier = true;
}

void AGovernerPawn::OnBuildModifierRelease()
{
	bBuildModifier = false;
}