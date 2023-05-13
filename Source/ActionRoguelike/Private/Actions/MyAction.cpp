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
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);	
	// ��GrantsTags��ӵ�ӵ��UMyActionʵ���Ľ�ɫ��
	UMyActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	// ��bIsRunning����Ϊtrue����ʾUMyAction��������
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	// �����ǰ���������������ӵ���ߴ��ڷ�������ɫ��ROLE_Authority��
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		// ��¼��ǰ����ʱ����Ϊ������ʼ��ʱ��
		TimeStarted = GetWorld()->TimeSeconds;
	}
	// ��������ʼʱ���㲥OnActionStarted�¼�
	// GetOwningComponent()��ȡ��ǰ���������������this����ǰ����ʵ��
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void UMyAction::StopAction_Implementation(AActor* Instigator)
{
	/*UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));*/
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	//ensureAlways(bIsRunning);
	// ��ӵ��UMyActionʵ���Ľ�ɫ���Ƴ�GrantsTags
	UMyActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	// ��bIsRunning����Ϊfalse����ʾUMyAction��ֹͣ����
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	// ������ֹͣʱ���㲥OnActionStopped�¼�
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
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
	return RepData.bIsRunning;
}
//����������֪RepData���Ա仯��Ļص�����������RepData.bIsRunning�������Ƿ�ִ�и�MyAction����
void UMyAction::OnRep_RepData()
{    
	//���RepData.bIsRunningΪ�棬�����ø�MyAction��StartAction�Ӷ����±��ؿͻ������MyAction�Ķ���
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UMyAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyAction, RepData);
	DOREPLIFETIME(UMyAction, ActionComp);
	DOREPLIFETIME(UMyAction, TimeStarted);
}