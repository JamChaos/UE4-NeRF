
#ifdef _WIN32
#pragma comment (lib,"ue4_plug_v3.lib")
#endif 

#include "nerf_types.h"
#include "NerfCameraActor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "NerfManagerActor.h"
//#include <Developer/MeshBuilderCommon/Public/ThirdPartyBuildOptimizationHelper.h>
ANerfManagerActor* root_save = nullptr;

//BuildOptimizationThirdParty::CVarTriangleOrderOptimization 
void free_global_root() {
	root_save = nullptr;
};

FNerfMesh::FNerfMesh() {

};
FNerfMesh::FNerfMesh(FNerfMesh& other) {
	this->center = other.center;
	this->feature_maps = other.feature_maps;
	this->has_maps = other.has_maps;
	this->has_mesh = other.has_mesh;
	this->is_loading = other.is_loading;
	this->mesh = other.mesh;
	this->mip_id = other.mip_id;
	this->model_id = other.model_id;
	this->params_maps = other.params_maps;
	this->region_id = region_id;
	this->thres_map = thres_map;
};

FNerfMesh::~FNerfMesh() {
	free_mesh();
	free_feature_maps();
	for (int i = 0; i < params_maps.Num(); i++) {
		params_maps[i] = nullptr;
//		delete params_maps[i];
	}
	params_maps.Empty();
	mesh.Empty();
};

void to_uncompact(TArray<FVector>& vert, TArray<uint32>& face_inds) {
	TArray<FVector> temp_vert = vert;
	TArray<uint32> temp_face_inds = face_inds;
	vert.SetNum(temp_face_inds.Num());


	for (int i = 0; i < temp_face_inds.Num(); i++) {
		face_inds[i] = i;
		vert[i] = temp_vert[temp_face_inds[i]];
	}
}

void FNerfMesh::get_uncompact(int start_face_ind, int end_face_ind, TArray<FVector>& vert, TArray<int>& face_inds, TArray<FVector2D>& uvs) {
	vert.SetNum(end_face_ind - start_face_ind);
	face_inds.SetNum(end_face_ind - start_face_ind);
	uvs.SetNum(end_face_ind - start_face_ind);
	for (int i = start_face_ind,j=0; i < end_face_ind; i++,j++) {
		face_inds[j] = j;
		vert[j] = mesh.VertexPositions[mesh.WedgeIndices[i]];
		uvs[j] = mesh.WedgeTexCoords[0][i];
	}

}
void FNerfMesh::get_uncompact(int start_face_ind, int end_face_ind, TArray<FVector>& vert, TArray<uint32_t>& face_inds, TArray<FVector2D>& uvs) {
	vert.SetNum(end_face_ind - start_face_ind);
	face_inds.SetNum(end_face_ind - start_face_ind);
	uvs.SetNum(end_face_ind - start_face_ind);
	for (int i = start_face_ind, j = 0; i < end_face_ind; i++, j++) {
		face_inds[j] = j;
		vert[j] = mesh.VertexPositions[mesh.WedgeIndices[i]];
		uvs[j] = mesh.WedgeTexCoords[0][j];
	}

}

