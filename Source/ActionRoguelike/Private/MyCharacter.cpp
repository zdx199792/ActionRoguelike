// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "MyAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建弹簧臂组件
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	//使用Pawn(ControllerRotation)控制弹簧臂旋转
	SpringArmComp->bUsePawnControlRotation = true;
	//将弹簧臂组件附加到根组件上
	SpringArmComp->SetupAttachment(RootComponent);

	//创建摄像机组件
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	//将摄像机组件附加到弹簧臂组件上
	CameraComp->SetupAttachment(SpringArmComp);
	
	//旋转Actor朝向加速方向(false为不旋转)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//bUseControllerRotationYaw表示人物的ActorRotation跟随ControllerRotation旋转
	bUseControllerRotationYaw = false;

	//交互组件
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	//属性组件
	AttributeComp = CreateDefaultSubobject<UMyAttributeComponent>("AttributeComp");


	HandSocketName = "Muzzle_01";
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChange.AddDynamic(this, &AMyCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//绑定前后移动事件与处理函数
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	//绑定左右移动事件与处理函数
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	//绑定移动鼠标事件与处理函数
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &AMyCharacter::PrimaryAttack);
	//绑定跳跃事件和处理函数(Character已有对应Jump函数)
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Pressed, this, &AMyCharacter::PickUp);

	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AMyCharacter::Dash);
	
	PlayerInputComponent->BindAction(TEXT("BlackHoleAttack"), IE_Pressed, this, &AMyCharacter::BlackHoleAttack);


}

void AMyCharacter::MoveForward(float value)
{
	FRotator controlRot = GetControlRotation();
	//忽略其他坐标轴的旋转
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	AddMovementInput(controlRot.Vector(), value);
}

void AMyCharacter::MoveRight(float value)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	/*
	*  X = Forward
	*  Y = Right
	*  Z = Up
	*/
	//返回右向量(Y代表右向量)
	FVector RightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}

void AMyCharacter::PrimaryAttack()
{
	
	//播放攻击动画
	PlayAnimMontage(AttackAnim);

	//设置定时器，延迟生成攻击特效，0.2s后调用PrimaryAttack_TimeElapsed()
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AMyCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	
	//销毁定时器，当角色抬手攻击时死亡，定时器销毁，不会执行PrimaryAttack_TimeElapsed()
	//GetWorldTimerManager.ClearTimer(TimerHandle_PrimaryAttack);
}

void AMyCharacter::PickUp()
{
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}

//生成攻击特效
void AMyCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(Projectileclass);
}

void AMyCharacter::Dash()
{
	//播放攻击动画
	PlayAnimMontage(AttackAnim);

	//设置定时器，延迟生成攻击特效，0.2s后调用PrimaryAttack_TimeElapsed()
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AMyCharacter::Dash_TimeElapsed, 0.2f);
}

void AMyCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileclass);
}

void AMyCharacter::BlackHoleAttack()
{
	//播放攻击动画
	PlayAnimMontage(AttackAnim);

	//设置定时器，延迟生成攻击特效，0.2s后调用PrimaryAttack_TimeElapsed()
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AMyCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void AMyCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileclass);
}

void AMyCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

//发射投射物
void AMyCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn)) {
		//获取骨骼体手部的插槽作为投射物类的生成点
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		//参数设置
		FActorSpawnParameters SpawnParams;
		//设置为：总是生成；粒子在角色手部插槽生成，可能会与角色发生碰撞
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//设置抛体的发起者为玩家自身
		SpawnParams.Instigator = this;
		//用于碰撞检测
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		//碰撞查询参数，并设置当前角色对象this忽略
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		//碰撞对象查询参数，并添加需要查询的对象类型
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		//通过 CameraComp（相机组件）获取相机位置作为射线起点：TraceStart
		FVector TraceStart = CameraComp->GetComponentLocation();
		//通过角色的控制旋转获取射线方向，并乘以距离因子5000，得到射线终点：TraceEnd。
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		//进行射线检测，如果有碰撞，则更新TraceEnd为碰撞点的位置
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		//获取生成物的选择角度
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		//根据生成物的生成位置和旋转角度获得生成物变换矩阵
		FTransform spawnTM = FTransform(ProjRotation, HandLocation);

		//生成指定类ClassToSpawn的抛体对象，并传入生成的位置、旋转(spawnTM)和生成参数 SpawnParams 进行生成
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, spawnTM, SpawnParams);
	}
}

void AMyCharacter::OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	//血量为0时，禁止玩家输入
	if(NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* MyController = Cast<APlayerController>(GetController());
		DisableInput(MyController);
	}
}
