// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayFunctionLibrary.h"
#include "MyAttributeComponent.h"

bool UMyGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
    UMyAttributeComponent* AttributeComp = UMyAttributeComponent::GetAttributes(TargetActor);
    if (AttributeComp)
    {
        // 对目标角色应用伤害，调用 UMyAttributeComponent 的 ApplyHealthChange 函数
        return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
    }
    return false;
}
bool UMyGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
    // 如果成功应用伤害
    if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
    {
        // 获取受到伤害的组件
        UPrimitiveComponent* HitComp = HitResult.GetComponent();
        // 如果组件存在并且在命中的骨骼上模拟物理效果
        if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
        {
            // 在受到伤害的位置添加一个反方向的冲击力
            HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
        }
        // 返回 true，表示成功应用方向性伤害
        return true;
    }
    // 如果未成功应用伤害，则返回 false
    return false;
}