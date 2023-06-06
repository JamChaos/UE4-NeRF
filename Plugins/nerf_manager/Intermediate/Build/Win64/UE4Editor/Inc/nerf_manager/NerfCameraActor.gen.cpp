// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "nerf_manager/Public/NerfCameraActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNerfCameraActor() {}
// Cross Module References
	NERF_MANAGER_API UClass* Z_Construct_UClass_ANerfCameraActor_NoRegister();
	NERF_MANAGER_API UClass* Z_Construct_UClass_ANerfCameraActor();
	ENGINE_API UClass* Z_Construct_UClass_ACameraActor();
	UPackage* Z_Construct_UPackage__Script_nerf_manager();
// End Cross Module References
	void ANerfCameraActor::StaticRegisterNativesANerfCameraActor()
	{
	}
	UClass* Z_Construct_UClass_ANerfCameraActor_NoRegister()
	{
		return ANerfCameraActor::StaticClass();
	}
	struct Z_Construct_UClass_ANerfCameraActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ANerfCameraActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACameraActor,
		(UObject* (*)())Z_Construct_UPackage__Script_nerf_manager,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANerfCameraActor_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Input Rendering" },
		{ "IncludePath", "NerfCameraActor.h" },
		{ "ModuleRelativePath", "Public/NerfCameraActor.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ANerfCameraActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANerfCameraActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ANerfCameraActor_Statics::ClassParams = {
		&ANerfCameraActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ANerfCameraActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ANerfCameraActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ANerfCameraActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ANerfCameraActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ANerfCameraActor, 295489763);
	template<> NERF_MANAGER_API UClass* StaticClass<ANerfCameraActor>()
	{
		return ANerfCameraActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ANerfCameraActor(Z_Construct_UClass_ANerfCameraActor, &ANerfCameraActor::StaticClass, TEXT("/Script/nerf_manager"), TEXT("ANerfCameraActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANerfCameraActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
