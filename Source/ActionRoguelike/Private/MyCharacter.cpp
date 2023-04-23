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
	
	//���Ź�������
	PlayAnimMontage(AttackAnim);

	//���ö�ʱ�����ӳ����ɹ�����Ч��0.2s�����PrimaryAttack_TimeElapsed()
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AMyCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	
	//���ٶ�ʱ��������ɫ̧�ֹ���ʱ��������ʱ�����٣�����ִ��PrimaryAttack_TimeElapsed()
	//GetWorldTimerManager.ClearTimer(TimerHandle_PrimaryAttack);
}

void AMyCharacter::PickUp()
{
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}

//���ɹ�����Ч
void AMyCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(Projectileclass);
}

void AMyCharacter::Dash()
{
	//���Ź�������
	PlayAnimMontage(AttackAnim);

	//���ö�ʱ�����ӳ����ɹ�����Ч��0.2s�����PrimaryAttack_TimeElapsed()
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AMyCharacter::Dash_TimeElapsed, 0.2f);
}

void AMyCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileclass);
}

void AMyCharacter::BlackHoleAttack()
{
	//���Ź�������
	PlayAnimMontage(AttackAnim);

	//���ö�ʱ�����ӳ����ɹ�����Ч��0.2s�����PrimaryAttack_TimeElapsed()
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

//����Ͷ����
void AMyCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn)) {
		//��ȡ�������ֲ��Ĳ����ΪͶ����������ɵ�
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		//��������
		FActorSpawnParameters SpawnParams;
		//����Ϊ���������ɣ������ڽ�ɫ�ֲ�������ɣ����ܻ����ɫ������ײ
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//��������ķ�����Ϊ�������
		SpawnParams.Instigator = this;
		//������ײ���
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		//��ײ��ѯ�����������õ�ǰ��ɫ����this����
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		//��ײ�����ѯ�������������Ҫ��ѯ�Ķ�������
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		//ͨ�� CameraComp������������ȡ���λ����Ϊ������㣺TraceStart
		FVector TraceStart = CameraComp->GetComponentLocation();
		//ͨ����ɫ�Ŀ�����ת��ȡ���߷��򣬲����Ծ�������5000���õ������յ㣺TraceEnd��
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		//�������߼�⣬�������ײ�������TraceEndΪ��ײ���λ��
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		//��ȡ�������ѡ��Ƕ�
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		//���������������λ�ú���ת�ǶȻ��������任����
		FTransform spawnTM = FTransform(ProjRotation, HandLocation);

		//����ָ����ClassToSpawn��������󣬲��������ɵ�λ�á���ת(spawnTM)�����ɲ��� SpawnParams ��������
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, spawnTM, SpawnParams);
	}
}

void AMyCharacter::OnHealthChanged(AActor* InstigatorActor, UMyAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	//Ѫ��Ϊ0ʱ����ֹ�������
	if(NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* MyController = Cast<APlayerController>(GetController());
		DisableInput(MyController);
	}
}
