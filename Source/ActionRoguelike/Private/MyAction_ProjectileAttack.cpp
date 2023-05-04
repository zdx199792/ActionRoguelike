// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UMyAction_ProjectileAttack::UMyAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.5f;
}

//实现攻击能力的逻辑
void UMyAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	// 如果 Instigator 是 ACharacter 类型，则进行攻击
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		//播放攻击动画
		Character->PlayAnimMontage(AttackAnim);
		// 在手部 socket 上播放粒子特效
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		// 创建一个定时器，在攻击动画延迟之后调用 AttackDelay_Elapsed 函数
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

//在攻击动画延迟之后调用，生成弹道攻击的 Projectile 对象
void UMyAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		// 获取手部 socket 的位置
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		// 设置生成 Projectile 对象的参数
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		// 设置碰撞形状
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		// 设置碰撞查询参数
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		// 设置碰撞对象查询参数
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// 获取 TraceStart 和 TraceEnd，用于进行碰撞检测
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);
		// 进行碰撞检测，如果发生碰撞，则将 TraceEnd 设置为碰撞点
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		// 根据 TraceEnd 和 HandLocation 计算生成 Projectile 对象的旋转
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		// 设置生成 Projectile 对象的位置和旋转
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		// 生成 Projectile 对象
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	// 停止攻击
	StopAction(InstigatorCharacter);
}