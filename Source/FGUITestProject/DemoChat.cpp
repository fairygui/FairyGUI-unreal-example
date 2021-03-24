#include "DemoChat.h"

UDemoChat::UDemoChat()
{
    TArray<FString> tags{ "88","am","bs","bz","ch","cool","dhq","dn","fd","gz","han","hx","hxiao","hxiu" };

    for (auto& str : tags)
        EmojiParser.Handlers.Add(":" + str, FTagHandler::CreateUObject(this, &UDemoChat::OnTag_Emoji));
}

UDemoChat::~UDemoChat()
{
}

void UDemoChat::OnDemoStart_Implementation()
{
    UUIPackage::AddPackage(TEXT("/Game/UI/Chat"), this);

    MainView = UUIPackage::CreateObject("Chat", "Main", this)->As<UGComponent>();
    MainView->MakeFullScreen();
    MainView->SetParentToRoot();

    List = MainView->GetChild("list")->As<UGList>();
    List->SetVirtual();
    List->SetItemProvider(FListItemProvider::CreateUObject(this, &UDemoChat::GetListItemResource));
    List->SetItemRenderer(FListItemRenderer::CreateUObject(this, &UDemoChat::RenderListItem));

    Input = MainView->GetChild("input")->As<UGTextInput>();
    Input->OnSubmit.AddUniqueDynamic(this, &UDemoChat::OnSubmit);

    MainView->GetChild("btnSend")->OnClick.AddUniqueDynamic(this, &UDemoChat::OnClickSendBtn);
    MainView->GetChild("btnEmoji")->OnClick.AddUniqueDynamic(this, &UDemoChat::OnClickEmojiBtn);

    EmojiSelectUI = UUIPackage::CreateObject("Chat", "EmojiSelectUI", this)->As<UGComponent>();
    EmojiSelectUI->GetChild("list")->As<UGList>()->OnClickItem.AddUniqueDynamic(this, &UDemoChat::OnClickEmoji);
}

void UDemoChat::OnDemoEnd_Implementation()
{
    UUIPackage::RemovePackage("Chat", this);
}

void UDemoChat::OnClickSendBtn(UEventContext* Context)
{
    const FString& msg = Input->GetText();
    if (msg.IsEmpty())
        return;

    AddMsg("Unity", "r0", msg, true);
    Input->SetText("");
}

void UDemoChat::OnClickEmojiBtn(UEventContext* Context)
{
    MainView->GetUIRoot()->ShowPopup(EmojiSelectUI, Context->GetSender(), EPopupDirection::Up);
}

void UDemoChat::OnClickEmoji(UEventContext* Context)
{
    UGObject* item = Cast<UGObject>(Context->GetData().AsUObject());
    Input->SetText(Input->GetText() + "[:" + item->GetText() + "]");
}

void UDemoChat::OnSubmit(UEventContext* Context)
{
    OnClickSendBtn(nullptr);
}

void UDemoChat::RenderListItem(int32 Index, UGObject* Obj)
{
    UGButton* item = Obj->As<UGButton>();
    FMessageInfo& Info = Messages[Index];
    if (!Info.bFromMe)
        item->GetChild("name")->SetText(Info.Sender);
    item->SetIcon("ui://Chat/" + Info.SenderIcon);

    UGRichTextField* tf = item->GetChild("msg")->As<UGRichTextField>();
    tf->SetText(EmojiParser.Parse(Info.Msg));
}

FString UDemoChat::GetListItemResource(int32 Index)
{
    FMessageInfo Info = Messages[Index];
    if (Info.bFromMe)
        return "ui://Chat/chatRight";
    else
        return "ui://Chat/chatLeft";
}

void UDemoChat::AddMsg(const FString& Sender, const FString& SenderIcon, const FString& Msg, bool bFromMe)
{
    bool isScrollBottom = List->GetScrollPane()->IsBottomMost();
    FMessageInfo NewInfo;
    NewInfo.Sender = Sender;
    NewInfo.SenderIcon = SenderIcon;
    NewInfo.Msg = Msg;
    NewInfo.bFromMe = bFromMe;
    Messages.Add(NewInfo);

    if (NewInfo.bFromMe)
    {
        if (Messages.Num() == 1 || FMath::RandBool())
        {
            FMessageInfo ReplayInfo;
            ReplayInfo.Sender = "FairyGUI";
            ReplayInfo.SenderIcon = "r1";
            ReplayInfo.Msg = "Today is a good day. [:cool]";
            ReplayInfo.bFromMe = false;
            Messages.Add(ReplayInfo);
        }
    }

    if (Messages.Num() > 100)
        Messages.RemoveAt(0, Messages.Num() - 100);

    List->SetNumItems(Messages.Num());

    if (isScrollBottom)
        List->GetScrollPane()->ScrollBottom(true);
}

FString UDemoChat::OnTag_Emoji(const FString& TagName, bool bEnd, const FString& Attr)
{
    FString str = TagName.Mid(1);
    str.ToLowerInline();
    return "<img src='ui://Chat/" + str + "'/>";
}