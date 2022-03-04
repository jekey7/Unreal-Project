#include "Login.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void ULogin::NativeConstruct() {
	Super::NativeConstruct();

	LoginButton->OnClicked.AddDynamic(this, &ULogin::OnLoginClicked);
}

void ULogin::OnLoginClicked() {
	FString id = IdTextBox->GetText().ToString();
	FString password = PasswordTextBox->GetText().ToString();
	FString LoginData = "";

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("id", id);
	JsonObject->SetStringField("password", password);

	TSharedRef<TJsonWriter<>> json_writer = TJsonWriterFactory<>::Create(&LoginData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), json_writer);

	HTTPModule = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = HTTPModule->CreateRequest();
	request->SetURL("203.250.133.43:8000/login");
	request->SetVerb("POST");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->SetContentAsString(LoginData);
	request->OnProcessRequestComplete().BindUObject(this, &ULogin::OnResponseReceived);
	request->ProcessRequest();

	UE_LOG(LogTemp, Log, TEXT("Id :: %s"), *id);
	UE_LOG(LogTemp, Log, TEXT("PassWord :: %s"), *password);
}

void ULogin::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	UE_LOG(LogTemp, Log, TEXT("Response :: %s"), *Response->GetContentAsString());

	FString result = Response->GetContentAsString();

	UE_LOG(LogTemp, Log, TEXT("ChangedResponse :: %s"), *Response->GetContentAsString());

	if (result == "1") {
		UE_LOG(LogTemp, Log, TEXT("LoginSuccess!"));
		LoginSuccess();
	}
	else {
		PlayAnimation(FailedTextAnim);
	}
}
