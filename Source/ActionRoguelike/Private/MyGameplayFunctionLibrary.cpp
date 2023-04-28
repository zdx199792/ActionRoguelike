// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayFunctionLibrary.h"
#include "MyAttributeComponent.h"

bool UMyGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
    UMyAttributeComponent* AttributeComp = UMyAttributeComponent::GetAttributes(TargetActor);
    if (AttributeComp)
    {
        // ��Ŀ���ɫӦ���˺������� UMyAttributeComponent �� ApplyHealthChange ����
        return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
    }
    return false;
}
bool UMyGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
    // ����ɹ�Ӧ���˺�
    if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
    {
        // ��ȡ�ܵ��˺������
        UPrimitiveComponent* HitComp = HitResult.GetComponent();
        // ���������ڲ��������еĹ�����ģ������Ч��
        if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
        {
            // ���ܵ��˺���λ�����һ��������ĳ����
            HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
        }
        // ���� true����ʾ�ɹ�Ӧ�÷������˺�
        return true;
    }
    // ���δ�ɹ�Ӧ���˺����򷵻� false
    return false;
}