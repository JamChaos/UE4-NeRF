// Fill out your copyright notice in the Description page of Project Settings.


#include "NerfRegionActor.h"
#include "nerf_types.h"

/*
ANerfRegionActor::~ANerfRegionActor() {
	for (int i = 0; i < dyn_mat.Num(); i++) {
		delete dyn_mat[i];
	}
	dyn_mat.Empty();
	if(mesh!=nullptr)
		delete mesh;
	if(mesh_t!=nullptr)
		delete mesh_t;
};*/
void ANerfRegionActor::before_do(uint32_t model_id, uint32_t region_id) {
		FString PathName = FString(TEXT("/Game/temp_Mesh/"));
		FString PackageName = PathName + "_";
		FString outMeshName = "";
		outMeshName.AppendInt(model_id);
		outMeshName += "_";
		outMeshName.AppendInt(region_id);
		UPackage* Package = CreatePackage(NULL, *PackageName);
		mesh = NewObject<UStaticMesh>(Package, FName(*outMeshName), RF_Public | RF_Standalone);
		mesh_t = NewObject<UStaticMesh>(Package, FName(*(outMeshName + "_t")), RF_Public | RF_Standalone);

//		dyn_mat.Reset(region->meshs.size());
		dyn_mat.Empty();
		
		for (int i = 0; i < 5; i++) {
			FString name = FString("dyn_mat_");
			name.Append(this->GetFName().ToString() + "_");
			name.AppendInt(i);
			dyn_mat.Add( UMaterialInstanceDynamic::Create(mat, this, FName(name)));
		}
		dyn_mat_pro=UMaterialInstanceDynamic::Create(mat, this, FName(FString("dyn_mat_pro")));


};


ANerfRegionActor::ANerfRegionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->mesh_component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh_comp"));
	this->pro_mesh_component = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("pro_mesh_comp"));
	pro_mesh_component->bUseAsyncCooking = true;
	pro_mesh_component->SetCastInsetShadow(false);
	pro_mesh_component->SetCastShadow(false);
	pro_mesh_component->SetEnableGravity(false);
	//	this->mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh_comp"));
	auto  mat_finder = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/nerf_manager/NerfObjectMat.NerfObjectMat'"));
	if (mat_finder.Succeeded()) {
		mat = mat_finder.Object;
	}
	empty_mat = UTexture2D::CreateTransient(1, 1, PF_R8G8B8A8);
	rand_idx = rand() % rand_idx_max;
}


// Called when the game starts or when spawned
void ANerfRegionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANerfRegionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	updata_mip();
	if (current_meshs.Num() > 0) {
		this->SetActorLocation(current_meshs[0]->center);
		this->pro_mesh_component->SetWorldLocation(current_meshs[0]->center);


	}
//	FVector loc = this->pro_mesh_component->GetComponentLocation();
	updata_pro_mesh();

}

bool check_meshs_same(TArray<FNerfMesh*> meshs, TArray<FNerfMesh*> current_meshs) {
	bool is_same = true;
	if (meshs.Num() != current_meshs.Num())
		is_same = false;
	if ((meshs.Num() == current_meshs.Num() && meshs.Num() == 0))
		is_same = true;

	if (is_same) {
		for (int i = 0; i < meshs.Num(); i++) {
			if (current_meshs.Contains(meshs[i]) == false) {
				is_same = false;
				break;
			}
		}
	}
	if (is_same) {
		for (int i = 0; i < current_meshs.Num(); i++) {
			if (meshs.Contains(current_meshs[i]) == false) {
				is_same = false;
				break;
			}
		}
	}
	return is_same;
};

