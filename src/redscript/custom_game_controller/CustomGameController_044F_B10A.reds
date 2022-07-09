public class CustomGameController_044F_B10A extends ICustomGameController {
  public func OnSetup() -> Void {
    if this.id == 0 {
      // left hand, maybe?
      this.SetButton(1  - 1, EInputKey.IK_Pad_A_CROSS);
      this.SetButton(2  - 1, EInputKey.IK_Pad_B_CIRCLE);
      this.SetButton(3  - 1, EInputKey.IK_Pad_X_SQUARE);
      this.SetButton(4  - 1, EInputKey.IK_Pad_Y_TRIANGLE);
      this.SetButton(5  - 1, EInputKey.IK_Pad_LeftShoulder);
      this.SetButton(6  - 1, EInputKey.IK_Pad_RightShoulder);
      this.SetButton(7  - 1, EInputKey.IK_Pad_Back_Select);
      this.SetButton(8  - 1, EInputKey.IK_Pad_Start);
      this.SetButton(9  - 1, EInputKey.IK_Pad_LeftThumb);
      this.SetButton(10 - 1, EInputKey.IK_Pad_RightThumb);
      this.SetButton(11 - 1, EInputKey.IK_Pad_DigitUp);
      this.SetButton(12 - 1, EInputKey.IK_Pad_DigitRight);
      this.SetButton(13 - 1, EInputKey.IK_Pad_DigitDown);
      this.SetButton(14 - 1, EInputKey.IK_Pad_DigitLeft);

      this.SetAxis(1 - 1, EInputKey.IK_Pad_LeftAxisY,    false, 0.5);
      this.SetAxis(2 - 1, EInputKey.IK_Pad_LeftAxisX,    false, 0.5);
      this.SetAxis(3 - 1, EInputKey.IK_Pad_RightAxisY,   false, 0.5);
      this.SetAxis(4 - 1, EInputKey.IK_Pad_RightAxisX,   false, 0.5);
      this.SetAxis(5 - 1, EInputKey.IK_Pad_LeftTrigger,  false, 0.0);
      this.SetAxis(6 - 1, EInputKey.IK_Pad_RightTrigger, false, 0.0);
    } else {
      // other
      this.SetButton(1  - 1, EInputKey.IK_Pad_A_CROSS);
      this.SetButton(2  - 1, EInputKey.IK_Pad_B_CIRCLE);
      this.SetButton(3  - 1, EInputKey.IK_Pad_X_SQUARE);
      this.SetButton(4  - 1, EInputKey.IK_Pad_Y_TRIANGLE);
      this.SetButton(5  - 1, EInputKey.IK_Pad_LeftShoulder);
      this.SetButton(6  - 1, EInputKey.IK_Pad_RightShoulder);
      this.SetButton(7  - 1, EInputKey.IK_Pad_Back_Select);
      this.SetButton(8  - 1, EInputKey.IK_Pad_Start);
      this.SetButton(9  - 1, EInputKey.IK_Pad_LeftThumb);
      this.SetButton(10 - 1, EInputKey.IK_Pad_RightThumb);
      this.SetButton(11 - 1, EInputKey.IK_Pad_DigitUp);
      this.SetButton(12 - 1, EInputKey.IK_Pad_DigitRight);
      this.SetButton(13 - 1, EInputKey.IK_Pad_DigitDown);
      this.SetButton(14 - 1, EInputKey.IK_Pad_DigitLeft);

      this.SetAxis(1 - 1, EInputKey.IK_Pad_LeftAxisY,    false, 0.5);
      this.SetAxis(2 - 1, EInputKey.IK_Pad_LeftAxisX,    false, 0.5);
      this.SetAxis(3 - 1, EInputKey.IK_Pad_RightAxisY,   false, 0.5);
      this.SetAxis(4 - 1, EInputKey.IK_Pad_RightAxisX,   false, 0.5);
      this.SetAxis(5 - 1, EInputKey.IK_Pad_LeftTrigger,  false, 0.0);
      this.SetAxis(6 - 1, EInputKey.IK_Pad_RightTrigger, false, 0.0);
    }
  }
}