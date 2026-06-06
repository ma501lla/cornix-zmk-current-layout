# Configuration Reference

Detailed explanation of behaviors, macros, and firmware settings used in this keymap.

## Design Philosophy

This keymap is **combo-centric**. Modifier keys (Shift, Ctrl, Alt, GUI) are accessed through combos rather than homerow mods. The `behaviors` block still contains HM definitions (`hm_l`, `hm_r`, `hm_shift_l`, `hm_shift_r`) for historical reference, but they are **currently unused** in any layer's bindings.

For combo placement and usage, see [Combo Reference](combos.md). For the design history and decision rationale, see [Keymap Design Log](keymap-design.ja.md) (Japanese only).

## Behaviors

### Mod-Morph: `bs_del`

Sends different keycodes depending on whether Shift is held.

```dts
bs_del: bs_del {
    compatible = "zmk,behavior-mod-morph";
    bindings = <&kp BSPC>, <&kp DEL>;
    mods = <(MOD_LSFT|MOD_RSFT)>;
};
```

- **Tap**: Backspace
- **Shift + Tap**: Delete

Used in the `cb_kp_bs` combo (I + O).

### Layer Tap: `lt`

Hold to activate a layer, tap to send a keycode.

```dts
&lt 5 ESCAPE    // Tap: Escape, Hold: NumPad layer
&lt 4 SPACE     // Tap: Space, Hold: Navi layer
&lt 1 BACKSPACE // Tap: Backspace, Hold: Symbols layer
```

### Layer Tap (tap-preferred): `lt_g`

A `tap-preferred` variant of layer-tap. Currently defined but not used in any binding after the HM removal. Kept for potential future use.

```dts
lt_g: layer_tap_g {
    compatible = "zmk,behavior-hold-tap";
    bindings = <&mo>, <&kp>;
    flavor = "tap-preferred";
    tapping-term-ms = <HM_TAPPING_TERM>;
    quick-tap-ms = <200>;
    require-prior-idle-ms = <HM_PRIOR_IDLE>;
};
```

### Unused HM Behaviors (legacy)

The following are defined in the file but currently unused after the combo-centric redesign. They may be removed in a future cleanup:

- `hm_l`, `hm_r` — left/right hand homerow mods
- `hm_shift_l`, `hm_shift_r` — Shift-specific homerow mods with shorter tapping term
- `hm` — non-positional homerow mod

If you fork this repo and want to re-enable homerow mods, refer to these behaviors and rebind home row keys (A/S/D/F, J/K/L/;) to use them.

## Macros

### IME Toggle: `my_macro_zenhan`

Sends Alt + `` ` `` to toggle IME on/off (Windows, US keyboard layout).

```dts
my_macro_zenhan: my_macro_zenhan {
    compatible = "zmk,behavior-macro";
    bindings =
        <&macro_press>, <&kp LEFT_ALT>,
        <&macro_tap>, <&kp GRAVE>,
        <&macro_release>, <&kp LEFT_ALT>;
};
```

Used in the E + R combo.

## Layers

The keymap currently has **7 layers** (0–6):

| # | Name | Entry | Purpose |
|---|------|-------|---------|
| 0 | Base | Default | Letters, combos for modifiers/symbols/editing |
| 1 | Symbols | `mo 1` thumb (position 42) or layer-activation combos | Symbols, numbers, F-keys |
| 2 | Mix | `mo 2` thumb (position 45) or layer-activation combo | Function keys, number row, editing |
| 3 | Adjust | Conditional (Symbols + Mix simultaneously) | BT management, system controls |
| 4 | Navi | `lt 4 SPACE` thumb hold (position 43) | Arrow keys, navigation |
| 5 | NumPad | `lt 5 ESCAPE` thumb hold (position 41) | Numeric keypad |
| 6 | Debug | `mo 6` within Navi or NumPad layer | Bluetooth pairing, bootloader |

The historical Combo layer (formerly index 6) has been removed, and the Debug layer was renumbered from 7 to 6.

## Conditional Layers

When both Symbols (layer 1) and Mix (layer 2) are active simultaneously, Adjust (layer 3) activates automatically.

```dts
conditional_layers {
    adjust_layer {
        if-layers = <2 1>;
        then-layer = <3>;
    };
};
```

## Encoder (Sensor Bindings)

Each layer can override encoder behavior. Layers without `sensor-bindings` fall back to lower layers.

| Layer | Left Encoder ↑/↓ | Right Encoder ↑/↓ |
|-------|-----------------|-------------------|
| Base | Volume Up / Down | Page Up / Page Down |
| Symbols | Cursor Right / Left | Cursor Down / Up |
| Mix | Ctrl+Tab / Ctrl+Shift+Tab (browser tab switch) | Volume Up / Down |
| Navi | End / Home | Page Down / Up |
| NumPad | KP_PLUS / KP_MINUS | KP_MULTIPLY / KP_DIVIDE |
| Adjust, Debug | (fallback to Base) | (fallback to Base) |

## Firmware Settings (`cornix.conf`)

| Setting | Value | Purpose |
|---------|-------|---------|
| `CONFIG_BT_CTLR_TX_PWR_PLUS_8` | `y` | Increase BT TX power for stable split connection |
| `CONFIG_ZMK_IDLE_SLEEP_TIMEOUT` | `1800000` | Sleep after 30 min idle (saves battery) |
| `CONFIG_ZMK_KSCAN_DEBOUNCE_PRESS_MS` | `10` | Key press debounce |
| `CONFIG_ZMK_KSCAN_DEBOUNCE_RELEASE_MS` | `10` | Key release debounce |
| `CONFIG_BT_PERIPHERAL_PREF_MIN_INT` | `6` | BT connection interval min (7.5 ms) |
| `CONFIG_BT_PERIPHERAL_PREF_MAX_INT` | `12` | BT connection interval max (15 ms) |
| `CONFIG_ZMK_STUDIO` | `y` | Enable ZMK Studio (live keymap editing over USB) |
| `CONFIG_ZMK_STUDIO_LOCKING` | `n` | Disable studio locking (personal use) |
| `CONFIG_ZMK_USB_LOGGING` | `n` | Disable USB logging |

## ZMK Studio Support

This config builds with **ZMK Studio** support on the central (left) side. Connect the left half via USB and open <https://zmk.studio/> in Chrome or Edge to live-edit the keymap.

The `build.yaml` adds the `studio-rpc-usb-uart` snippet to `cornix_left//zmk`:

```yaml
include:
  - board: cornix_left//zmk
    snippet: studio-rpc-usb-uart
    artifact-name: cornix_left
```

**Note**: ZMK Studio can edit static keymap bindings (which key maps to which keycode) but **cannot edit behavior definitions** (`bs_del`, layer-taps, mod-morphs, encoder mappings). For those, edit `config/cornix.keymap` directly and rebuild.
