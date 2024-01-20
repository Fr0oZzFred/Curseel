#include "GameplayEffectCalculations/GEC_Health.h"

struct FTargetHealthCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FTargetHealthCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, false);
	}
};
static const FTargetHealthCapture& TargetHealthCapture() {
	static FTargetHealthCapture TargetHealthCapture;
	return TargetHealthCapture;
}

UGEC_Health::UGEC_Health() {
	RelevantAttributesToCapture.Add(FTargetHealthCapture().HealthDef);
}

//Source and Target is inversed since we can only exec on target
void UGEC_Health::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
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
	float Health = 0.0f;
	Health += FMath::Max<float>(Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Health")), false, -1.0f), 0.0f);

	//Apply
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		FTargetHealthCapture().HealthProperty, EGameplayModOp::Additive, Health));

}