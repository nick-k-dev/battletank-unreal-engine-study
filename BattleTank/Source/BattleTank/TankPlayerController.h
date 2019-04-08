// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
	void BeginPlay() override;

private:
	ATank* GetControlledTank() const;
	
	//Start the tank moving the barrel so that a shot would hit where
	//the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector &) const;

	bool GetLookDirection(FVector2D const & screenLocation, FVector & lookDirection) const;

	bool GetLookVectorHitLocation(FVector & lookDirection, FVector & hitLocation) const;

	UPROPERTY(EditAnywhere)
	float crosshairXLocation = 0.5;
	UPROPERTY(EditAnywhere)
	float crosshairYLocation = 0.3333;
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000;

};
