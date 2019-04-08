// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Begin Play"))
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not possessing a tank"))
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing: %s"), *(ControlledTank->GetName()))
	}
}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {	
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair() {
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // Out parameter
	if (GetSightRayHitLocation(HitLocation)) { //Has "side-effect", is going to line trace
		UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *(HitLocation.ToString()))
		
		// TODO Tell controlled tank to aim at this point
	}
}

//Get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector & OutHitLocation) const
{
	//Find the crosshair position
	int32 viewportSizeX, viewportSizeY;
	GetViewportSize(viewportSizeX, viewportSizeY);
	auto screenLocation = FVector2D(viewportSizeX * crosshairXLocation, viewportSizeY * crosshairYLocation);
	UE_LOG(LogTemp, Warning, TEXT("ScreenLocation: %s"), *screenLocation.ToString())
		//"De-project" the screen position of the crosshair to a world direction
	FVector lookDirection;
	if (GetLookDirection(screenLocation, lookDirection)) {
		//Line-trace along that look direction, and see what we hit (up to max range)
		GetLookVectorHitLocation(lookDirection, OutHitLocation);
	}

	return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector & lookDirection, FVector & hitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + lookDirection * LineTraceRange;
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility)) {
		hitLocation = HitResult.Location;
		return true;
	}
	hitLocation = FVector(0);
	return false; //Line trace didn't succeed
}

bool ATankPlayerController::GetLookDirection(FVector2D const & screenLocation, FVector& lookDirection) const {
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(
		screenLocation.X, 
		screenLocation.Y, 
		CameraWorldLocation, 
		lookDirection
	);
}
