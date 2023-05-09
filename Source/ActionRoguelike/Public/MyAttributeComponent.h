// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UMyAttributeComponent*, OwningComp, float, NewValue, float, Delta);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UMyAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyAttributeComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UMyAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
protected:
	//血量，Replicated指定这两个属性需要在客户端和服务器之间进行同步
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxRage;
	// 声明一个多播的RPC函数，用于同步生命值的改变
    // NetMulticast指定这个函数需要在客户端和服务器之间进行广播，Reliable指定需要保证可靠性
	UFUNCTION(NetMulticast, Reliable) // @FIXME: mark as unreliable once we moved the 'state' our of scharacter
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

public:	

	//多播委托FOnHealthChange
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChange;
	//多播委托FOnRageChanged
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChange;
	//对血量的改变
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	//对愤怒值的改变
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	//判断玩家是否存活
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;
	
	UFUNCTION(BlueprintCallable)
	float GetRage() const;
};
