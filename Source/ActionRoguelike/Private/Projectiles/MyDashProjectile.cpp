// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/MyDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMyDashProjectile::AMyDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.0f;
}
//实现闪现效果
void AMyDashProjectile::Explode_Implementation()
{
	//清除了之前设置的定时器TimerHandle_DelayedDetonate
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	
	//在当前位置生成粒子效果
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	
	//调用EffectComp->DeactivateSystem()方法停止粒子效果的播放
	EffectComp->DeactivateSystem();
	
	//调用MoveComp->StopMovementImmediately()方法停止移动
	MoveComp->StopMovementImmediately();

	//调用SetActorEnableCollision(false) 函数禁用了碰撞
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	//调用GetWorldTimerManager().SetTimer()设置了一个延迟执行的定时器TimerHandle_DelayedTeleport:
	//在指定的延迟时间后调用了AMyDashProjectile::TeleportInstigator()方法
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AMyDashProjectile::TeleportInstigator, TeleportDelay);
}

//定时器触发时所执行的传送逻辑
void AMyDashProjectile::TeleportInstigator()
{
	//获取了投射物的Instigator（发射者）
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//将一个游戏角色或物体瞬间传送到指定位置
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}

void AMyDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	//设置一个延迟执行的定时器TimerHandle_DelayedDetonate，在指定的延迟时间后调用了AMyDashProjectile::Explode_Implementation()函数，从而触发闪现效果
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &AMyDashProjectile::Explode_Implementation, DetonateDelay);
}
