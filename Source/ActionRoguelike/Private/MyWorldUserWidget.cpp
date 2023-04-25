// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

//���û�������ͼ�е� Widget ʵʱ����һ��ָ���� Actor������ Widget ��λ����Ļ�ϵ�Ŀ��λ��
void UMyWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ��� AttachedActor �Ƿ���Ч
	if (!IsValid(AttachedActor))
	{
		// ��� AttachedActor ��Ч������ǰ Widget �Ӹ��ؼ����Ƴ�
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));
		return;
	}

	FVector2D ScreenPosition;
	// �� AttachedActor ����������ͶӰ����Ļ����
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		// ��ȡ��ǰ�ӿڵ����ű���
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		// �������ű�������Ļ�����������
		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			// ����Ļ��������Ϊ ParentSizeBox ����Ⱦƽ��
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}

