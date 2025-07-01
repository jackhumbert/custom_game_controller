## Axes & Buttons

In addition to the built-in gamepad axis, buttons, and keys, the following have also been added:

### Axes

* `IK_JoyU`
* `IK_JoyV`
* `IK_JoySlider1`
* `IK_JoySlider2`
* `IK_JoyX`
* `IK_JoyY`
* `IK_JoyZ`
* `IK_JoyR`

### Buttons

* `IK_Joy1`
* `IK_Joy2`
* `IK_Joy3`
* `IK_Joy4`
* `IK_Joy5`
* `IK_Joy6`
* `IK_Joy7`
* `IK_Joy8`
* `IK_Joy9`
* `IK_Joy10`
* `IK_Joy11`
* `IK_Joy12`
* `IK_Joy13`
* `IK_Joy14`
* `IK_Joy15`
* `IK_Joy16`

## Configuration

To configure your own device, you'll need to extend the redscript class `ICustomGameController`, seen below, with notes. Examples can be found in the [examples folder](https://github.com/jackhumbert/custom_game_controller/tree/main/examples):

```swift
﻿public abstract native class ICustomGameController extends IScriptable {
  public native let calibrated: Bool;
  public native let pid: Uint16;
  public native let vid: Uint16;
  public native let id: Int32;
  public native let buttons: array<Bool>;
  // This is the enum used by `switches` interally:
  //   Center    = 0
  //   Up        = 1
  //   UpRight   = 2
  //   Right     = 3
  //   DownRight = 4
  //   Down      = 5
  //   DownLeft  = 6
  //   Left      = 7
  //   UpLeft    = 8
  public native let switches: array<Uint32>;
  public native let axes: array<Float>;
 
  public native let buttonKeys: array<EInputKey>;
  public native let axisKeys: array<EInputKey>;
  public native let axisInversions: array<Bool>;
  public native let axisCenters: array<Float>;
  public native let axisDeadzones: array<Float>;

  // Maps a controller button to a key
  //   button: 0-indexed
  //   key: https://nativedb.red4ext.com/EInputKey
  public native func SetButton(button: Int32, key: EInputKey);

  // Maps a controller axis to a key
  //   axis: 0-indexed
  //   key: https://nativedb.red4ext.com/EInputKey
  //   inverted: whether or not the axis should be inverted
  //   center: value between 0.0-1.0 that acts as the natural position of the axis
  //   deadzone: value between 0.0-1.0 that acts as a threshold for movement
  public native func SetAxis(axis: Int32, key: EInputKey, inverted: Bool, center: Float, deadzone: Float);

  // Called after the controller is created but not intialized (no values are read yet)
  // Use this to call SetButton & SetAxis
  public abstract func OnSetup() -> Void;

  // Called after new values are read, but before they're assigned
  public abstract func OnUpdate() -> Void;

  // Called when axis values are read from the controller, after center & deadzone correction
  //   index: 0-indexed axis index
  //   value: axis value to be compared & assigned if different
  public abstract func GetAxisValue(index: Uint32, value: Float) -> Float;
}
```