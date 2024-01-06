#include "PawnBase.h"

APawnBase::APawnBase(const FObjectInitializer& ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = false;

	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));


	HardRefAbilitySystemComponent = CreateDefaultSubobject<UCharacterASC>(TEXT("AbilitySystemComponent"));
	HardRefAbilitySystemComponent->SetIsReplicated(true);

	HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AbilitySystemComponent = HardRefAbilitySystemComponent;

	HardRefAttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));

	AttributeSetBase = HardRefAttributeSetBase;
}
UAbilitySystemComponent* APawnBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent.Get();
}
bool APawnBase::IsAlive() const {
	return GetHealth() > 0.0f;
}
void APawnBase::Die() {
	RemoveAbilities();

	OnPawnDied.Broadcast(this);

	if (!AbilitySystemComponent.IsValid()) return;


	AbilitySystemComponent->CancelAbilities();

	FGameplayTagContainer EffectTagsToRemove;
	EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
	int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
	AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
}

void APawnBase::BeginPlay() {
	Super::BeginPlay();

	if (!AbilitySystemComponent.IsValid()) return;


	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	AddStartupEffects();
	AddAbilities();
	AddStartupBoosts();

	// Attribute change callbacks
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &APawnBase::HealthChanged);
	MoveSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMoveSpeedAttribute()).AddUObject(this, &APawnBase::MoveSpeedChanged);

	// Tag change callbacks
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APawnBase::StunTagChanged);

	SetHealth(GetMaxHealth());
}

#pragma region ASC
void APawnBase::AddAbilities() {
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

void APawnBase::InitializeAttributes() {
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

void APawnBase::AddStartupEffects() {
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

void APawnBase::AddStartupBoosts() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid()) {
		return;
	}

	for (UBoost* Boost : StartupBoosts) {
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Boost->Effect, 0.0f, EffectContext);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void APawnBase::RemoveAbilities() {
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

int32 APawnBase::GetAbilityLevel(AbilityID AbilityID) const {
	return static_cast<int32>(AbilityID);
}
#pragma endregion

#pragma region Delegates
void APawnBase::HealthChanged(const FOnAttributeChangeData& Data) {

	OnPawnHealthChanged.Broadcast(this, Data.NewValue, Data.OldValue);

	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
		Die();
}

void APawnBase::MoveSpeedChanged(const FOnAttributeChangeData& Data) {
	OnPawnMoveSpeedChanged.Broadcast(this);
}

void APawnBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	bool bIsStunned = NewCount > 0;

	OnPawnStunned.Broadcast(this, bIsStunned);


	if (!bIsStunned) return;

	FGameplayTagContainer AbilityTagsToCancel;
	AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	FGameplayTagContainer AbilityTagsToIgnore;
	AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

	AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
}
#pragma endregion

#pragma region Attributes
float APawnBase::GetHealth() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetHealth() : 0.0f;
}
float APawnBase::GetMaxHealth() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMaxHealth() : 0.0f;
}

float APawnBase::GetShield() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetShield() : 0.0f;
}
float APawnBase::GetMaxShield() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMaxShield() : 0.0f;
}

float APawnBase::GetAttack() {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttack() : 0.0f;
}
float APawnBase::GetAttackSpeed() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttackSpeed() : 0.0f;
}
float APawnBase::GetAttackPotency() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttackPotency() : 0.0f;
}

float APawnBase::GetHealthSteal() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetHealthSteal() : 0.0f;
}
float APawnBase::GetHealthStealPotency() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetHealthStealPotency() : 0.0f;
}
float APawnBase::GetShieldSteal() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetShieldSteal() : 0.0f;
}
float APawnBase::GetShieldStealPotency() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetShieldStealPotency() : 0.0f;
}

float APawnBase::GetAttackResistance() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetAttackResistance() : 0.0f;
}
float APawnBase::GetStealResistance() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetStealResistance() : 0.0f;
}

float APawnBase::GetMoveSpeed() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetMoveSpeed() : 0.0f;
}
float APawnBase::GetDashPower() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetDashPower() : 0.0f;
}
float APawnBase::GetDashCooldown() const {
	return AttributeSetBase.IsValid() ? AttributeSetBase->GetDashCooldown() : 0.0f;
}
#pragma endregion

#pragma region Hardcode
void APawnBase::SetHealth(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetHealth(NewValue);
}
void APawnBase::SetMaxHealth(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetMaxHealth(NewValue);
}

void APawnBase::SetShield(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetShield(NewValue);
}
void APawnBase::SetMaxShield(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetMaxShield(NewValue);
}

void APawnBase::SetAttack(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttack(NewValue);
}
void APawnBase::SetAttackSpeed(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttackSpeed(NewValue);
}
void APawnBase::SetAttackPotency(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttackPotency(NewValue);
}

void APawnBase::SetHealthSteal(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetHealthSteal(NewValue);
}
void APawnBase::SetHealthStealPotency(float NewValue) {
if (AttributeSetBase.IsValid()) AttributeSetBase->SetHealthStealPotency(NewValue);
}
void APawnBase::SetShieldSteal(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetShieldSteal(NewValue);
}
void APawnBase::SetShieldStealPotency(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetShieldStealPotency(NewValue);
}

void APawnBase::SetAttackResistance(float NewValue) {
if (AttributeSetBase.IsValid()) AttributeSetBase->SetAttackResistance(NewValue);
}
void APawnBase::SetStealResistance(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetStealResistance(NewValue);
}

void APawnBase::SetMoveSpeed(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetMoveSpeed(NewValue);
}
void APawnBase::SetDashPower(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetDashPower(NewValue);
}
void APawnBase::SetDashCooldown(float NewValue) {
	if (AttributeSetBase.IsValid()) AttributeSetBase->SetDashCooldown(NewValue);
}
#pragma endregion