void ANerfRegionActor::set_mesh(TArray<FNerfMesh*> meshs) {

	bool is_same = check_meshs_same(meshs, current_meshs);
	if (is_same == false) {
		bool need_proc_mesh = false;
		for (int i = 0; i < meshs.Num(); i++) {

			if (meshs[i]->mip_id == FNerfRegion::proc_mip)
			{
				need_proc_mesh = true;
				break;
			}

		}
		if (meshs.Num() <= 1)
			need_proc_mesh = false;
		bool need_rebuild_static_mesh = false;
		if ((need_proc_mesh == this->need_updata_pro_mesh)) {
			if (meshs.Num() != current_meshs.Num())
				need_rebuild_static_mesh = true;
		}
		if (need_rebuild_static_mesh == false) {
			if (need_proc_mesh && !this->need_updata_pro_mesh) {
				TArray<FNerfMesh*> temp_meshs;
				for (int i = 1; i < meshs.Num(); i++) {
					temp_meshs.Add(meshs[i]);
				}
				need_rebuild_static_mesh = !check_meshs_same(temp_meshs, current_meshs);
			}
			else if (!need_proc_mesh && this->need_updata_pro_mesh) {
				TArray<FNerfMesh*> temp_meshs;
				for (int i = 1; i < current_meshs.Num(); i++) {
					temp_meshs.Add(current_meshs[i]);
				}
				need_rebuild_static_mesh = !check_meshs_same(meshs, temp_meshs);

			}
		}
		current_meshs = meshs;
		if (need_proc_mesh) {
			set_need_updata_pro_mesh(true);
			if(need_rebuild_static_mesh)
				build_static_mesh(1);
		}
		else {
			set_need_updata_pro_mesh(false);
			if (need_rebuild_static_mesh)
				build_static_mesh(0);
		}
	}

};

void ANerfRegionActor::build_static_mesh(int min_idx) {
	mesh_t->ClearMeshDescriptions();
	mesh_t->SetNumSourceModels(0);
	mesh_t->StaticMaterials.Empty();

	for (int i = min_idx; i < current_meshs.Num(); i++) {
		FStaticMeshSourceModel& SrcModel = mesh_t->AddSourceModel();
		SrcModel.BuildSettings.bRecomputeNormals = false;
		SrcModel.BuildSettings.bRecomputeTangents = false;
		SrcModel.BuildSettings.bRemoveDegenerates = false;
		SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
		SrcModel.BuildSettings.bUseFullPrecisionUVs = true;
		SrcModel.BuildSettings.bGenerateLightmapUVs = false;
		SrcModel.BuildSettings.SrcLightmapIndex = 0;
		SrcModel.BuildSettings.DstLightmapIndex = 1;
		SrcModel.SaveRawMesh(current_meshs[i]->mesh);
		SrcModel.ScreenSize = powf(0.45, i- min_idx+1); //region->mip_idx_to_mip(active_mip[i])

	}
	mesh_t->bAutoComputeLODScreenSize = false;
	mesh_t->ImportVersion = EImportStaticMeshVersion::LastVersion;


	for (int i = min_idx; i < current_meshs.Num(); i++) {
		for (int j = 0; j < current_meshs[i]->feature_maps.Num(); j++) {
			FString f_name = "feature";
			f_name.AppendInt(j);
			dyn_mat[i]->SetTextureParameterValue(FName(*f_name), current_meshs[i] -> feature_maps[j]);
		}
		if (current_meshs[i]->feature_maps.Num() == 3) {
			dyn_mat[i]->SetScalarParameterValue(FName("mode_switch"), 1);
		}
		else {
			dyn_mat[i]->SetScalarParameterValue(FName("mode_switch"), 0);

		}

		for (int j = 0; j < current_meshs[i]->params_maps.Num(); j++) {
			FString f_name = "params";
			f_name.AppendInt(j);
			dyn_mat[i]->SetTextureParameterValue(FName(*f_name), current_meshs[i]->params_maps[j]);
		}
		FString f_name = "thres";
		dyn_mat[i]->SetTextureParameterValue(FName(*f_name), current_meshs[i]->thres_map);
		mesh_t->AddMaterial(dyn_mat[i]);
	}
	for (int i = 0; i < min_idx; i++) {
		dyn_mat[i]->ClearParameterValues();
	}

	for (int i = current_meshs.Num(); i < 5; i++) {
		dyn_mat[i]->ClearParameterValues();
	}
	mesh_t->SectionInfoMap.Clear();
	for (int i = min_idx; i < current_meshs.Num(); i++) {
		FMeshSectionInfo info;
		info.bCastShadow = false;
		info.bEnableCollision = false;
		info.MaterialIndex = i- min_idx;
		mesh_t->SectionInfoMap.Set(i - min_idx, 0, info);
	}
	mesh_t->OriginalSectionInfoMap.CopyFrom(mesh_t->SectionInfoMap);

	mesh_t->Build(true);
	mesh_t->PostEditChange();

	this->mesh_component->SetStaticMesh(mesh_t);
	for (int i = min_idx; i < current_meshs.Num(); i++) {
		this->mesh_component->SetMaterial(i- min_idx, dyn_mat[i]);
	}

	mesh_component->PostEditChange();

};

