// Fill out your copyright notice in the Description page of Project Settings.


#include "NerfCameraActor.h"

#include <Runtime/Engine/Classes/Camera/CameraComponent.h>

TArray<ANerfCameraActor*> ANerfCameraActor::cameras;
ANerfCameraActor::ANerfCameraActor() {
	PrimaryActorTick.bCanEverTick = true;
	this->PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;
	this->PrimaryActorTick.EndTickGroup = ETickingGroup::TG_PostPhysics;;
	this->GetCameraComponent()->SetRelativeLocation(FVector(0, 0, 0));
	this->GetCameraComponent()->SetRelativeRotation(FRotator(0, 0, 0));

};
//ANerfCameraActor::~ANerfCameraActor() {

//};
void ANerfCameraActor::BeginPlay() {
	Super::BeginPlay();
	ANerfCameraActor::cameras.Add(this);
};
void ANerfCameraActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	this->GetCameraComponent()->SetRelativeLocation(FVector(0, 0, 0));			//保证相机组件内部的部件不会相对于此actor发生平移
	this->GetCameraComponent()->SetRelativeRotation(FRotator(0, 0, 0));			//保证相机组件内部的部件不会相对于此actor发生旋转

};
FVector ANerfCameraActor::get_loc() {
	return this->GetActorLocation();
};
FVector ANerfCameraActor::get_dir() {
	return this->GetActorRotation().Vector();
};
