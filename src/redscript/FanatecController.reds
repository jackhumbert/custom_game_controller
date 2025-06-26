public class CustomGameController_0EB7_0020 extends ICustomGameController {
  public func OnSetup() -> Void {
    if Equals(this.id, 1) {
      this.SetButton(3  - 1, EInputKey.IK_Pad_B_CIRCLE);
      this.SetButton(7  - 1, EInputKey.IK_Pad_DigitLeft);
      this.SetButton(11  - 1, EInputKey.IK_Pad_DigitRight);

      this.SetButton(10 - 1, EInputKey.IK_Pad_LeftThumb);
      this.SetButton(22 - 1, EInputKey.IK_Pad_RightThumb);

      this.SetAxis(0, EInputKey.IK_JoyZ, false, 0.5, 0.001); // steering
      // this.SetAxis(1, EInputKey.IK_Pad_LeftTrigger, true, 1.0, 0.001); // clutch
      this.SetAxis(2, EInputKey.IK_Pad_RightTrigger, true, 1.0, 0.001); // gas
      // this.SetAxis(3, EInputKey.IK_Pad_LeftTrigger, false, 0.0, 0.001);
      // this.SetAxis(4, EInputKey.IK_Pad_RightTrigger, false, 0.0, 0.001); 
      this.SetAxis(5, EInputKey.IK_Pad_LeftTrigger, true, 1.0, 0.001); // brake
      // this.SetAxis(6, EInputKey.IK_Pad_RightAxisX, true, 1.0, 0.001);
      // this.SetAxis(7, EInputKey.IK_Pad_RightAxisY, false, 0.5, 0.001);
    }
  }
}