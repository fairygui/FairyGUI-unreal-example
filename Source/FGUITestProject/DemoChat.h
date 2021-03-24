// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DemoObject.h"
#include "Utils/UBBParser.h"
#include "DemoChat.generated.h"

UCLASS(Blueprintable)
class FGUITESTPROJECT_API UDemoChat : public UDemoObject
{
    GENERATED_BODY()

public:
    UDemoChat();
    virtual ~UDemoChat();

    virtual void OnDemoStart_Implementation() override;
    virtual void OnDemoEnd_Implementation() override;

private:
    UFUNCTION()
    void OnClickSendBtn(UEventContext* Context);
    UFUNCTION()
    void OnClickEmojiBtn(UEventContext* Context);
    UFUNCTION()
    void OnClickEmoji(UEventContext* Context);
    UFUNCTION()
    void OnSubmit(UEventContext* Context);
    void RenderListItem(int32 Index, UGObject* Obj);
    FString GetListItemResource(int32 Index);

    void AddMsg(const FString& Sender, const FString& SenderIcon, const FString& Msg, bool bFromMe);
    FString OnTag_Emoji(const FString& TagName, bool bEnd, const FString& Attr);

    UGComponent* MainView;
    UGList* List;
    UGTextInput* Input;
    UPROPERTY(Transient)
    UGComponent* EmojiSelectUI;
    FUBBParser EmojiParser;

    struct FMessageInfo
    {
        FString Sender;
        FString SenderIcon;
        FString Msg;
        bool bFromMe;
    };
    TArray<FMessageInfo> Messages;
};
