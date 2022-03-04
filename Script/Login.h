#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Login.generated.h"

/**
 * 
 */
UCLASS()
class LAB_PROJECT_MORPG_API ULogin : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IdTextBox;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* PasswordTextBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* LoginButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FailedText;

	FHttpModule* HTTPModule;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* FailedTextAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginBool")
		bool Logined = false;

	UFUNCTION(BlueprintImplementableEvent)
		void LoginSuccess();

private:
	UFUNCTION()
		void OnLoginClicked();
		void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY()
		FString ResultString;
		FText ResultText;
};
