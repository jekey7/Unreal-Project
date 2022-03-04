#include "SignUp.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void USignUp::NativeConstruct() {
	Super::NativeConstruct();

	SignUpButton->OnClicked.AddDynamic(this, &USignUp::OnSignUpClicked);
}

void USignUp::OnSignUpClicked() {
	FString id = IdTextBox->GetText().ToString();
	FString password = PasswordTextBox->GetText().ToString();
	FString nick = NicknameTextBox->GetText().ToString();
	FString SignUpData = "";

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("id", id);
	JsonObject->SetStringField("password", password);
	JsonObject->SetStringField("nick", nick);

	TSharedRef<TJsonWriter<>> json_writer = TJsonWriterFactory<>::Create(&SignUpData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), json_writer);

	HTTPModule = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = HTTPModule->CreateRequest();
	request->SetURL("203.250.133.43:8000/join");
	request->SetVerb("POST");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->SetContentAsString(SignUpData);
	request->OnProcessRequestComplete().BindUObject(this, &USignUp::OnResponseReceived);
	request->ProcessRequest();

	UE_LOG(LogTemp, Log, TEXT("Id :: %s"), *id);
	UE_LOG(LogTemp, Log, TEXT("PassWord :: %s"), *password);
	UE_LOG(LogTemp, Log, TEXT("Nickname :: %s"), *nick);
}

void USignUp::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	UE_LOG(LogTemp, Log, TEXT("Response :: %s"), *Response->GetContentAsString());

	FString result = Response->GetContentAsString();

	if (result == "1") {
		PlayAnimation(SignUpSuccessTextAnim);
	}
	else if (result == "2") {
		SignUpFailResultText->SetText(FText::FromString(FString::Printf(TEXT("동일한 아이디를 사용한 계정이 있습니다."))));
		PlayAnimation(SignUpFailTextAnim);
	}
	else if (result == "3") {
		SignUpFailResultText->SetText(FText::FromString(FString::Printf(TEXT("이미 사용된 이름입니다."))));
		PlayAnimation(SignUpFailTextAnim);
	}
	else {
		SignUpFailResultText->SetText(FText::FromString(FString::Printf(TEXT("회원가입 오류!"))));
		PlayAnimation(SignUpFailTextAnim);
	}
}