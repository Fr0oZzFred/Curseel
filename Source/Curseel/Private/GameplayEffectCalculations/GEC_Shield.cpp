#include "GameplayEffectCalculations/GEC_Shield.h"

struct FTargetShieldCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

	FTargetShieldCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Shield, Target, false);
	}
};

static const FTargetShieldCapture& TargetShieldCapture() {
	static FTargetShieldCapture TargetShieldCapture;
	return TargetShieldCapture;
}

UGEC_Shield::UGEC_Shield() {
	RelevantAttributesToCapture.Add(FTargetShieldCapture().ShieldDef);
}

//Source and Target is inversed since we can only exec on target
void UGEC_Shield::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//Target
	float Shield = 0.0f;
	Shield += FMath::Max<float>(Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Shield")), false, -1.0f), 0.0f);

	//Apply
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		FTargetShieldCapture().ShieldProperty, EGameplayModOp::Additive, Shield));

}