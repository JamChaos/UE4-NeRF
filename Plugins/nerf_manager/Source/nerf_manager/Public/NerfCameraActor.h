// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "NerfCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class NERF_MANAGER_API ANerfCameraActor : public ACameraActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	static TArray<ANerfCameraActor*> cameras;
	ANerfCameraActor();
//	virtual ~ANerfCameraActor();
	virtual void Tick(float DeltaTime) override;

	FVector get_loc();
	FVector get_dir();
};
