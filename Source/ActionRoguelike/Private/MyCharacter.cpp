// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "MyAttributeComponent.h"

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
    //获取骨骼体手部的插槽作为魔法攻击类的生成点
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
    //生成朝向：控制器当前的旋转量 生成位置：手部插槽
	FTransform SpawnTM = FTransform(GetControlRotation(),HandLocation);
    
    //参数设置
	FActorSpawnParameters SpawnParams;
    //设置为：总是生成；粒子在角色手部插槽生成，可能会与角色发生碰撞
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//设置抛体的发起者为玩家自身
	SpawnParams.Instigator = this;
    //从World中的SpawnTM按SpawnParams设置生成Projectileclass类
	GetWorld()->SpawnActor<AActor>(Projectileclass ,SpawnTM, SpawnParams);
}
