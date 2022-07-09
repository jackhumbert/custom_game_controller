// Setup for dual Thrustermaster T-160000M Flight Sticks
// May need to be swapped depending on which device you plugged in first
public class CustomGameController_044F_B10A extends ICustomGameController {
  public func OnSetup() -> Void {
    if Equals(this.id, 0) {
      // left hand, maybe?
      this.SetButton(1  - 1, EInputKey.IK_Joy1);    // Flight_Fire1
      this.SetButton(2  - 1, EInputKey.IK_Joy2);    // Flight_Trick
      this.SetButton(3  - 1, EInputKey.IK_Joy3);    // Flight_ModeSwitchBackward
      this.SetButton(4  - 1, EInputKey.IK_Joy4);    // Flight_ModeSwitchForward
      this.SetButton(5  - 1, EInputKey.IK_Joy5);
      this.SetButton(6  - 1, EInputKey.IK_Joy6);

      this.SetAxis(1 - 1, EInputKey.IK_JoyU,  false, 0.5, 0.1);            // Flight_Lift
      this.SetAxis(2 - 1, EInputKey.IK_JoyV,  true,  0.5, 0.1);            // Flight_Sway
      this.SetAxis(3 - 1, EInputKey.IK_JoyR,  false, 0.5, 0.1);            // Flight_SurgePos
      this.SetAxis(7 - 1, EInputKey.IK_JoySlider1,  false, 0.5, 0.2);      // 
      
    } else {
      // other
      this.SetButton(1  - 1, EInputKey.IK_Joy7);    // Flight_Fire2
      this.SetButton(2  - 1, EInputKey.IK_Joy8);    // Flight_LinearBrake, Flight_AngularBrake
      this.SetButton(3  - 1, EInputKey.IK_Joy9);    // Flight_Toggle
      this.SetButton(4  - 1, EInputKey.IK_Joy10);   // Flight_UIToggle
      this.SetButton(5  - 1, EInputKey.IK_Joy11);
      this.SetButton(6  - 1, EInputKey.IK_Joy12);
      
      this.SetAxis(1 - 1, EInputKey.IK_JoyX, false, 0.5, 0.1);            // Flight_Roll
      this.SetAxis(2 - 1, EInputKey.IK_JoyY, true,  0.5, 0.1);            // Flight_Pitch
      this.SetAxis(3 - 1, EInputKey.IK_JoyZ, false, 0.5, 0.1);            // Flight_Yaw
      this.SetAxis(7 - 1, EInputKey.IK_JoySlider2,  false, 0.5, 0.1);     //
    }
  }
}