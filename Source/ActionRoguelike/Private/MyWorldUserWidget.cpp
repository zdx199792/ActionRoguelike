// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

//在用户界面蓝图中的 Widget 实时跟随一个指定的 Actor，并将 Widget 定位到屏幕上的目标位置
void UMyWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 检查 AttachedActor 是否有效
	if (!IsValid(AttachedActor))
	{
		// 如果 AttachedActor 无效，将当前 Widget 从父控件中移除
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));
		return;
	}

	FVector2D ScreenPosition;
	// 将 AttachedActor 的世界坐标投影到屏幕坐标
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		// 获取当前视口的缩放比例
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		// 根据缩放比例对屏幕坐标进行缩放
		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			// 将屏幕坐标设置为 ParentSizeBox 的渲染平移
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}