void ANerfRegionActor::updata_pro_mesh() {
	cnt++;
	if (cnt%rand_idx_max!=rand_idx) {
		return;
	}
	if (cnt > 10000000)
		cnt = 0;
	if (this->need_updata_pro_mesh) {
		if (current_sub_mesh_cnt == pro_mesh_sub_cnt)
		{
			this->pro_mesh_component->SetVisibility(true);
			current_sub_mesh_cnt++;
			return;

		}
		if (current_sub_mesh_cnt > pro_mesh_sub_cnt) {
			this->mesh_component->SetVisibility(false);
			return;

		}
		TArray<FVector> empty_normal;
		TArray<FColor> empty_color;
		TArray<FProcMeshTangent> empty_tangent;
		TArray<int> face_inds;
		TArray<FVector> verts;
		TArray<FVector2D> uvs;

		int split_num = ((current_meshs[0]->mesh.WedgeIndices.Num() + pro_mesh_sub_cnt - 1) / pro_mesh_sub_cnt+2)/3*3;
		int start_idx = current_sub_mesh_cnt * split_num;
		int end_idx = (current_sub_mesh_cnt + 1) * split_num;
		if (end_idx > current_meshs[0]->mesh.WedgeIndices.Num())
			end_idx = current_meshs[0]->mesh.WedgeIndices.Num();

		current_meshs[0]->get_uncompact(start_idx, end_idx, verts, face_inds, uvs);

		pro_mesh_component->CreateMeshSection(current_sub_mesh_cnt, verts, face_inds,
			empty_normal, uvs, empty_color, empty_tangent, false);
		this->pro_mesh_component->SetMaterial(current_sub_mesh_cnt, dyn_mat_pro);
		current_sub_mesh_cnt++;
	}
};
void ANerfRegionActor::set_need_updata_pro_mesh(bool flag) {
	if (flag == this->need_updata_pro_mesh&&flag==false)
		return;

	this->current_sub_mesh_cnt = 0;
	this->need_updata_pro_mesh = flag;
	this->mesh_component->SetVisibility(true);
	this->pro_mesh_component->SetVisibility(false);

	this->pro_mesh_component->EmptyOverrideMaterials();
	this->pro_mesh_component->ClearAllMeshSections();
	dyn_mat_pro->ClearParameterValues();

	if (flag == false) {
//		this->pro_mesh_component->EmptyOverrideMaterials();
//		this->pro_mesh_component->ClearAllMeshSections();
//		dyn_mat_pro->ClearParameterValues();
	}
	else {

		for (int j = 0; j < current_meshs[0]->feature_maps.Num(); j++) {
			FString f_name = "feature";
			f_name.AppendInt(j);
			dyn_mat_pro->SetTextureParameterValue(FName(*f_name), current_meshs[0]->feature_maps[j]);
		}

		for (int j = 0; j < current_meshs[0]->params_maps.Num(); j++) {
			FString f_name = "params";
			f_name.AppendInt(j);
			dyn_mat_pro->SetTextureParameterValue(FName(*f_name), current_meshs[0]->params_maps[j]);
		}

		if (current_meshs[0]->feature_maps.Num() == 3) {
			dyn_mat_pro->SetScalarParameterValue(FName("mode_switch"), 1);
		}
		else {
			dyn_mat_pro->SetScalarParameterValue(FName("mode_switch"), 0);

		}

		FString f_name = "thres";
		dyn_mat_pro->SetTextureParameterValue(FName(*f_name), current_meshs[0]->thres_map);

//		this->pro_mesh_component->SetVisibility(true);
//		this->mesh_component->SetVisibility(false);

	}
};


void ANerfRegionActor::updata_mip() {
};
