#include "DemoTreeView.h"

UDemoTreeView::UDemoTreeView()
{

}

UDemoTreeView::~UDemoTreeView()
{
}

void UDemoTreeView::OnDemoStart_Implementation()
{
    UUIPackage::AddPackage(TEXT("/Game/UI/TreeView"), this);

    MainView = UUIPackage::CreateObject("TreeView", "Main", this)->As<UGComponent>();
    MainView->MakeFullScreen();
    MainView->SetParentToRoot();

    Tree1 = MainView->GetChild("tree")->As<UGTree>();
    Tree1->On(FUIEvents::ClickItem).AddUObject(this, &UDemoTreeView::OnClickNode);

    Tree2 = MainView->GetChild("tree2")->As<UGTree>();
    Tree2->On(FUIEvents::ClickItem).AddUObject(this, &UDemoTreeView::OnClickNode);
    Tree2->SetTreeNodeRenderer(FTreeNodeRenderer::CreateUObject(this, &UDemoTreeView::RenderTreeNode));

    UGTreeNode* TopNode = UGTreeNode::CreateNode(true);
    TopNode->UserData = FString("I'm a top node");
    Tree2->GetRootNode()->AddChild(TopNode);
    for (int32 i = 0; i < 5; i++)
    {
        UGTreeNode* Node = UGTreeNode::CreateNode();
        Node->UserData = FString("Hello " + FString::FromInt(i));
        TopNode->AddChild(Node);
    }

    UGTreeNode* AFolderNode = UGTreeNode::CreateNode(true);
    AFolderNode->UserData = FString("A folder node");
    TopNode->AddChild(AFolderNode);
    for (int32 i = 0; i < 5; i++)
    {
        UGTreeNode* Node = UGTreeNode::CreateNode();
        Node->UserData = FString("Good " + FString::FromInt(i));
        AFolderNode->AddChild(Node);
    }

    for (int32 i = 0; i < 3; i++)
    {
        UGTreeNode* Node = UGTreeNode::CreateNode();
        Node->UserData = FString("World " + FString::FromInt(i));
        TopNode->AddChild(Node);
    }

    SpecialNode = UGTreeNode::CreateNode();
    SpecialNode->UserData = FString("I'm a top node too");
    Tree2->GetRootNode()->AddChild(SpecialNode);
}

void UDemoTreeView::OnDemoEnd_Implementation()
{
    UUIPackage::RemovePackage("TreeView", this);
}

void UDemoTreeView::OnClickNode(UEventContext* Context)
{
    UGTreeNode* Node = Cast<UGObject>(Context->GetData().AsUObject())->GetTreeNode();
    UE_LOG(LogFairyGUI, Warning, TEXT("click node %s"), *Node->GetText());
}

void UDemoTreeView::RenderTreeNode(UGTreeNode* Node, UGComponent* Obj)
{
    if (Node->IsFolder())
    {
        Obj->SetText(Node->UserData.AsString());
    }
    else
    {
        if (Node == SpecialNode)
            Obj->SetIcon("ui://TreeView/heart");
        else
            Obj->SetIcon("ui://TreeView/file");
        Obj->SetText(Node->UserData.AsString());
    }
}