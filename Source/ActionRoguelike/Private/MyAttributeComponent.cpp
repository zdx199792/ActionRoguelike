// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeComponent.h"

// Sets default values for this component's properties
UMyAttributeComponent::UMyAttributeComponent()
{
	Health = 100;
}

bool UMyAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	//
	OnHealthChange.Broadcast(nullptr,this, Health,Delta);
	return true;
}

