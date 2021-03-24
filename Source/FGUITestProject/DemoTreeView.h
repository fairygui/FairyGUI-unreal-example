// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DemoObject.h"
#include "DemoTreeView.generated.h"

UCLASS(Blueprintable)
class FGUITESTPROJECT_API UDemoTreeView : public UDemoObject
{
    GENERATED_BODY()

public:
    UDemoTreeView();
    virtual ~UDemoTreeView();

    virtual void OnDemoStart_Implementation() override;
    virtual void OnDemoEnd_Implementation() override;

    void OnClickNode(UEventContext* Context);
    void RenderTreeNode(UGTreeNode* Node, UGComponent* Obj);

private:
    UGComponent* MainView;
    UGTree* Tree1;
    UGTree* Tree2;
    UGTreeNode* SpecialNode;
};
