// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime/Engine/Classes/Engine/Texture2D.h>
#include "mobile_nerf_module.h"
#include <../../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h>
//#include "nerf_types.h"
#include "NerfRegionActor.generated.h"

extern class FNerfRegion;
extern class FNerfMesh;


UCLASS()
class NERF_MANAGER_API ANerfRegionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANerfRegionActor();


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	UStaticMeshComponent* mesh_component = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	UProceduralMeshComponent* pro_mesh_component = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	UMaterial* mat = nullptr;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	UStaticMesh* mesh = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	UStaticMesh* mesh_t = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	TArray<UMaterialInstanceDynamic*> dyn_mat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "cfg");
	UMaterialInstanceDynamic* dyn_mat_pro = nullptr;

	UPROPERTY();
	UTexture2D* empty_mat = nullptr;
//	virtual ~ANerfRegionActor();

	void updata_mip();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void before_do(uint32_t model_id,uint32_t region_id);

	bool need_updata_pro_mesh = false;

	int pro_mesh_sub_cnt = 5;
	int current_sub_mesh_cnt = 0;
	int rand_idx = 0;
	int rand_idx_max = 10;
	TArray<int> last_mips_no_pro;
	TArray<FNerfMesh*> current_meshs;

	uint32_t cnt = 0;

	void updata_pro_mesh();
	void set_need_updata_pro_mesh(bool flag);

	void set_mesh(TArray<FNerfMesh*> meshs);

	void build_static_mesh(int min_idx);

};
