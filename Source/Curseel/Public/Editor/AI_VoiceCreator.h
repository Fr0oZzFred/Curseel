#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "AI_VoiceCreator.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FVoiceRequest,
	int, StatusCode,
	FString, HttpBody,
	bool, bSucceeded,
	FDateTime, Time
);

UCLASS()
class CURSEEL_API UAI_VoiceRequestNode : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	FString Url;
	FDateTime Time;

public:
	UPROPERTY(BlueprintAssignable)
	FVoiceRequest OnResponse;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAI_VoiceRequestNode* AI_VoiceRequest(FString InUrl);
	void HandleResponse(
		FHttpRequestPtr HttpRequest,
		const FHttpResponsePtr HttpResponse,
		bool bSucceeded
		);

	virtual void Activate() override;

};


UCLASS()
class CURSEEL_API UAI_VoiceCreator : public UObject
{
	GENERATED_BODY()
	
public:

};
