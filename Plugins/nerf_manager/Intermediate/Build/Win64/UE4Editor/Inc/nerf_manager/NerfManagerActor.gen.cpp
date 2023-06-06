// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "nerf_manager/Public/NerfManagerActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNerfManagerActor() {}
// Cross Module References
	NERF_MANAGER_API UClass* Z_Construct_UClass_ANerfManagerActor_NoRegister();
	NERF_MANAGER_API UClass* Z_Construct_UClass_ANerfManagerActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_nerf_manager();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
// End Cross Module References
	void ANerfManagerActor::StaticRegisterNativesANerfManagerActor()
	{
	}
	UClass* Z_Construct_UClass_ANerfManagerActor_NoRegister()
	{
		return ANerfManagerActor::StaticClass();
	}
	struct Z_Construct_UClass_ANerfManagerActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_model_path_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_model_path;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_cfg_path_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_cfg_path;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_mesh_component_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_mesh_component;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_sigh_check_dis_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_sigh_check_dis;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_textures_MetaData[];
#endif
		static const UE4CodeGen_Private::FSetPropertyParams NewProp_textures;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_textures_ElementProp;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ANerfManagerActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_nerf_manager,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANerfManagerActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "NerfManagerActor.h" },
		{ "ModuleRelativePath", "Public/NerfManagerActor.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_model_path_MetaData[] = {
		{ "Category", "cfg" },
		{ "ModuleRelativePath", "Public/NerfManagerActor.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_model_path = { "model_path", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ANerfManagerActor, model_path), METADATA_PARAMS(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_model_path_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_model_path_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_cfg_path_MetaData[] = {
		{ "Category", "cfg" },
		{ "ModuleRelativePath", "Public/NerfManagerActor.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_cfg_path = { "cfg_path", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ANerfManagerActor, cfg_path), METADATA_PARAMS(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_cfg_path_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_cfg_path_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_mesh_component_MetaData[] = {
		{ "Category", "cfg" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/NerfManagerActor.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_mesh_component = { "mesh_component", nullptr, (EPropertyFlags)0x001000000008001d, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ANerfManagerActor, mesh_component), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_mesh_component_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_mesh_component_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_sigh_check_dis_MetaData[] = {
		{ "Category", "cfg" },
		{ "ModuleRelativePath", "Public/NerfManagerActor.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_sigh_check_dis = { "sigh_check_dis", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ANerfManagerActor, sigh_check_dis), METADATA_PARAMS(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_sigh_check_dis_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_sigh_check_dis_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_textures_MetaData[] = {
		{ "ModuleRelativePath", "Public/NerfManagerActor.h" },
	};
#endif
	const UE4CodeGen_Private::FSetPropertyParams Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_textures = { "textures", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Set, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ANerfManagerActor, textures), METADATA_PARAMS(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_textures_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_textures_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_textures_ElementProp = { "textures", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ANerfManagerActor_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_model_path,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_cfg_path,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_mesh_component,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_sigh_check_dis,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_textures,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ANerfManagerActor_Statics::NewProp_textures_ElementProp,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ANerfManagerActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANerfManagerActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ANerfManagerActor_Statics::ClassParams = {
		&ANerfManagerActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ANerfManagerActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ANerfManagerActor_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ANerfManagerActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ANerfManagerActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ANerfManagerActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ANerfManagerActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ANerfManagerActor, 1814896296);
	template<> NERF_MANAGER_API UClass* StaticClass<ANerfManagerActor>()
	{
		return ANerfManagerActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ANerfManagerActor(Z_Construct_UClass_ANerfManagerActor, &ANerfManagerActor::StaticClass, TEXT("/Script/nerf_manager"), TEXT("ANerfManagerActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANerfManagerActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
