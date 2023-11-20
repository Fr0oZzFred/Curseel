#include "Characters/Player/EnhancedInputASC.h"

namespace EnhancedInputAbilitySystem_Impl {
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID() {
		return ++IncrementingInputID;
	}
}

void UEnhancedInputASC::SetInputBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle) {
	using namespace EnhancedInputAbilitySystem_Impl;

	FGameplayAbilitySpec* BindingAbility = FindAbilitySpec(AbilityHandle);

	FAbilityInputBinding* AbilityInputBinding = MappedAbilities.Find(InputAction);
	if (AbilityInputBinding) {
		FGameplayAbilitySpec* OldBoundAbility = FindAbilitySpec(AbilityInputBinding->BoundAbilitiesStack.Top());
		if (OldBoundAbility && OldBoundAbility->InputID == AbilityInputBinding->InputID) {
			OldBoundAbility->InputID = InvalidInputID;
		}
	}
	else {
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
		AbilityInputBinding->InputID = GetNextInputID();
	}

	if (BindingAbility) {
		BindingAbility->InputID = AbilityInputBinding->InputID;
	}

	AbilityInputBinding->BoundAbilitiesStack.Push(AbilityHandle);
	TryBindAbilityInput(InputAction, *AbilityInputBinding);
}

void UEnhancedInputASC::ClearInputBinding(FGameplayAbilitySpecHandle AbilityHandle) {
	using namespace EnhancedInputAbilitySystem_Impl;

	if (FGameplayAbilitySpec* FoundAbility = FindAbilitySpec(AbilityHandle)) {
		// Find the mapping for this ability
		auto MappedIterator = MappedAbilities.CreateIterator();
		while (MappedIterator) {
			if (MappedIterator.Value().InputID == FoundAbility->InputID) {
				break;
			}

			++MappedIterator;
		}

		if (MappedIterator) {
			FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

			if (AbilityInputBinding.BoundAbilitiesStack.Remove(AbilityHandle) > 0) {
				if (AbilityInputBinding.BoundAbilitiesStack.Num() > 0) {
					FGameplayAbilitySpec* StackedAbility = FindAbilitySpec(AbilityInputBinding.BoundAbilitiesStack.Top());
					if (StackedAbility && StackedAbility->InputID == 0) {
						StackedAbility->InputID = AbilityInputBinding.InputID;
					}
				}
				else {
					// NOTE: This will invalidate the `AbilityInputBinding` ref above
					RemoveEntry(MappedIterator.Key());
				}
				// DO NOT act on `AbilityInputBinding` after here (it could have been removed)


				FoundAbility->InputID = InvalidInputID;
			}
		}
	}
}

void UEnhancedInputASC::ClearAbilityBindings(UInputAction* InputAction) {
	RemoveEntry(InputAction);
}

void UEnhancedInputASC::OnAbilityInputPressed(UInputAction* InputAction) {
	using namespace EnhancedInputAbilitySystem_Impl;

	FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID)) {
		AbilityLocalInputPressed(FoundBinding->InputID);
	}
}

void UEnhancedInputASC::OnAbilityInputReleased(UInputAction* InputAction) {
	using namespace EnhancedInputAbilitySystem_Impl;

	FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID)) {
		AbilityLocalInputReleased(FoundBinding->InputID);
	}
}

void UEnhancedInputASC::RemoveEntry(UInputAction* InputAction) {
	if (FAbilityInputBinding* Bindings = MappedAbilities.Find(InputAction)) {
		if (InputComponent) {
			InputComponent->RemoveBindingByHandle(Bindings->OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Bindings->OnReleasedHandle);
		}

		for (FGameplayAbilitySpecHandle AbilityHandle : Bindings->BoundAbilitiesStack) {
			using namespace EnhancedInputAbilitySystem_Impl;

			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpec(AbilityHandle);
			if (AbilitySpec && AbilitySpec->InputID == Bindings->InputID) {
				AbilitySpec->InputID = InvalidInputID;
			}
		}

		MappedAbilities.Remove(InputAction);
	}
}

FGameplayAbilitySpec* UEnhancedInputASC::FindAbilitySpec(FGameplayAbilitySpecHandle Handle) {
	FGameplayAbilitySpec* FoundAbility = nullptr;
	FoundAbility = FindAbilitySpecFromHandle(Handle);
	return FoundAbility;
}

void UEnhancedInputASC::TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding) {
	if (InputComponent) {
		// Pressed event
		if (AbilityInputBinding.OnPressedHandle == 0) {
			AbilityInputBinding.OnPressedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UEnhancedInputASC::OnAbilityInputPressed, InputAction).GetHandle();
		}

		// Released event
		if (AbilityInputBinding.OnReleasedHandle == 0) {
			AbilityInputBinding.OnReleasedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UEnhancedInputASC::OnAbilityInputReleased, InputAction).GetHandle();
		}
	}
}

void UEnhancedInputASC::BeginPlay() {
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	if (IsValid(Owner) && Owner->InputComponent) {
		InputComponent = CastChecked<UEnhancedInputComponent>(Owner->InputComponent);
	}
}