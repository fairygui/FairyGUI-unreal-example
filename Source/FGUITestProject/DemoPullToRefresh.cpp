#include "DemoPullToRefresh.h"
#include "TimerManager.h"

bool UScrollPaneHeader::IsReadyToRefresh()
{
    return C1->GetSelectedIndex() == 1;
}

void UScrollPaneHeader::OnConstruct()
{
    C1 = GetController("c1");

    OnSizeChanged().AddUObject(this, &UScrollPaneHeader::OnSizeChangedHandler);
}

void UScrollPaneHeader::OnSizeChangedHandler()
{
    if (C1->GetSelectedIndex() == 2 || C1->GetSelectedIndex() == 3)
        return;

    if (GetHeight() > SourceSize.Y)
        C1->SetSelectedIndex(1);
    else
        C1->SetSelectedIndex(0);
}

void UScrollPaneHeader::SetRefreshStatus(int32 InStatus)
{
    C1->SetSelectedIndex(InStatus);
}

void UDemoPullToRefresh::OnDemoStart_Implementation()
{
    UUIPackage::AddPackage(TEXT("/Game/UI/PullToRefresh"), this);
    FUIObjectFactory::SetExtension("ui://PullToRefresh/Header", UScrollPaneHeader::StaticClass());

    MainView = UUIPackage::CreateObject("PullToRefresh", "Main", this)->As<UGComponent>();
    MainView->MakeFullScreen();
    MainView->SetParentToRoot();

    List1 = MainView->GetChild("list1")->As<UGList>();
    List1->SetItemRenderer(FListItemRenderer::CreateUObject(this, &UDemoPullToRefresh::RenderListItem1));
    List1->SetVirtual();
    List1->SetNumItems(1);
    List1->On(FUIEvents::PullDownRelease).AddUObject(this, &UDemoPullToRefresh::OnPullDownToRefreshHandler);

    List2 = MainView->GetChild("list2")->As<UGList>();
    List2->SetItemRenderer(FListItemRenderer::CreateUObject(this, &UDemoPullToRefresh::RenderListItem2));
    List2->SetVirtual();
    List2->SetNumItems(1);
    List2->On(FUIEvents::PullUpRelease).AddUObject(this, &UDemoPullToRefresh::OnPullUpToRefreshHandler);
}

void UDemoPullToRefresh::OnDemoEnd_Implementation()
{
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    UUIPackage::RemovePackage("PullToRefresh", this);
}

void UDemoPullToRefresh::RenderListItem1(int32 Index, UGObject* Obj)
{
    Obj->SetText("Item " + FString::FromInt(List1->GetNumItems() - Index - 1));
}

void UDemoPullToRefresh::RenderListItem2(int32 Index, UGObject* Obj)
{
    Obj->SetText("Item " + FString::FromInt(Index));
}

void UDemoPullToRefresh::OnPullDownToRefreshHandler(UEventContext* Context)
{
    UScrollPaneHeader* header = Cast<UScrollPaneHeader>(List1->GetScrollPane()->GetHeader());
    if (header->IsReadyToRefresh())
    {
        header->SetRefreshStatus(2);
        List1->GetScrollPane()->LockHeader(header->SourceSize.Y);

        //Simulate a async resquest
        GetWorld()->GetTimerManager().SetTimer(Handle1, this, &UDemoPullToRefresh::AddMoreItems, 2.f, false);
    }
}

void UDemoPullToRefresh::OnPullUpToRefreshHandler(UEventContext* Context)
{
    UGComponent* footer = Cast<UGComponent>(List2->GetScrollPane()->GetFooter());

    footer->GetController("c1")->SetSelectedIndex(1);
    List2->GetScrollPane()->LockFooter(footer->SourceSize.Y);

    GetWorld()->GetTimerManager().SetTimer(Handle2, this, &UDemoPullToRefresh::AddMoreItems2, 2, false);
}

void UDemoPullToRefresh::AddMoreItems()
{
    UScrollPaneHeader* header = Cast<UScrollPaneHeader>(List1->GetScrollPane()->GetHeader());

    List1->SetNumItems(List1->GetNumItems() + 5);
    header->SetRefreshStatus(3);
    List1->GetScrollPane()->LockHeader(35);

    GetWorld()->GetTimerManager().SetTimer(Handle1, this, &UDemoPullToRefresh::HideMessage, 2, false);
}

void UDemoPullToRefresh::HideMessage()
{
    UScrollPaneHeader* header = Cast<UScrollPaneHeader>(List1->GetScrollPane()->GetHeader());

    header->SetRefreshStatus(0);
    List1->GetScrollPane()->LockHeader(0);
}

void UDemoPullToRefresh::AddMoreItems2()
{
    UGComponent* footer = List2->GetScrollPane()->GetFooter();

    List2->SetNumItems(List2->GetNumItems() + 5);
    footer->GetController("c1")->SetSelectedIndex(0);
    List2->GetScrollPane()->LockFooter(0);
}