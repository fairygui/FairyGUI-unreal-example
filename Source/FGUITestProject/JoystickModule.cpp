#include "JoystickModule.h"

FJoystickModule::FJoystickModule(UGComponent* MainView)
{
    Button = MainView->GetChild("joystick")->As<UGButton>();
    Button->bChangeStateOnClick = false;

    Thumb = Button->GetChild("thumb");
    TouchArea = MainView->GetChild("joystick_touch");
    Center = MainView->GetChild("joystick_center");

    InitPos = Center->GetPosition() + Center->GetSize() * 0.5f;
    PointerId = -1;
    Radius = 150;

    TouchArea->On(FUIEvents::TouchBegin).AddRaw(this, &FJoystickModule::OnTouchBegin);
    TouchArea->On(FUIEvents::TouchMove).AddRaw(this, &FJoystickModule::OnTouchMove);
    TouchArea->On(FUIEvents::TouchEnd).AddRaw(this, &FJoystickModule::OnTouchEnd);
}

void FJoystickModule::OnTouchBegin(UEventContext* Context)
{
    if (PointerId == -1)//First touch
    {
        PointerId = Context->GetPointerIndex();

        if (TweenerHandle.IsValid())
            FGTween::Kill(TweenerHandle);

        UGRoot* UIRoot = Button->GetUIRoot();
        FVector2D pt = UIRoot->GlobalToLocal(Context->GetPointerPosition());
        Button->SetSelected(true);

        if (pt.X < 0)
            pt.X = 0;
        else if (pt.X > TouchArea->GetWidth())
            pt.X = TouchArea->GetWidth();

        if (pt.Y > UIRoot->GetHeight())
            pt.Y = UIRoot->GetHeight();
        else if (pt.Y < TouchArea->GetY())
            pt.Y = TouchArea->GetY();

        LastStagePos = StartStagePos = pt;

        Center->SetVisible(true);
        Center->SetPosition(pt - Center->GetSize() * 0.5f);
        Button->SetPosition(pt - Button->GetSize() * 0.5f);

        FVector2D deltaPos = pt - InitPos;
        float degrees = FMath::Atan2(deltaPos.Y, deltaPos.X) * 180 / PI;
        Thumb->SetRotation(degrees + 90);

        Context->CaptureTouch();
    }
}

void FJoystickModule::OnTouchMove(UEventContext* Context)
{
    if (PointerId != -1 && Context->GetPointerIndex() == PointerId)
    {
        UGRoot* UIRoot = Button->GetUIRoot();
        FVector2D pt = UIRoot->GlobalToLocal(Context->GetPointerPosition());
        FVector2D move = pt - LastStagePos;
        LastStagePos = pt;
        FVector2D offset = Button->GetPosition() + move + Button->GetSize() * 0.5f - StartStagePos;

        float rad = FMath::Atan2(offset.Y, offset.X);
        float degree = rad * 180 / PI;
        Thumb->SetRotation(degree + 90);

        FVector2D maxPos(Radius * FMath::Cos(rad), Radius * FMath::Sin(rad));
        if (FMath::Abs(offset.X) > FMath::Abs(maxPos.X))
            offset.X = maxPos.X;
        if (FMath::Abs(offset.Y) > FMath::Abs(maxPos.Y))
            offset.Y = maxPos.Y;

        FVector2D buttonPos = StartStagePos + offset;
        if (buttonPos.X < 0)
            buttonPos.X = 0;
        if (buttonPos.Y > UIRoot->GetHeight())
            buttonPos.Y = UIRoot->GetHeight();

        Button->SetPosition(buttonPos - Button->GetSize() * 0.5f);

        OnMoving.ExecuteIfBound(degree);
    }
}

void FJoystickModule::OnTouchEnd(UEventContext* Context)
{
    if (PointerId != -1 && Context->GetPointerIndex() == PointerId)
    {
        PointerId = -1;
        Thumb->SetRotation(Thumb->GetRotation() + 180);
        Center->SetVisible(false);

        TweenerHandle = FGTween::To(Button->GetPosition(), InitPos - Button->GetSize() * 0.5f, 0.3f)
            ->SetTarget(Button)
            ->OnUpdate(FTweenDelegate::CreateStatic(&FGTweenAction::Move))
            ->OnComplete(FSimpleDelegate::CreateLambda([this]()
        {
            Button->SetSelected(false);
            Thumb->SetRotation(0);
            Center->SetVisible(true);
            Center->SetPosition(InitPos - Center->GetSize() * 0.5f);
        }))
            ->GetHandle();

        OnMoveEnd.ExecuteIfBound();
    }
}
