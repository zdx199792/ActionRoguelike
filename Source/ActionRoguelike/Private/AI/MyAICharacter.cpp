// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAICharacter.h"

// Sets default values
AMyAICharacter::AMyAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
