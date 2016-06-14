// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelCitiesVR.h"
#include "GovernerPawn.h"


// Sets default values
AGovernerPawn::AGovernerPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraArm"));
	CameraArm->RelativeRotation.Pitch = -60.0f;
	CameraArm->TargetArmLength = 1800;
	CameraArmMaxLength = 3600;
	CameraArmMinLength = 200;
	CameraMaxPitch = 60.0f;
	CameraMinPitch = 15.0f;
	CameraArm->bDoCollisionTest = false;
	CameraArm->ProbeChannel = ECC_GameTraceChannel1;

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepWorldTransform);

	MoveSensitivity = 30;
	ZoomSensitivity = 150;

	bPlacingBuilding = false;
	bDraggingBuilding = false;
	bCanRotate = false;

	CurrentMoney = 1500;
	InitialTaxUpgradeCost = 50;
	CurrentTaxUpgradeCost = 50;

	bBuildModifier = false;

	DistX = 0.0f;
}

// Called when the game starts or when spawned
void AGovernerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGovernerPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bPlacingBuilding && !bDraggingBuilding)
	{
		if (BuildingGhost != nullptr)
		{
			FVector GridLoc = FVector(GetMouseHit().Location.X + ((50 / 200) * 200), GetMouseHit().Location.Y + ((50 / 200) * 200), GetMouseHit().Location.Z);
			BuildingGhost->SetActorLocation(GridLoc);
		}
	}

	if (bDraggingBuilding)
	{
		DistX = GetMouseHit().Location.X - CurrentMouseLoc.X;
		DistY = GetMouseHit().Location.Y - CurrentMouseLoc.Y;
		//		UE_LOG(LogTemp, Log, TEXT("DistX %f"), DistX);
	}
}

// Called to bind functionality to input
void AGovernerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AGovernerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGovernerPawn::MoveRight);
	InputComponent->BindAxis("Zoom", this, &AGovernerPawn::Zoom);
	InputComponent->BindAxis("Rotate", this, &AGovernerPawn::Rotate);

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AGovernerPawn::OnLeftClickPress);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AGovernerPawn::OnLeftClickRelease);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AGovernerPawn::OnRightClick);
	InputComponent->BindAction("MouseWheelClick", IE_Pressed, this, &AGovernerPawn::OnMiddleClickPress);
	InputComponent->BindAction("MouseWheelClick", IE_Released, this, &AGovernerPawn::OnMiddleClickRelease);
	InputComponent->BindAction("BuildModifier", IE_Pressed, this, &AGovernerPawn::OnBuildModifierPress);
	InputComponent->BindAction("BuildModifier", IE_Released, this, &AGovernerPawn::OnBuildModifierRelease);
}

FHitResult AGovernerPawn::GetMouseHit()
{
	FVector MouseLocation = FVector::ZeroVector;
	FVector MouseDirection = FVector::ZeroVector;
	PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	FVector TraceStart = MouseLocation;
	FVector TraceEnd = MouseLocation + (MouseDirection * 100000);
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
		NewRotation.Yaw += Val * 15;
		FRotator New = BuildingGhost->GetActorRotation() + NewRotation;
		BuildingGhost->SetActorRotation(New);
	}
}

// called when item is clicked on in interface
void AGovernerPawn::SelectedForPlacement(const TSubclassOf<ABaseBuilding> BuildingToMake)
{
	CurrentBuildingType = BuildingToMake;
	BuildingGhost = GetWorld()->SpawnActor<ABaseBuilding>(BuildingToMake);
	BuildingGhost->BuildingMesh->SetMaterial(0, BuildingGhost->PlacingMaterial);
	BuildingGhost->bGhostBuilding = true;
	bPlacingBuilding = true;
}

// Used to interact with and place buildings
void AGovernerPawn::OnLeftClickPress()
{
	// If we're placing a building
	if (bPlacingBuilding)
	{
		if (!bDraggingBuilding)
		{
			bDraggingBuilding = true;
			CurrentMouseLoc = GetMouseHit().Location;

			//			for (int32 i = 0; i < DistX; i++)
			{
				//				BuildingGhosts.SetNumUninitialized(i);
				//				BuildingGhosts[i] = GetWorld()->SpawnActor<ABaseBuilding>(BuildingGhost->GetClass());
				//				BuildingGhosts[i]->SetActorLocation(CurrentMouseLoc + (50 * i));

			}
		}
	}
}

void AGovernerPawn::OnLeftClickRelease()
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
				CurrentMoney = CurrentMoney - BuildingGhost->Cost;
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