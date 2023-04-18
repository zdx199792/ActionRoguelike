// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTargetDummy.h"
#include "MyAttributeComponent.h"

// Sets default values
AMyTargetDummy::AMyTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UMyAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChange.AddDynamic(this, &AMyTargetDummy::OnHealthChanged);
}


void AMyTargetDummy::OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f) 
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
