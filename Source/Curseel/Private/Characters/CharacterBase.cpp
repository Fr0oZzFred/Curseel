#include "Characters/CharacterBase.h"

ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = false;

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
void ACharacterBase::Die() {
	RemoveAbilities();

	OnCharacterDied.Broadcast(this);

	if (!AbilitySystemComponent.IsValid()) return;


	AbilitySystemComponent->CancelAbilities();

	FGameplayTagContainer EffectTagsToRemove;
	EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
	int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
	AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
}

void ACharacterBase::BeginPlay() {
	Super::BeginPlay();

	if (!AbilitySystemComponent.IsValid()) return;


	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	AddStartupEffects();
	AddAbilities();
	AddStartupBoosts();

	// Attribute change callbacks
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACharacterBase::HealthChanged);
	MoveSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMoveSpeedAttribute()).AddUObject(this, &ACharacterBase::MoveSpeedChanged);

	// Tag change callbacks
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACharacterBase::StunTagChanged);

	SetHealth(GetMaxHealth());
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
}

#pragma region ASC
void ACharacterBase::AddAbilities() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven) {
		return;
	}

	for (TSubclassOf<UCharacterGameplayAbility>& StartupAbility : Abilities) {
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

void ACharacterBase::AddStartupBoosts() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid()) {
		return;
	}

	for (UBoost* Boost : StartupBoosts) {
		AddBoost(Boost);
	}
}

void ACharacterBase::AddBoost(UBoost* Boost) {
	ActiveBoosts.Add(Boost);

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Boost->Effect, 0.0f, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACharacterBase::RemoveAbilities() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven) {
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities()) {
		if ((Spec.SourceObject == this) && Abilities.Contains(Spec.Ability->GetClass())) {
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++) {
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

int32 ACharacterBase::GetAbilityLevel(AbilityID AbilityID) const {
	return static_cast<int32>(AbilityID);
}
#pragma endregion

#pragma region Delegates
void ACharacterBase::HealthChanged(const FOnAttributeChangeData& Data) {

	OnCharacterHealthChanged.Broadcast(this, Data.NewValue, Data.OldValue);

	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
		Die();
}

void ACharacterBase::MoveSpeedChanged(const FOnAttributeChangeData& Data) {
	OnCharacterMoveSpeedChanged.Broadcast(this);

	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void ACharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	bool bIsStunned = NewCount > 0;

	OnCharacterStunned.Broadcast(this, bIsStunned);


	if (!bIsStunned) return;

	FGameplayTagContainer AbilityTagsToCancel;
	AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	FGameplayTagContainer AbilityTagsToIgnore;
	AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

	AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
}
#pragma endregion

#pragma region Attributes
float ACharacterBase::GetHealth() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetHealth() : 0.0f;
}
float ACharacterBase::GetMaxHealth() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMaxHealth() : 0.0f;
}

float ACharacterBase::GetShield() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetShield() : 0.0f;
}
float ACharacterBase::GetMaxShield() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMaxShield() : 0.0f;
}

float ACharacterBase::GetAttack() {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttack() : 0.0f;
}
float ACharacterBase::GetAttackSpeed() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttackSpeed() : 0.0f;
}
float ACharacterBase::GetAttackPotency() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttackPotency() : 0.0f;
}

float ACharacterBase::GetHealthSteal() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetHealthSteal() : 0.0f;
}
float ACharacterBase::GetHealthStealPotency() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetHealthStealPotency() : 0.0f;
}
float ACharacterBase::GetShieldSteal() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetShieldSteal() : 0.0f;
}
float ACharacterBase::GetShieldStealPotency() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetShieldStealPotency() : 0.0f;
}

float ACharacterBase::GetAttackResistance() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttackResistance() : 0.0f;
}
float ACharacterBase::GetStealResistance() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetStealResistance() : 0.0f;
}

float ACharacterBase::GetMoveSpeed() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMoveSpeed() : 0.0f;
}
float ACharacterBase::GetDashPower() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetDashPower() : 0.0f;
}
float ACharacterBase::GetDashCooldown() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetDashCooldown() : 0.0f;
}
#pragma endregion

#pragma region Hardcode
void ACharacterBase::SetHealth(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetHealth(NewValue);
}
void ACharacterBase::SetMaxHealth(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetMaxHealth(NewValue);
}

void ACharacterBase::SetShield(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetShield(NewValue);
}
void ACharacterBase::SetMaxShield(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetMaxShield(NewValue);
}

void ACharacterBase::SetAttack(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttack(NewValue);
}
void ACharacterBase::SetAttackSpeed(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttackSpeed(NewValue);
}
void ACharacterBase::SetAttackPotency(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttackPotency(NewValue);
}

void ACharacterBase::SetHealthSteal(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetHealthSteal(NewValue);
}
void ACharacterBase::SetHealthStealPotency(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetHealthStealPotency(NewValue);
}
void ACharacterBase::SetShieldSteal(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetShieldSteal(NewValue);
}
void ACharacterBase::SetShieldStealPotency(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetShieldStealPotency(NewValue);
}

void ACharacterBase::SetAttackResistance(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttackResistance(NewValue);
}
void ACharacterBase::SetStealResistance(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetStealResistance(NewValue);
}

void ACharacterBase::SetMoveSpeed(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetMoveSpeed(NewValue);
}
void ACharacterBase::SetDashPower(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetDashPower(NewValue);
}
void ACharacterBase::SetDashCooldown(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetDashCooldown(NewValue);
}
#pragma endregion