bool FNerfMesh::mesh_from_nerf(NerfManager* manager) {
	if (has_mesh) {
		return true;
	}
	if (is_loading) {
		return false;
	}
	uint32_t param_nlayers=0;
	if (read_lock.try_lock()) {
		if (is_run_async1) {
			read_lock.unlock();
			return false;
			//		std::this_thread::sleep_for(std::chrono::microseconds(50));
		}

	}
	else
		return false;

	is_loading = true;
	is_run_async1 = true;
	read_lock.unlock();

	if (params_maps.Num() <= 0) {

		param_nlayers = manager->get_model_param_nlayers(model_id);
//		this->params_maps.resize(param_nlayers);
		uint32_t* shape=new uint32_t[param_nlayers*2];
		bool res_tp=manager->get_model_param_map_shape(model_id, shape);
		if (res_tp == false) {
			read_lock.lock();
			has_mesh = false;
			is_run_async1 = false;
			is_loading = false;
			read_lock.unlock();
			return false;
		}
		for (uint32_t i = 0; i < param_nlayers; i++) {
			FString f_name = "pm_tx_";
			f_name.AppendInt(model_id);
			f_name += "_";
			f_name.AppendInt(region_id);
			f_name += "_";
			f_name.AppendInt(mip_id);
			f_name += "_";
			f_name.AppendInt(i);

			UTexture2D* MyScreenshot = UTexture2D::CreateTransient(shape[i*2] / 4, shape[i*2+1], PF_A32B32G32R32F, *f_name);
			params_maps.Add( MyScreenshot);
			root_save->textures.Add(MyScreenshot);
			// 锁住他的数据，以便修改

		}

	}
	bool need_thres = false;
	if (thres_map == nullptr) {
		uint32_t shape[2];
		bool res_tp = manager->get_model_channel_thres_shape(model_id,region_id,mip_id, shape);
		if (res_tp == false) {
			read_lock.lock();
			has_mesh = false;
			is_run_async1 = false;
			is_loading = false;
			read_lock.unlock();
			return false;
		}

		FString f_name = "th_tx_";
		f_name.AppendInt(model_id);
		f_name += "_";
		f_name.AppendInt(region_id);
		f_name += "_";
		f_name.AppendInt(mip_id);

		thres_map = UTexture2D::CreateTransient(shape[1] / 4, shape[0], PF_A32B32G32R32F,*f_name);
		root_save->textures.Add(thres_map);
		if(thres_map!=nullptr)
			need_thres = true;
		else {
			int a = 1;
		}

	}
	
//	AsyncTask(ENamedThreads::AnyThread, [this,manager, param_nlayers, need_thres]() {
		if (param_nlayers > 0) {
			std::vector<float*> buffer_ptr;
			for (uint32_t i = 0; i < param_nlayers; i++) {
				UTexture2D* MyScreenshot = params_maps[i];
				float* MipData = (float*)(MyScreenshot->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
				buffer_ptr.push_back(MipData);
			}
			bool res_tp = manager->get_model_param_map(model_id, buffer_ptr.data());
			if (res_tp == false) {
				for (uint32_t i = 0; i < param_nlayers; i++) {
					UTexture2D* MyScreenshot = params_maps[i];
					MyScreenshot->PlatformData->Mips[0].BulkData.Unlock();
				}
				read_lock.lock();
				has_mesh = false;
				is_run_async1 = false;
				is_loading = false;
				read_lock.unlock();
				return false;
			}

			for (uint32_t i = 0; i < param_nlayers; i++) {
				UTexture2D* MyScreenshot = params_maps[i];
				MyScreenshot->PlatformData->Mips[0].BulkData.Unlock();
				//			AsyncTask(ENamedThreads::GameThread, [&MyScreenshot]() {
				MyScreenshot->SRGB = 0;
				MyScreenshot->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
				MyScreenshot->Filter = TextureFilter::TF_Nearest;
				MyScreenshot->UpdateResource();

				//			});
			}
			buffer_ptr.clear();

		}
		if (need_thres) {
			float* thres_MipData = nullptr;

			thres_MipData = (float*)(thres_map->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
			bool res_tp=manager->get_model_channel_thres(model_id, region_id, mip_id, thres_MipData);
			if (res_tp == false) {
				thres_map->PlatformData->Mips[0].BulkData.Unlock();
				thres_map = nullptr;
				read_lock.lock();
				has_mesh = false;
				is_run_async1 = false;
				is_loading = false;
				read_lock.unlock();
				return false;
			}

			thres_map->PlatformData->Mips[0].BulkData.Unlock();
			thres_map->SRGB = 0;
			thres_map->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
			thres_map->Filter = TextureFilter::TF_Nearest;
			thres_map->UpdateResource();

		}
		uint32_t res[3];
		float center_temp[3];
		bool res_tp =manager->get_subregion_mip_mesh_cfg(model_id, region_id, mip_id, res, center_temp);
		if (res_tp == false) {
			read_lock.lock();
			has_mesh = false;
			is_run_async1 = false;
			is_loading = false;
			read_lock.unlock();
			return false;
		}
		this->center = FVector(center_temp[0], center_temp[1], center_temp[2])*100.0f;
		mesh.WedgeIndices.SetNum(res[1]);
		mesh.WedgeTexCoords[0].SetNum(res[2] / 2);
		mesh.FaceMaterialIndices.SetNum(res[1] / 3);
		mesh.FaceSmoothingMasks.SetNum(res[1] / 3);

		mesh.VertexPositions.SetNum(res[0] / 3);

		res_tp = manager->get_subregion_mip_mesh(model_id, region_id, mip_id,
			(float*)mesh.VertexPositions.GetData(),
			mesh.WedgeIndices.GetData(), (float*)mesh.WedgeTexCoords[0].GetData());

		if (res_tp == false) {
			read_lock.lock();
			has_mesh = false;
			is_run_async1 = false;
			is_loading = false;
			read_lock.unlock();
			return false;
		}
		AsyncTask(ENamedThreads::AnyThread, [this,manager]() {

		for (int i = 0; i < mesh.VertexPositions.Num(); i++) {
			mesh.VertexPositions[i]*=100.0f;
		}

		mesh.CompactMaterialIndices();

		read_lock.lock();
		has_mesh = true;
		is_run_async1 = false;
		is_loading = false;

		read_lock.unlock();

		});

		return true;

};
bool FNerfMesh::feature_maps_from_nerf(NerfManager* manager) {

	if (has_maps) {
		return true;
	}
	if (read_lock.try_lock()) {
		if (is_run_async2) {
			read_lock.unlock();
			return false;
			//		std::this_thread::sleep_for(std::chrono::microseconds(50));
		}

	}
	else
		return false;
//	read_lock.lock();
	is_run_async2 = true;
	read_lock.unlock();

	uint32_t n_channel = manager->get_model_feature_nchannel(model_id);
	uint32_t res[3];
	bool res_tp=manager->get_subregion_mip_feature_map_size(model_id, region_id, mip_id, res);
	if (res_tp == false) {
		read_lock.lock();
		is_run_async2 = false;
		has_maps = false;
		read_lock.unlock();
		return false;

	}
	int w = res[0];
	int h = res[1];
	int size = res[2];

	for (int i = 0; i < feature_maps.Num(); i++) {
		root_save->textures.Remove(feature_maps[i]);
		feature_maps[i] = nullptr;
	}
	feature_maps.Empty();

	uint32_t model_id_t = model_id;
	uint32_t region_id_t = region_id;
	uint32_t mip_id_t = mip_id;

	if (root_save->compress_mode == 3) {
		for (uint32_t i = 0; i < n_channel; i++) {
			FString f_name = "f_tx_";
			f_name.AppendInt(model_id);
			f_name += "_";
			f_name.AppendInt(region_id);
			f_name += "_";
			f_name.AppendInt(mip_id);
			f_name += "_";
			f_name.AppendInt(i);


			UTexture2D* MyScreenshot =  UTexture2D::CreateTransient(w, h, PF_BC4, *f_name);

			this->feature_maps.Add(MyScreenshot);
			root_save->textures.Add(MyScreenshot);



		}

	}

	else {
		for (uint32_t i = 0; i < n_channel / 3; i++) {
			FString f_name = "f_tx_";
			f_name.AppendInt(model_id);
			f_name += "_";
			f_name.AppendInt(region_id);
			f_name += "_";
			f_name.AppendInt(mip_id);
			f_name += "_";
			f_name.AppendInt(i);


			UTexture2D* MyScreenshot = nullptr;
			if (root_save->compress_mode == 0) {
				MyScreenshot = UTexture2D::CreateTransient(w, h, PF_R8G8B8A8, *f_name);

			}
			else if (root_save->compress_mode == 1) {
				MyScreenshot = UTexture2D::CreateTransient(w, h, PF_DXT1, *f_name);

			}
			else if (root_save->compress_mode == 2) {
				MyScreenshot = UTexture2D::CreateTransient(w, h, PF_BC7, *f_name);

			}
			else {
				MyScreenshot = UTexture2D::CreateTransient(w, h, PF_R8G8B8A8, *f_name);

			}
			this->feature_maps.Add(MyScreenshot);
			root_save->textures.Add(MyScreenshot);



		}

	}

//	AsyncTask(ENamedThreads::AnyThread, [this, manager, size, model_id_t, region_id_t, mip_id_t]() {

	std::vector<uint8*> output_buffer;
	std::vector<uint32_t> output_buffer_sizes;
	for (int i = 0; i < this->feature_maps.Num(); i++) {
		UTexture2D* MyScreenshot = this->feature_maps[i];

		uint8* MipData = (uint8*)(MyScreenshot->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
		output_buffer.push_back(MipData);
		output_buffer_sizes.push_back(MyScreenshot->PlatformData->Mips[0].BulkData.GetBulkDataSize());
	}
	res_tp = manager->get_subregion_mip_feature_map_sync_permap(output_buffer.data(), output_buffer.size(), output_buffer_sizes.data(), model_id_t, region_id_t, mip_id_t);

	
//	std::vector<uint8_t> output_buffer;
//		output_buffer.resize(size);
//		res_tp=manager->get_subregion_mip_feature_map_sync(output_buffer.data(), model_id_t, region_id_t, mip_id_t);

		if (res_tp == false) {
			for (int i = 0; i < this->feature_maps.Num(); i++) {
				UTexture2D* MyScreenshot = this->feature_maps[i];

				MyScreenshot->PlatformData->Mips[0].BulkData.Unlock();
			}
			read_lock.lock();
			is_run_async2 = false;
			has_maps = false;
			read_lock.unlock();
			return false;
		}
//		AsyncTask(ENamedThreads::AnyThread, [this, manager, size, output_buffer]() {
		for (int i = 0; i < this->feature_maps.Num(); i++) {
			UTexture2D* MyScreenshot = this->feature_maps[i];
//			uint8* MipData = static_cast<uint8*>(MyScreenshot->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

//			memcpy(MipData, output_buffer.data() + size / this->feature_maps.Num() * i, size / this->feature_maps.Num() * sizeof(uint8_t));

			MyScreenshot->PlatformData->Mips[0].BulkData.Unlock();
			MyScreenshot->SRGB = 0;
			MyScreenshot->Filter = TextureFilter::TF_Nearest;
			MyScreenshot->UpdateResource();
		}
	//	AsyncTask(ENamedThreads::GameThread, [this, manager, size, output_buffer]() {
//			for (int i = 0; i < this->feature_maps.Num(); i++) {
//				UTexture2D* MyScreenshot = this->feature_maps[i];
//			}
			read_lock.lock();

			is_run_async2 = false;
			has_maps = true;
			read_lock.unlock();

//			});

//		});

//		});
	return true;

//	read_lock.unlock();

};
bool FNerfMesh::check_load_stata() {
	if (is_run_async1||is_run_async2)
		return false;
	return has_maps && has_mesh;
};

void FNerfMesh::free_feature_maps() {
	if (read_lock.try_lock()) {
		if (is_run_async2) {
			//		AsyncTask(ENamedThreads::AnyThread, [this]() {
			//			while (is_run_async2) {
			//				std::this_thread::sleep_for(std::chrono::microseconds(50));
			//			}
			//			is_run_async2 = true;
			//			has_maps = false;
			//			for (int i = 0; i < feature_maps.Num(); i++) {
			//				feature_maps[i] = nullptr;
			//				root_save->textures.Remove(feature_maps[i]);
			//			}
			//			feature_maps.Empty();
			//			is_run_async2 = false;
			//			});
		}
		else {

			has_maps = false;

			for (int i = 0; i < feature_maps.Num(); i++) {
				auto id = root_save->textures.FindId(feature_maps[i]);
				root_save->textures.Remove(feature_maps[i]);
				feature_maps[i] = nullptr;
			}
			feature_maps.Empty();

		}
		read_lock.unlock();

	}

};
void FNerfMesh::free_mesh() {
	if (read_lock.try_lock()) {
		if (is_run_async1) {
			//		AsyncTask(ENamedThreads::AnyThread, [this]() {
			//			while (is_run_async1) {
			//				std::this_thread::sleep_for(std::chrono::microseconds(50));
			//			}
			//			read_lock.lock();
			//			is_run_async1 = true;
			//			has_mesh = false;
			//			mesh.Empty();
			//			is_run_async1 = false;
			//			read_lock.unlock();

			//			});
		}
		else {
			has_mesh = false;
			mesh.Empty();

		}
		read_lock.unlock();
	}
	
};

bool FNerfMesh::need_get_feature_map() {
	if (is_run_async2 == true && has_maps == false)
		return false;
	if (is_run_async2 == false&& has_maps)
		return false;
	return true;
};


std::map<uint32_t, float> FNerfRegion::mip_dis;
uint32_t FNerfRegion::proc_mip;

FVector FNerfRegion::get_center() {
	if (meshs.Num() > 0) {
		for (int i = 0; i < meshs.Num(); i++) {
			if(meshs[i].has_mesh)
				return meshs[i].center;

		}
	}
	return FVector(0,0,1e32);
};

FVector FNerfRegion::get_mesh_center() {
	if (region_box.is_load == false) {
		return get_center();
	}
	else {
		return region_box.get_center();
	}
};


FNerfRegion::FNerfRegion() {
	mip_list.Empty();
	for (auto iter = mip_dis.begin(); iter != mip_dis.end(); iter++) {
		mip_list.Add(iter->first);
	}
	if (mip_list.Num() == 0) {
		mip_list.Add(this->meshs.Num() - 1);
	}
	mip_list.Sort();
//	std::sort(mip_list.begin(), mip_list.end());
	last_mip = mip_list.Last();
	current_mip = mip_list.Last();
};
FNerfRegion::~FNerfRegion() {
//	target = nullptr;
	meshs.Empty();
};
void FNerfRegion::update_mip(NerfManager* manager, bool high_prop) {

	if (region_box.is_load == false) {
		float res[6];
		if (manager->get_aabb_box(model_id, region_id, res)) {
			region_box.min.X = res[0] * 100.0f;
			region_box.min.Y = res[1] * 100.0f;
			region_box.min.Z = res[2] * 100.0f;
			region_box.max.X = res[3] * 100.0f;
			region_box.max.Y = res[4] * 100.0f;
			region_box.max.Z = res[5] * 100.0f;
			region_box.is_load = true;
		}

	}



	TArray<ANerfCameraActor*> temp = ANerfCameraActor::cameras;
	auto primary_camera = UGameplayStatics::GetPlayerCameraManager(root_save->GetWorld(), 0);
	//		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	FVector loc = primary_camera->GetCameraLocation();

	FVector temp_loc = (get_mesh_center() - loc);
	if (temp_loc.Z > 0)
		temp_loc.Z = 0;
	float dis = temp_loc.Size();
	for (int i = 0; i < temp.Num(); i++) {
		temp_loc = (get_mesh_center() - temp[i]->get_loc());
		if (temp_loc.Z > 0)
			temp_loc.Z = 0;
		float temp_dis = temp_loc.Size();

		if (temp_dis < dis)
			dis = temp_dis;
	}

	if (high_prop) {
		current_mip = mip_list[0];
		last_mip = mip_list[0];
		min_dis = dis;
	}
	else {
		min_dis = dis+100000;
		current_mip = mip_list.Last();
		for (int i = 0; i < mip_list.Num(); i++) {
			if (dis < mip_dis[mip_list[i]]) {
				current_mip = mip_list[i];
				break;
			}
		}
		last_mip = mip_list.Last();
		for (int i = 0; i < mip_list.Num(); i++) {
			if (dis < mip_dis[mip_list[i]] * 1.5) {
				last_mip = mip_list[i];
				break;
			}
		}

	}


//	current_mip = std::min(current_mip, last_mip);
};

void FNerfRegion::init(NerfManager* manager) {
	meshs.Empty();
	meshs.SetNum(mip_list.Num());
	for (int i = 0; i < mip_list.Num(); i++) {
		//				meshs.Add(FNerfMesh());
		meshs[i].model_id = model_id;
		meshs[i].region_id = region_id;
		meshs[i].mip_id = mip_list[i];
	}
	uint32_t mip_idx=mip_list.Num()-1;
	meshs[mip_idx].mesh_from_nerf(manager);
	if (region_box.is_load == false) {
		float res[6];
		if (manager->get_aabb_box(model_id, region_id, res)) {
			region_box.min.X = res[0] * 100.0f;
			region_box.min.Y = res[1] * 100.0f;
			region_box.min.Z = res[2] * 100.0f;
			region_box.max.X = res[3] * 100.0f;
			region_box.max.Y = res[4] * 100.0f;
			region_box.max.Z = res[5] * 100.0f;
			region_box.is_load = true;
		}
		
	}
	current_mip = mip_list.Last();
	last_mip= mip_list.Last();
};
TArray<uint32_t> FNerfRegion::get_need_get_mip_idx() {
	TArray<uint32_t> res;
	for (int i = 0; i < mip_list.Num(); i++) {
		if (current_mip <= mip_list[i]) {
			if (meshs[i].need_get_feature_map()) {
				res.Add(i);
			}
		}
	}
	return res;
};
TArray<uint32_t> FNerfRegion::get_need_free_mip_idx() {
	TArray<uint32_t> res;
	for (int i = 0; i < mip_list.Num(); i++) {
		if (current_mip > mip_list[i] && current_mip> last_mip) {
			if (meshs[i].check_load_stata()==false) {
				res.Add(i);
			}

		}
		else {
			if (last_mip > mip_list[i]) {
				res.Add(i);
			}

		}
	}
	return res;
};
TArray<uint32_t> FNerfRegion::get_need_get_mip() {
	TArray<uint32_t> res;
	for (int i = 0; i < mip_list.Num(); i++) {
		if (current_mip <= mip_list[i]) {
			if (meshs[i].need_get_feature_map()) {
				res.Add(mip_list[i]);
			}
		}
	}
	return res;
};
TArray<uint32_t> FNerfRegion::get_need_free_mip() {
	TArray<uint32_t> res;
	for (int i = 0; i < mip_list.Num(); i++) {
		if (last_mip > mip_list[i]) {
			if (meshs[i].has_maps || meshs[i].has_mesh) {
				res.Add(mip_list[i]);
			}
		}
	}
	return res;
};

void FNerfRegion::update(NerfManager* manager,bool high_prop,float dir_cos) {
	update_mip(manager, high_prop);
	float ext_cost = 0;
	if (high_prop) {
		if (dir_cos < 0)
			dir_cos = 0;
		ext_cost = (1 - dir_cos) * 6000;
		min_dis += ext_cost;
	}
//	if (current_mip != last_mip)
//	{
//		std::cout << "" << std::endl;
//	}
	TArray<uint32_t> need_get_mip = get_need_get_mip_idx();

	TArray<uint32_t> need_free_mip = get_need_free_mip_idx();
	for (int i = 0; i < need_get_mip.Num(); i++) {
		manager->append_infer_regions(this->model_id, this->region_id, mip_list[need_get_mip[i]], min_dis);
	}
	for (int i = 0; i < need_free_mip.Num(); i++) {
		manager->cancle_infer_regions(this->model_id, this->region_id, mip_list[need_free_mip[i]]);
		meshs[need_free_mip[i]].free_feature_maps();
	}
	for (int i = 0; i < need_get_mip.Num(); i++) {
		int state = manager->check_feature_map_state(this->model_id, this->region_id, mip_list[need_get_mip[i]]);
		if (state > 0) {
			meshs[need_get_mip[i]].feature_maps_from_nerf(manager);
			meshs[need_get_mip[i]].mesh_from_nerf(manager);
		}
		else if (state < 0) {
			manager->get_subregion_mip_feature_map_sync(nullptr, this->model_id, this->region_id, mip_list[need_get_mip[i]]);
		}
	}

}

TArray<uint32_t> FNerfRegion::get_active_mip_idx() {
	TArray<uint32_t> res;
	for (int i = 0; i < mip_list.Num(); i++) {
		if (mip_list[i] >= current_mip &&meshs[i].check_load_stata()) {
			res.Add(i);
		}
	}
	if (res.Num() <= 0 && meshs[mip_list.Num()-1].check_load_stata()) {
		res.Add(mip_list.Num()-1);
	}
	return res;
};

TArray<FNerfMesh*> FNerfRegion::get_active_mip_meshs() {
	TArray<uint32_t> mip_idx = get_active_mip_idx();
	TArray<FNerfMesh*> res;
	for (int i = 0; i < mip_idx.Num(); i++) {
		res.Add(&meshs[mip_idx[i]]);
	}
	return res;
};


uint32_t FNerfRegion::mip_idx_to_mip(uint32_t mip_idx) {
	return mip_list[mip_idx];
};

FNerfModel::FNerfModel() {

};
FNerfModel::~FNerfModel() {
	regions.Empty();
};
void FNerfModel::init(ANerfManagerActor* root, NerfManager* manager) {
	if (root_save == nullptr) {
		root_save = root;
	}
	int sub_reg_num=manager->get_subregion_num(model_id);
	regions.SetNum(sub_reg_num);
	for (int i = 0; i < sub_reg_num; i++) {
//		regions.Add(FNerfRegion());
		regions[i].model_id = model_id;
		regions[i].region_id = i;
		regions[i].init(manager);
	}

};

TArray<FNerfRegion*> FNerfModel::get_regions() {
	TArray<FNerfRegion*> res;
	for (int i = 0; i < regions.Num(); i++) {
		res.Add(&regions[i]);
	}
	return res;
};
