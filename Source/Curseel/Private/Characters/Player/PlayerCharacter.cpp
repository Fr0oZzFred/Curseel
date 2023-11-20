#include "Characters/Player/PlayerCharacter.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	StunTag = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"));
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Bind player inputto the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

void APlayerCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	ACharacterPlayerState* PS = GetPlayerState<ACharacterPlayerState>();

	if (!PS) return;

	InitiliazeStartingValues(PS);
	AddStartupEffects();
}

bool APlayerCharacter::CanMove() const {
	return IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(StunTag);
}

void APlayerCharacter::InitiliazeStartingValues(ACharacterPlayerState* PS) {
	if (!PS) return;

	AbilitySystemComponent = Cast<UCharacterASC>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	AttributeSetBase = PS->GetAttributeSetBase();
	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	InitializeAttributes();
	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());
}

void APlayerCharacter::OnRep_PlayerState() {
	ACharacterPlayerState* PS = GetPlayerState<ACharacterPlayerState>();

	if (!PS) return;

	InitiliazeStartingValues(PS);
	BindASCInput();
}

void APlayerCharacter::BindASCInput() {
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent)) {
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/Curseel"), FName("AbilityID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(AbilityID::Confirm), static_cast<int32>(AbilityID::Cancel)));

		ASCInputBound = true;
	}
}