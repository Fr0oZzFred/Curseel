#include "Characters/Player/CharacterPlayerState.h"

ACharacterPlayerState::ACharacterPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterASC>(TEXT("AbilitySystemCompoenent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* ACharacterPlayerState::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

UAttributeSetBase* ACharacterPlayerState::GetAttributeSetBase() const {
	return AttributeSetBase;
}

bool ACharacterPlayerState::IsAlive() const {
	return GetHealth() > 0.0f;
}

float ACharacterPlayerState::GetHealth() const {
	return AttributeSetBase->GetHealth();
}

float ACharacterPlayerState::GetMaxHealth() const {
	return AttributeSetBase->GetMaxHealth();
}

void ACharacterPlayerState::BeginPlay() {
	Super::BeginPlay();
	if (!AbilitySystemComponent) return;

	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACharacterPlayerState::HealthChanged);

	MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &ACharacterPlayerState::MaxHealthChanged);

	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(
		FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACharacterPlayerState::StunTagChanged);
}

void ACharacterPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data) {
	UE_LOG(LogTemp, Warning, TEXT("Level Changed!"));
}
void ACharacterPlayerState::HealthChanged(const FOnAttributeChangeData& Data) {
	UE_LOG(LogTemp, Warning, TEXT("Health Changed!"));
}
void ACharacterPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data) {
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed!"));
}

void ACharacterPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	if (NewCount > 0) {
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));


		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}