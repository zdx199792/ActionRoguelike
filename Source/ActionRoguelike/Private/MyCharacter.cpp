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

	//�������ɱ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	//ʹ��Pawn(ControllerRotation)���Ƶ��ɱ���ת
	SpringArmComp->bUsePawnControlRotation = true;
	//�����ɱ�������ӵ��������
	SpringArmComp->SetupAttachment(RootComponent);

	//������������
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	//�������������ӵ����ɱ������
	CameraComp->SetupAttachment(SpringArmComp);
	
	//��תActor������ٷ���(falseΪ����ת)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//bUseControllerRotationYaw��ʾ�����ActorRotation����ControllerRotation��ת
	bUseControllerRotationYaw = false;

	//�������
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	//�������
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

	//��ǰ���ƶ��¼��봦����
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	//�������ƶ��¼��봦����
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	//���ƶ�����¼��봦����
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &AMyCharacter::PrimaryAttack);
	//����Ծ�¼��ʹ�����(Character���ж�ӦJump����)
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
	//�����������������ת
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
	//����������(Y����������)
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

	//Ѫ��Ϊ0ʱ����ֹ�������
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