// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "mobile_nerf_module.h"
#include "NerfRegionActor.h"
#include "NerfManagerActor.generated.h"

extern class FNerfModel;
extern class FNerfRegion;

UCLASS()
class NERF_MANAGER_API ANerfManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANerfManagerActor();
	virtual ~ANerfManagerActor();
	NerfManager* manager = nullptr;

	UPROPERTY();
	TSet<UTexture2D*> textures;

	TArray<FNerfModel*> models;

	TArray<FNerfRegion*> regions;
	TArray<ANerfRegionActor*> regionactors;



	bool first_run = true;

	int compress_mode = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	float sigh_check_dis = 6000;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	UStaticMeshComponent* mesh_component = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	FString cfg_path = "E:/workspace/workflow/work_folder2/scene/region_cfg_s.json";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	FString model_path = "E:/workspace/workflow/work_folder2/output_models/";

	void init();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
