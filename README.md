# SMG2_IronCannonLauncher
The Iron Cannonballs from several places in SMG1.

Below is a list of everything that's included:
- The ObjectData archive for Cannonballs.
- JSON files for particle effects.

Below is a list of other things that you will need:

- A level editor.
- A Effect JSON supported particle editor.
- A UseResource file with Cannonball related sound labels.

## Credits
- @superhackio - Decompiled, ported and enhanced SMG1 code to SMG2.

## Setup
It is adviced to reference the template mod for this port. Regardless here are some parameters that apply to Cannonballs.

### IronCannonLauncher
| Parameter | Description |
| --- | --- |
| Obj_arg0 | Delay between shots in frames. Default is 300. |
| Obj_arg1 | Speed of the Cannonballs. Default is 30. |
| Obj_arg2 | Lifetime of the Cannonballs in frames. Default is 300. |
| SW_APPEAR | Makes the Cannonball appear when activated. |

### IronCannonLauncherPoint
| Parameter | Description |
| --- | --- |
| Obj_arg0 | Delay between shots in frames. Default is 300. |
| Obj_arg1 | Speed of the Cannonballs. Default is 30. |
| Obj_arg2 | Lifetime of the Cannonballs in frames. Default is 300. |
| SW_A | If used, it will only shoot bullets while this switch is activated. |