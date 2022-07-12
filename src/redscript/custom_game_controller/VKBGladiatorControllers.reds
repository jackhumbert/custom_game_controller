// Right hand
public class CustomGameController_231D_0200 extends ICustomGameController {
  public func OnSetup() -> Void {
    this.SetButton(1 -  1, EInputKey.IK_Joy1);

    this.SetButton(4 -  1, EInputKey.IK_Pad_Start);

    this.SetButton(6 -  1, EInputKey.IK_Pad_Y_TRIANGLE);
    this.SetButton(7 -  1, EInputKey.IK_Pad_B_CIRCLE);
    this.SetButton(8 -  1, EInputKey.IK_Pad_A_CROSS);
    this.SetButton(9 -  1, EInputKey.IK_Pad_X_SQUARE);

    this.SetButton(10 - 1, EInputKey.IK_Pad_RightThumb);
    this.SetButton(22 - 1, EInputKey.IK_Pad_RightShoulder);

    this.SetAxis(1 - 1, EInputKey.IK_JoyX, false, 0.5, 0.005);
    this.SetAxis(2 - 1, EInputKey.IK_JoyY, true, 0.5, 0.005);
    this.SetAxis(4 - 1, EInputKey.IK_Pad_RightAxisX, false, 0.5, 0.005);
    this.SetAxis(5 - 1, EInputKey.IK_Pad_RightAxisY, false, 0.5, 0.005);
    this.SetAxis(6 - 1, EInputKey.IK_JoyZ, false, 0.5, 0.005);
    this.SetAxis(7 - 1, EInputKey.IK_JoySlider1, false, 0.5, 0.005);
    this.SetAxis(8 - 1, EInputKey.IK_JoySlider2, false, 0.5, 0.005);
  }
}

// Left hand
public class CustomGameController_231D_0201 extends ICustomGameController {
  public func OnSetup() -> Void {
    this.SetButton(1  - 1, EInputKey.IK_Joy2);

    this.SetButton(4  - 1, EInputKey.IK_Pad_Back_Select);

    this.SetButton(6  - 1, EInputKey.IK_Pad_DigitUp);
    this.SetButton(7  - 1, EInputKey.IK_Pad_DigitRight);
    this.SetButton(8  - 1, EInputKey.IK_Pad_DigitDown);
    this.SetButton(9  - 1, EInputKey.IK_Pad_DigitLeft);

    this.SetButton(10 - 1, EInputKey.IK_Pad_LeftThumb);
    this.SetButton(22 - 1, EInputKey.IK_Pad_LeftShoulder);

    this.SetAxis(1 - 1, EInputKey.IK_JoyU, false, 0.5, 0.005);
    this.SetAxis(2 - 1, EInputKey.IK_JoyV, true, 0.5, 0.005);
    this.SetAxis(4 - 1, EInputKey.IK_Pad_LeftAxisX, false, 0.5, 0.005);
    this.SetAxis(5 - 1, EInputKey.IK_Pad_LeftAxisY, false, 0.5, 0.005);
    this.SetAxis(6 - 1, EInputKey.IK_JoyR, false, 0.5, 0.005);
  }
}