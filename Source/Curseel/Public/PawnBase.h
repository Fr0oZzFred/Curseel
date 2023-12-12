#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Curseel/Curseel.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/CharacterGameplayAbility.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "PawnBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, APawnBase*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterStunnedDelegate, APawnBase*, Pawn, bool, Stunned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCharacterHealthChangedDelegate, APawnBase*, Pawn, float, NewHealth, float, OldHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterMoveSpeedChangedDelegate, APawnBase*, Pawn);

UCLASS()
class CURSEEL_API APawnBase : public APawn, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	APawnBase(const class FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters")
	virtual void Die();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Characters")
	FText Name;

protected:
	virtual void BeginPlay() override;
	//Not used for now
	//virtual void Tick(float DeltaTime) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region ASC
	TWeakObjectPtr<class UCharacterASC> AbilitySystemComponent;
	TWeakObjectPtr<class UAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> Abilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void RemoveAbilities();

	UFUNCTION(BlueprintCallable, Category = "GAS|Characters")
	virtual int32 GetAbilityLevel(AbilityID AbilityID) const;
#pragma endregion

#pragma region Delegates
public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Characters")
	FCharacterDiedDelegate OnCharacterDied;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Characters")
	FCharacterStunnedDelegate OnCharacterStunned;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Characters")
	FCharacterHealthChangedDelegate OnCharacterHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Characters")
	FCharacterMoveSpeedChangedDelegate OnCharacterMoveSpeedChanged;


protected:
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MoveSpeedChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	virtual void MoveSpeedChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
#pragma endregion


#pragma region Attributes
public:
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
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetAttackSpeed() const;
#pragma endregion


#pragma region Hardcode
	//Should be only used for hardcode/debug

protected:
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
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	virtual void SetAttackSpeed(float AttackSpeed);
#pragma endregion
};
