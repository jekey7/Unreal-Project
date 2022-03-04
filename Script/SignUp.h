#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "SignUp.generated.h"

/**
 * 
 */
UCLASS()
class LAB_PROJECT_MORPG_API USignUp : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IdTextBox;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* PasswordTextBox;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* NicknameTextBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* SignUpButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SignUpFailResultText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SignUpSuccessResultText;

	FHttpModule* HTTPModule;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* SignUpFailTextAnim;
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* SignUpSuccessTextAnim;

private:
	UFUNCTION()
		void OnSignUpClicked();
		void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
