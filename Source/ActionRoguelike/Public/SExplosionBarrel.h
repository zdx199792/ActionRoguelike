// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosionBarrel.generated.h"

class URadialForceComponent;
class UStaticMeshComponent;
UCLASS()
class ACTIONROGUELIKE_API ASExplosionBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosionBarrel();

protected:
	//��̬������
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	//��ը������
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;
	
	//������ײ������
	UFUNCTION(BlueprintCallable)
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
