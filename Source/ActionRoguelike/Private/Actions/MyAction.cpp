// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/MyAction.h"
#include "Actions/MyActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void UMyAction::Initialize(UMyActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void UMyAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);	
	// ��GrantsTags��ӵ�ӵ��UMyActionʵ���Ľ�ɫ��
	UMyActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	// ��bIsRunning����Ϊtrue����ʾUMyAction��������
	bIsRunning = true;
}


void UMyAction::StopAction_Implementation(AActor* Instigator)
{
	/*UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));*/
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	//ensureAlways(bIsRunning);
	// ��ӵ��UMyActionʵ���Ľ�ɫ���Ƴ�GrantsTags
	UMyActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	// ��bIsRunning����Ϊfalse����ʾUMyAction��ֹͣ����
	bIsRunning = false;
}
//���ڼ��UMyAction�Ƿ���Կ�ʼ�����UMyAction�������л�ӵ��UMyActionʵ���Ľ�ɫ�б���ֹ�����ı�ǩ���򷵻�false�����򷵻�true��
bool UMyAction::CanStart_Implementation(AActor* Instigator)
{
	// ���UMyAction�������У�����false�����ڹ���ʱ�����ٴι�����
	if (IsRunning())
	{
		return false;
	}

	UMyActionComponent* Comp = GetOwningComponent();
	// ���ӵ��UMyActionʵ���Ľ�ɫ���κ�һ��BlockedTags���򷵻�false
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	// ���UMyAction��������״̬��ӵ��UMyActionʵ���Ľ�ɫû��BlockedTags���򷵻�true
	return true;
}
//���ڻ�ȡUMyActionʵ�����ڵ����磬ͨ����ѯ�����ڵ�����������ʵ��
UWorld* UMyAction::GetWorld() const
{	
    // ��ȡ��Action���ڵ���������
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
        // �����������Ϊ AActor ���ͣ����������ڵ������ָ��
		return Actor->GetWorld();
	}
    // ����޷���ȡ������������������ָ�룬�򷵻� nullptr
	return nullptr;
}
//��UMyActionʵ���п���ͨ���ú�����ȡ��������UMyActionComponentʵ��
UMyActionComponent* UMyAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(UMyActionComponent::StaticClass());
	return ActionComp;
}

bool UMyAction::IsRunning() const
{
	return bIsRunning;
}
//ϣ���������ܹ��������еĿͻ���
void UMyAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void UMyAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyAction, bIsRunning);
	DOREPLIFETIME(UMyAction, ActionComp);
}