#include "DemoBlueprintLibrary.h"

void UDemoBlueprintLibrary::ScaleUpWindow(UGWindow* Window)
{
    Window->SetScale(FVector2D(0.1f, 0.1f));
    Window->SetPivot(FVector2D(0.5f, 0.5f), Window->IsPivotAsAnchor());

    FGTween::To(Window->GetScale(), FVector2D::UnitVector, 0.3f)
        ->SetTarget(Window)
        ->OnUpdate(FTweenDelegate::CreateStatic(&FGTweenAction::ScaleXY))
        ->OnComplete(FSimpleDelegate::CreateUObject(Window, &UGWindow::OnShown));
}

void UDemoBlueprintLibrary::ScaleDownWindow(UGWindow* Window)
{
    FGTween::To(Window->GetScale(), FVector2D(0.1f, 0.1f), 0.3f)
        ->SetTarget(Window)
        ->OnUpdate(FTweenDelegate::CreateStatic(&FGTweenAction::ScaleXY))
        ->OnComplete(FSimpleDelegate::CreateUObject(Window, &UGWindow::HideImmediately));
}