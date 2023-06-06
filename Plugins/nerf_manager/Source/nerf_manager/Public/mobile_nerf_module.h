#ifndef __NERF_MODULE__
#define __NERF_MODULE__

#ifdef _WIN32
#define CLASS_EXPORT _declspec(dllexport)
#define FUNC_EXPORT
#define GLOBAL_FUNC_EXPORT _declspec(dllexport)


#elif defined(__linux__)
#define CLASS_EXPORT
#define FUNC_EXPORT __attribute__((visibility("default")))
#define GLOBAL_FUNC_EXPORT __attribute__((visibility("default")))

#else
#error "Unknown platform"

#endif

class CLASS_EXPORT NerfManager {
protected:

	NerfManager();

public:
	FUNC_EXPORT virtual ~NerfManager();
	FUNC_EXPORT virtual bool start() = 0;
	FUNC_EXPORT virtual void tick() = 0;

	FUNC_EXPORT virtual bool set_gpu_ids(uint32_t num, uint32_t* gpu_ids) = 0;

	FUNC_EXPORT virtual bool set_scene_path(char* config_path) = 0;
	FUNC_EXPORT virtual bool set_model_folder_path(char* model_path) = 0;
	FUNC_EXPORT virtual bool set_div_exp_factor(uint32_t div_exp_factor) = 0;
	FUNC_EXPORT virtual bool set_compress_mode(int mode) = 0;

	FUNC_EXPORT virtual uint32_t get_model_num() = 0;
	FUNC_EXPORT virtual uint32_t get_subregion_num(uint32_t model_idx) = 0;
	FUNC_EXPORT virtual uint32_t get_subregion_mip_num(uint32_t model_idx, uint32_t region_idx) = 0;

	FUNC_EXPORT virtual bool get_subregion_mip_mesh_cfg(uint32_t model_idx, uint32_t region_idx, uint32_t mip, uint32_t* result, float* center) = 0;
	FUNC_EXPORT virtual bool get_subregion_mip_mesh(uint32_t model_idx, uint32_t region_idx, uint32_t mip, float* verts, uint32_t* face_inds, float* uvs) = 0;

	FUNC_EXPORT virtual uint32_t get_model_feature_nchannel(uint32_t model_idx) = 0;

	FUNC_EXPORT virtual uint32_t get_model_param_nlayers(uint32_t model_idx) = 0;
	FUNC_EXPORT virtual bool get_model_param_map_shape(uint32_t model_idx, uint32_t* res) = 0;
	FUNC_EXPORT virtual bool get_model_param_map(uint32_t model_idx, float** result) = 0;


	FUNC_EXPORT virtual bool get_model_channel_thres_shape(uint32_t model_idx, uint32_t region_idx, uint32_t mip, uint32_t* res) = 0;
	FUNC_EXPORT virtual bool get_model_channel_thres(uint32_t model_idx, uint32_t region_idx, uint32_t mip, float* result) = 0;

	FUNC_EXPORT virtual int check_feature_map_state(uint32_t model_idx, uint32_t region_idx, uint32_t mip_idx) = 0;
	FUNC_EXPORT virtual bool get_subregion_mip_feature_map_size(uint32_t model_idx, uint32_t region_idx, uint32_t mip_idx, uint32_t* result) = 0;
	//	FUNC_EXPORT virtual bool get_subregion_mip_feature_map_async(uint8_t* result_buffer, uint32_t model_idx, uint32_t region_idx, uint32_t mip_idx) = 0;
	FUNC_EXPORT virtual bool get_subregion_mip_feature_map_sync(uint8_t* result_buffer, uint32_t model_idx, uint32_t region_idx, uint32_t mip_idx) = 0;
	FUNC_EXPORT virtual bool get_subregion_mip_feature_map_sync_permap(uint8_t** result_buffer, uint32_t n_res, uint32_t* res_buffer_size, uint32_t model_idx, uint32_t region_idx, uint32_t mip_idx)=0;

	FUNC_EXPORT virtual bool append_infer_regions(uint32_t model_idx, uint32_t region_idx, uint32_t mip_idx, float score) = 0;
	FUNC_EXPORT virtual bool cancle_infer_regions(uint32_t model_idx, uint32_t region_idx, uint32_t mip_idx) = 0;
	FUNC_EXPORT virtual bool update_infer_regions() = 0;
	FUNC_EXPORT virtual bool get_aabb_box(uint32_t model_idx, uint32_t region_idx, float* res) = 0;

};

extern "C" GLOBAL_FUNC_EXPORT NerfManager * get_Manager();

#endif
