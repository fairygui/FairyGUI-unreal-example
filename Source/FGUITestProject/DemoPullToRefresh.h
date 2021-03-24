// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DemoObject.h"
#include "DemoPullToRefresh.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FGUITESTPROJECT_API UDemoPullToRefresh : public UDemoObject
{
	GENERATED_BODY()

public:
    virtual void OnDemoStart_Implementation() override;
    virtual void OnDemoEnd_Implementation() override;

private:
    void OnPullDownToRefreshHandler(UEventContext* Context);
    void OnPullUpToRefreshHandler(UEventContext* Context);
    void RenderListItem1(int32 Index, UGObject* Obj);
    void RenderListItem2(int32 Index, UGObject* Obj);

    void AddMoreItems();
    void HideMessage();
    void AddMoreItems2();

    UGComponent* MainView;
    UGList* List1;
    UGList* List2;

    FTimerHandle Handle1;
    FTimerHandle Handle2;
};

UCLASS()
class UScrollPaneHeader : public UGComponent
{
    GENERATED_BODY()

public:
    bool IsReadyToRefresh();
    void SetRefreshStatus(int32 InStatus);

protected:
    virtual void OnConstruct() override;

private:
    void OnSizeChangedHandler();

    UGController* C1;
};