// Fill out your copyright notice in the Description page of Project Settings.


#include "NerfManagerActor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "nerf_types.h"
#include<Runtime/Engine/Classes/Camera/CameraComponent.h>
#include"NerfCameraActor.h"


uint32_t binary_search(float val, const float* data, uint32_t length) {
	if (length == 0) {
		return 0;
	}

	uint32_t it;
	uint32_t count, step;
	count = length;

	uint32_t first = 0;
	while (count > 0) {
		it = first;
		step = count / 2;
		it += step;
		if (data[it] < val) {
			first = ++it;
			count -= step + 1;
		}
		else {
			count = step;
		}
	}
	return first > length - 1 ? length - 1 : first;
}

// Sets default values
ANerfManagerActor::ANerfManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->mesh_component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh_comp"));
	FNerfRegion::mip_dis.insert(std::make_pair(0, 6000));
	compress_mode = 3;
	FNerfRegion::mip_dis.insert(std::make_pair(2, 20000));
//	FNerfRegion::mip_dis.insert(std::make_pair(3, 50000));

//	FNerfRegion::mip_dis

	
	FNerfRegion::proc_mip = 5;
	for (auto iter = FNerfRegion::mip_dis.begin(); iter != FNerfRegion::mip_dis.end(); iter++) {
		if (iter->first < FNerfRegion::proc_mip)
			FNerfRegion::proc_mip = iter->first;
	}


}

ANerfManagerActor::~ANerfManagerActor() {
	models.Empty();
	regions.Empty();
	if (this->manager != nullptr)
		delete this->manager;
	free_global_root();
};

// Called when the game starts or when spawned
void ANerfManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANerfManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (first_run) {
		init();
	}
	first_run = false;


	TArray<ANerfCameraActor*> temp = ANerfCameraActor::cameras;
	auto primary_camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	TArray<FVector> dir_primary = Box::get_dirs(primary_camera->GetCameraLocation(), primary_camera->GetCameraRotation().Vector(), primary_camera->GetFOVAngle() * 0.8);
	TMap<FNerfRegion*,float> near_region;

	for (int j = 0; j < regions.Num(); j++) {
		auto& region = regions[j];
		if (region->region_box.is_load) {
			FVector2D dis = region->region_box.dirs_intersect(primary_camera->GetCameraLocation(), dir_primary, primary_camera->GetCameraRotation().Vector());
			if (dis.X < sigh_check_dis) {
				if (near_region.Contains(region) == false) {
					near_region.Add(region, dis.Y);
				}
				else {
					near_region[region] =std::max(near_region[region], dis.Y);
				}
			}
		}
	}
	for (int i = 0; i < temp.Num(); i++) {
		TArray<FVector> dir_temp = Box::get_dirs(temp[i]->get_loc(), temp[i]->get_dir(),temp[i]->GetCameraComponent()->FieldOfView*0.8);
		for (int j = 0; j < regions.Num(); j++) {
			auto& region = regions[j];
			if (region->region_box.is_load) {
				FVector2D dis = region->region_box.dirs_intersect(temp[i]->get_loc(), dir_temp, temp[i]->get_dir());
				if (dis.X < sigh_check_dis) {

					if (near_region.Contains(region) == false) {
						near_region.Add(region, dis.Y);
					}
					else {
						near_region[region] = std::max(near_region[region], dis.Y);
					}
				}
			}
		}		
	}

	for (int i = 0; i < regions.Num(); i++) {
		auto& region = regions[i];		

		FVector center = (region->region_box.max + region->region_box.min) / 2;
		center.Z = region->region_box.max.Z;

		if (near_region.Contains(region)) {
			region->update(manager, true, near_region[region]);
//			UKismetSystemLibrary::DrawDebugString(GetWorld(), center, FString::SanitizeFloat(region->min_dis), 0, FLinearColor::Blue, 0.1);

		}
		else {
			region->update(manager, false);
//			UKismetSystemLibrary::DrawDebugString(GetWorld(), center, FString::SanitizeFloat(region->min_dis), 0, FLinearColor::Red, 0.1);

		}
//		UKismetSystemLibrary::DrawDebugPoint(this->GetWorld(), region->get_center(), 10, FLinearColor::Blue, 0.1);
	}
	manager->update_infer_regions();
	manager->tick();

	for (int i = 0; i < regions.Num(); i++) {
		auto& region=regions[i];
		TArray<FNerfMesh*> meshs= region->get_active_mip_meshs();
		if (meshs.Num() > 0)
			regionactors[i]->set_mesh(meshs);
	}
