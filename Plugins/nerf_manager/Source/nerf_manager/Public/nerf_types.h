#pragma once
#include <cmath>
#include <iostream>
#include "mobile_nerf_module.h"
#include <mutex>
#include <map>
#include<vector>
#include <RawMesh/Public/RawMesh.h>
#include <Runtime/Engine/Classes/Engine/Texture2D.h>
#include<NerfRegionActor.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "mobile_nerf_module.h"
//#include "NerfManagerActor.h"

void free_global_root();

extern class ANerfManagerActor;

class Box {
public:
	bool is_load = false;
	FVector min;
	FVector max;
	FVector get_center() {
		return (min + max) / 2;
	};


	FVector2D ray_intersect(const FVector& pos, const FVector& dir) {
		float tmin = (min.X - pos.X) / dir.X;
		float tmax = (max.X - pos.X) / dir.X;

		if (tmin > tmax) {
			float temp = tmin;
			tmin = tmax;
			tmax = temp;
		}

		float tymin = (min.Y - pos.Y) / dir.Y;
		float tymax = (max.Y - pos.Y) / dir.Y;

		if (tymin > tymax) {
			float temp = tymin;
			tymin = tymax;
			tymax = temp;
		}

		if (tmin > tymax || tymin > tmax) {
			return { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
		}

		if (tymin > tmin) {
			tmin = tymin;
		}

		if (tymax < tmax) {
			tmax = tymax;
		}

		float tzmin = (min.Z - pos.Z) / dir.Z;
		float tzmax = (max.Z - pos.Z) / dir.Z;

		if (tzmin > tzmax) {
			float temp = tzmin;
			tzmin = tzmax;
			tzmax = temp;
		}

		if (tmin > tzmax || tzmin > tmax) {
			return { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
		}

		return { tmin, tmax };

	}
	static TArray<FVector> get_dirs(const FVector& pos, const FVector& dir, float fov = 60,int split = 5 ) {
		TArray<FVector> dirs;
		FQuat rot = dir.ToOrientationRotator().Quaternion();
		FVector up = rot.GetUpVector();
		FVector side = rot.GetRightVector();

		float half_fov = fov / 2;
		float fov_x_start = -fov / 2;
		float fov_y_start = -fov / 2;

		float fov_x_step = fov / (split - 1);
		float fov_y_step = fov / (split - 1);


		for (int i = 0; i < split; i++) {
			FVector rot_around_up = dir.RotateAngleAxis(i * (fov_x_step)+fov_x_start, up);
			for (int j = 0; j < split; j++) {
				FVector rot_around_side = rot_around_up.RotateAngleAxis(j * (fov_y_step)+fov_y_start, side);
				dirs.Add(rot_around_side);
			}
		}
		return dirs;
	};
	FVector2D dirs_intersect(FVector pos,TArray<FVector> dirs,FVector center_dir) {
		FVector2D dis_res(10000000, 10000000);
		for (int i = 0; i < dirs.Num(); i++) {
			FVector2D dis = ray_intersect(pos, dirs[i]);
			if (dis.X < dis_res.X) {
				dis.Y = FVector::DotProduct(center_dir.GetSafeNormal(), dirs[i].GetSafeNormal());
			
			}
			else {
				dis.Y = 0;
			}
			if (i == 0) {

				dis_res = dis;

			}
			else {

				dis_res.X = std::min(dis_res.X, dis.X);
				dis_res.Y = std::max(dis_res.Y, dis.Y);
			}
		}
		return dis_res;
	};
	FVector2D dir_intersect(const FVector& pos, const FVector& dir, int split = 5, float fov = 60) {
		TArray<FVector> dirs = this->get_dirs(pos, dir, split, fov);
		FVector2D dis_res(10000000, 10000000);
		for (int i = 0; i < dirs.Num(); i++) {
			FVector2D dis = ray_intersect(pos, dirs[i]);
			if (i == 0)
				dis_res = dis;
			else {
				dis_res.X = std::min(dis_res.X, dis.X);
				dis_res.Y = std::max(dis_res.X, dis.Y);
			}
		}
		return dis_res;
	};
};

//USTRUCT()
class FNerfMesh {
//	GENERATED_BODY()
public:
	std::uint32_t region_id;
	std::uint32_t model_id;
	std::uint32_t mip_id;

	FRawMesh mesh;
	UPROPERTY()
	TArray<UTexture2D* > feature_maps;

	UPROPERTY()
	UTexture2D* thres_map = nullptr;
	UPROPERTY()
	TArray<UTexture2D* > params_maps;
	bool is_loading = false;

	bool is_run_async1 = false;
	bool is_run_async2 = false;

	bool has_mesh = false;
	bool has_maps = false;
	std::mutex read_lock;
	FVector center;

	explicit FNerfMesh(FNerfMesh& other);

	FNerfMesh();
	~FNerfMesh();

	bool check_load_stata();

	bool mesh_from_nerf(NerfManager* manager);
	bool feature_maps_from_nerf(NerfManager* manager);

	void free_feature_maps();
	void free_mesh();
	void get_uncompact(int start_face_ind, int end_face_ind, TArray<FVector>& verts, TArray<uint32>& face_inds, TArray<FVector2D>& uvs);
	void get_uncompact(int start_face_ind, int end_face_ind, TArray<FVector>& verts, TArray<int>& face_inds, TArray<FVector2D>& uvs);
	bool need_get_feature_map();
};

//USTRUCT()
class FNerfRegion {
public:
//	GENERATED_BODY()
//	ANerfRegionActor* target = nullptr;
//	UPROPERTY(NoClear)

	Box region_box;

	TArray<FNerfMesh> meshs;
	static std::map<uint32_t, float> mip_dis;
	static uint32_t proc_mip;
	TArray<uint32_t> mip_list;
	uint32_t current_mip;
	uint32_t last_mip;

	std::uint32_t region_id;
	std::uint32_t model_id;
	FNerfRegion();
	~FNerfRegion();
	void update_mip(NerfManager* manager, bool high_prop=false);
	void update(NerfManager* manager,bool high_prop=false, float dir_cos=0);
	void init(NerfManager* manager);
	TArray<uint32_t> get_active_mip_idx();
	TArray<FNerfMesh*> get_active_mip_meshs();

	FVector get_center();
	FVector get_mesh_center();

	uint32_t mip_idx_to_mip(uint32_t mip_idx);
	TArray<uint32_t> get_need_get_mip();
	TArray<uint32_t> get_need_free_mip();
	TArray<uint32_t> get_need_get_mip_idx();
	TArray<uint32_t> get_need_free_mip_idx();

	float min_dis = 1e32;

};

class FNerfModel {
public:


//	GENERATED_BODY()
	std::uint32_t model_id;
//	UPROPERTY(NoClear)
	TArray<FNerfRegion> regions;

	FNerfModel();
	~FNerfModel();
	void init(ANerfManagerActor* root,NerfManager* manager);
	TArray<FNerfRegion*> get_regions();
};

