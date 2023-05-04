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
#include "MyActionComponent.h"
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

	ActionComp = CreateDefaultSubobject<UMyActionComponent>("ActionComp");

	TimeToHitParamName = "TimeToHit";
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChange.AddDynamic(this, &AMyCharacter::OnHealthChanged);
}
FVector AMyCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::SprintStop);
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
void AMyCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AMyCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AMyCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AMyCharacter::PickUp()
{
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}

void AMyCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void AMyCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "Blackhole");
}


void AMyCharacter::OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}

	//血量为0时，禁止玩家输入
	if(NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* MyController = Cast<APlayerController>(GetController());
		DisableInput(MyController);
	}
}

void AMyCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}