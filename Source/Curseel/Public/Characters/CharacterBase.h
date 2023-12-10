#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Curseel/Curseel.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/CharacterGameplayAbility.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACharacterBase*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterStunnedDelegate, ACharacterBase*, Character, bool, Stunned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCharacterHealthChangedDelegate, ACharacterBase*, Character, float, NewHealth, float, OldHealth);

UCLASS()
class CURSEEL_API ACharacterBase : public ACharacter, public IAbilitySystemInterface {
	GENERATED_BODY()


public:
	ACharacterBase(const class FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Characters")
	FCharacterDiedDelegate OnCharacterDied;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Characters")
	FCharacterStunnedDelegate OnCharacterStunned;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Characters")
	FCharacterHealthChangedDelegate OnCharacterHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters")
	virtual int32 GetAbilityLevel(AbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetShield() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetDamage() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetDamageBuff() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetDamageReduction() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetMoveSpeed() const;

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<class UCharacterASC> AbilitySystemComponent;
	TWeakObjectPtr<class UAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Characters")
	FText CharacterName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacterAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MoveSpeedChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	virtual void MoveSpeedChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	//Should be only used for hardcode/debug

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetHealth(float Health);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetMaxHealth(float MaxHealth);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetShield(float Shield);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetDamage(float Damage);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetDamageBuff(float DamageBuff);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetDamageReduction(float DamageReduction);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetMoveSpeed(float MoveSpeed);
};
