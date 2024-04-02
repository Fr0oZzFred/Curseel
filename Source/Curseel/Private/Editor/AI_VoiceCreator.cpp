#include "Editor/AI_VoiceCreator.h"

UAI_VoiceRequestNode* UAI_VoiceRequestNode::AI_VoiceRequest(FString InUrl) {

	UAI_VoiceRequestNode* Node = NewObject<UAI_VoiceRequestNode>();
	Node->Url = InUrl;
	return Node;
}

void UAI_VoiceRequestNode::HandleResponse(
	FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, bool bSucceeded) {


	if (!HttpResponse.IsValid()) {
		this->OnResponse.Broadcast(0, TEXT(""), false, Time);
		return;
	}

	if (HttpResponse->GetResponseCode() != 200) return;

	TSharedPtr<FJsonObject> JsonObj;
	const FString ResponseBody = HttpResponse->GetContentAsString();

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
	if (FJsonSerializer::Deserialize(Reader, JsonObj)) {
		UKismetMathLibrary::DateTimeFromIsoString(*JsonObj->GetStringField("dateTime"), Time);
	}




	this->OnResponse.Broadcast(HttpResponse->GetResponseCode(), HttpResponse->GetContentAsString(), true, Time);
}


void UAI_VoiceRequestNode::Activate() {
	FHttpModule* Http = &FHttpModule::Get();
	FHttpRequestRef Request = Http->CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");

	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngineAgent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindUObject(this, &UAI_VoiceRequestNode::HandleResponse);
	Request->ProcessRequest();
}
