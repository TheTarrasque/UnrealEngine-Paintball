// Fill out your copyright notice in the Description page of Project Settings.

#include "NXL_Player.h"




void ANXL_Player::BeginPlay()
{
	Super::BeginPlay();
	FoW = GetWorld()->SpawnActor<AFog>(AFog::StaticClass());


}