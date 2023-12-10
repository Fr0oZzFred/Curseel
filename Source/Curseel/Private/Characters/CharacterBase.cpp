#include "Characters/CharacterBase.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent.Get();
}
bool ACharacterBase::IsAlive() const {
	return GetHealth() > 0.0f;
}

int32 ACharacterBase::GetAbilityLevel(AbilityID AbilityID) const {
	return static_cast<int32>(AbilityID);
}

void ACharacterBase::RemoveCharacterAbilities() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven) {
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities()) {
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass())) {
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++) {
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

void ACharacterBase::Die() {
	RemoveCharacterAbilities();

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid()) {
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
}
float ACharacterBase::GetHealth() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetHealth() : 0.0f;
}
float ACharacterBase::GetMaxHealth() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMaxHealth() : 0.0f;
}
float ACharacterBase::GetDamage() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetDamage() : 0.0f;
}
float ACharacterBase::GetDamageBuff() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetDamageBuff() : 0.0f;
}
float ACharacterBase::GetDamageReduction() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetDamageReduction() : 0.0f;
}
float ACharacterBase::GetMoveSpeed() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMoveSpeed() : 0.0f;
}

float ACharacterBase::GetAttackSpeed() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttackSpeed() : 0.0f;
}

void ACharacterBase::BeginPlay() {
	Super::BeginPlay();

	if (AbilitySystemComponent.IsValid()) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();

		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACharacterBase::HealthChanged);
		MoveSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMoveSpeedAttribute()).AddUObject(this, &ACharacterBase::MoveSpeedChanged);

		// Tag change callbacks
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACharacterBase::StunTagChanged);

		SetHealth(GetMaxHealth());
		GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
	}
}
void ACharacterBase::AddCharacterAbilities() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven) {
		return;
	}

	for (TSubclassOf<UCharacterGameplayAbility>& StartupAbility : CharacterAbilities) {
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility,
				GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ACharacterBase::InitializeAttributes() {
	if (!AbilitySystemComponent.IsValid()) return;

	if (!DefaultAttributes) {
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0.0f, EffectContext);
	if (NewHandle.IsValid()) {
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ACharacterBase::AddStartupEffects() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bStartupEffectsApplied) {
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& GameplayEffect : StartupEffects) {
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0.0f, EffectContext);
		if (NewHandle.IsValid()) {
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void ACharacterBase::HealthChanged(const FOnAttributeChangeData& Data) {

	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag)) {
		Die();
	}
}
void ACharacterBase::MoveSpeedChanged(const FOnAttributeChangeData& Data) {
	float MoveSpeed = Data.NewValue;
	OnCharacterMoveSpeedChanged.Broadcast(this, Data.NewValue);
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void ACharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {

	OnCharacterStunned.Broadcast(this, NewCount > 0);

	if (NewCount > 0) {
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

void ACharacterBase::SetHealth(float Health) {
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetHealth(Health);
}
void ACharacterBase::SetMaxHealth(float MaxHealth) {
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetMaxHealth(MaxHealth);
}
void ACharacterBase::SetDamage(float Damage) {
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetDamage(Damage);
}
void ACharacterBase::SetDamageBuff(float DamageBuff) {
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetDamageBuff(DamageBuff);
}
void ACharacterBase::SetDamageReduction(float DamageReduction) {
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetDamageReduction(DamageReduction);
}
void ACharacterBase::SetMoveSpeed(float MoveSpeed) {
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetDamageBuff(MoveSpeed);
}

void ACharacterBase::SetAttackSpeed(float AttackSpeed) {
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetAttackSpeed(AttackSpeed);
}
