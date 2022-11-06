// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void AMyCharacter::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(),HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(Projectileclass ,SpawnTM, SpawnParams);
}
