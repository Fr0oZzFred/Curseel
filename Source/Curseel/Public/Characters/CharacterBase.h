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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterMoveSpeedChangedDelegate, ACharacterBase*, Character);


UCLASS()
class CURSEEL_API ACharacterBase : public ACharacter, public IAbilitySystemInterface {
	GENERATED_BODY()


public:
	ACharacterBase(const class FObjectInitializer& ObjectInitializer);

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


#pragma region ASC
protected:

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



#pragma region GetAttributes
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetMaxHealth() const;


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetShield() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetMaxShield() const;


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetAttack();
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetAttackSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetAttackPotency() const;


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetHealthSteal() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetHealthStealPotency() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetShieldSteal() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetShieldStealPotency() const;


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetAttackResistance() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetStealResistance() const;


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetMoveSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes")
	float GetDashPower() const;
#pragma endregion

#pragma region SetAttributes
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetHealth(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetMaxHealth(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetShield(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetMaxShield(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetAttack(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetAttackSpeed(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetAttackPotency(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetHealthSteal(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetHealthStealPotency(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetShieldSteal(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetShieldStealPotency(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetAttackResistance(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetStealResistance(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetMoveSpeed(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Characters|Attributes|Hardcode")
	void SetDashPower(float NewValue);
#pragma endregion
};