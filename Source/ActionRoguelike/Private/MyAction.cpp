// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAction.h"
#include "MyActionComponent.h"

void UMyAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	
	// ��GrantsTags��ӵ�ӵ��UMyActionʵ���Ľ�ɫ��
	UMyActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	// ��bIsRunning����Ϊtrue����ʾUMyAction��������
	bIsRunning = true;
}


void UMyAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	
	ensureAlways(bIsRunning);
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
    // ��ȡ�ö������ڵ���������
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
        // �����������Ϊ UActorComponent ���ͣ��ͷ��������ڵ������ָ��
		return Comp->GetWorld();
	}
    // ����޷���ȡ������������������ָ�룬�򷵻� nullptr
	return nullptr;
}
//��UMyActionʵ���п���ͨ���ú�����ȡ��������UMyActionComponentʵ��
UMyActionComponent* UMyAction::GetOwningComponent() const
{
	return Cast<UMyActionComponent>(GetOuter());
}

bool UMyAction::IsRunning() const
{
	return bIsRunning;
}