//	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), primary_camera->GetCameraLocation(), primary_camera->GetCameraLocation() + primary_camera->GetCameraRotation().Vector() * 6000, 10, FLinearColor::Blue, 0.1, 5);

	
	/*
	for (int j = 0; j < regions.Num(); j++) {
		auto& region = regions[j];
		if (near_region.Contains(region)) {
			UKismetSystemLibrary::DrawDebugBox(GetWorld(), (region->region_box.max + region->region_box.min) / 2, (region->region_box.max - region->region_box.min) / 2, FLinearColor::Green, FRotator::ZeroRotator, 0.1, 3);

		}
		else {
			UKismetSystemLibrary::DrawDebugBox(GetWorld(), (region->region_box.max + region->region_box.min) / 2, (region->region_box.max - region->region_box.min) / 2, FLinearColor::Red, FRotator::ZeroRotator, 0.1, 3);

		}
	}
	for (int i = 0; i < dir_primary.Num(); i++) {
		FVector dir=dir_primary[i];
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), primary_camera->GetCameraLocation(), primary_camera->GetCameraLocation() + dir * 10000, 10, FLinearColor::Blue, 0.1, 5);
	}
	for (int i = 0; i < temp.Num(); i++) {
		TArray<FVector> dir_temp = Box::get_dirs(temp[i]->get_loc(), temp[i]->get_dir(), temp[i]->GetCameraComponent()->FieldOfView * 0.8);
		for (int j = 0; j < dir_temp.Num(); j++) {
			FVector dir = dir_temp[j];
			UKismetSystemLibrary::DrawDebugArrow(GetWorld(), temp[i]->get_loc(), temp[i]->get_loc() + dir * 10000, 10, FLinearColor::Blue, 0.1, 5);
		}
	}

	*/
}

void ANerfManagerActor::init() {
	if (manager == nullptr) {
		
		manager=get_Manager();
	}
	if (manager != nullptr) {
		manager->set_div_exp_factor(0);
		uint32_t gpu_ids[1];
		gpu_ids[0] = 0;
		manager->set_gpu_ids(1, gpu_ids);
		manager->set_compress_mode(compress_mode);
//		manager->set_div_exp_factor(1);

		std::string cfg_path_t = TCHAR_TO_UTF8(*cfg_path);
		char* path = const_cast<char*>(cfg_path_t.c_str());

		manager->set_scene_path(path);

		std::string model_path_t = TCHAR_TO_UTF8(*model_path);
		char* path_t = const_cast<char*>(model_path_t.c_str());
		manager->set_model_folder_path(path_t);

		manager->start();

		uint32_t model_num = manager->get_model_num();
		for (uint32_t i = 0; i < model_num; i++) {
			models.Add(new FNerfModel());
		}
//		models.SetNum(model_num);
		for (uint32_t i = 0; i < model_num; i++) {
			//			models.Add(FNerfModel());
			FNerfModel* model = models[i];
			model->model_id = i;
			model->init(this,manager);
		}
		for (uint32_t i = 0; i < model_num; i++) {
			FNerfModel* model = models[i];
			auto regs = model->get_regions();
			for (int j = 0; j < regs.Num(); j++)
				regions.Add(regs[j]);
		}
	}
	for (int i = 0; i < regions.Num(); i++) {
		FTransform trans;
		FVector location(regions[i]->get_center());
		trans.SetLocation(location);

		ANerfRegionActor* mesh=Cast<ANerfRegionActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ANerfRegionActor::StaticClass(), trans));
		mesh->before_do(regions[i]->model_id, regions[i]->region_id);
		UGameplayStatics::FinishSpawningActor(mesh, trans);
		regionactors.Add(mesh);
	}
};
