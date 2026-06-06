# Combo Reference

All combos defined in `config/cornix.keymap`.

All combos have `require-prior-idle-ms = <150>` by default to suppress misfires during fast typing. Combos only trigger if 150 ms has passed since the last keypress.

## Design Philosophy

This keymap follows a **combo-centric design**. There are **no homerow mods (HM)** — modifier keys are accessed exclusively through combos. This eliminates HM "brake" feel and misfire risks from timing-based hold-tap detection.

Modifier combos use `&kp LXXX` (direct keypress) rather than `&sk` (sticky key) so that:

- **Sustained hold** is possible (keep combo pressed → modifier held across multiple target keys)
- **Double-tap detection** works (tap combo twice quickly → IDE features like Ctrl-Ctrl)
- **Multi-modifier stacking** is natural (hold multiple combos simultaneously → Ctrl+Alt+target)

## Modifier Combos

Press both keys simultaneously to engage the modifier. The modifier is held while at least one of the combo keys remains physically pressed.

### Left Hand

| Modifier | Keys (position) | Fingers |
|----------|----------------|---------|
| L-Shift  | A + S (13+14) | pinky + ring |
| L-Ctrl   | D + F (15+16) | middle + index |
| L-Alt    | A + D (13+15) | pinky + middle |
| L-GUI    | Z + X (25+26) | bottom row pinky + ring |

### Right Hand

| Modifier | Keys (position) | Fingers |
|----------|----------------|---------|
| R-Shift  | U + I (7+8) | index + middle |
| R-Ctrl   | O + P (9+10) | ring + pinky |
| R-Alt    | M + , (33+34) | bottom row index + middle |
| R-GUI    | , + . (34+35) | bottom row middle + ring |

### Multi-Modifier Combinations

- **Same-hand Shift + Ctrl** (e.g., Ctrl+Shift+Z): hold all 4 home row keys (A+S+D+F on left, or U+I+O+P on right)
- **Cross-hand multi-mod** (e.g., Ctrl+Alt): combine one left and one right modifier combo (e.g., D+F + M+,)
- **Same-hand Shift+Alt / Ctrl+Alt etc.**: requires cross-hand (some same-hand modifier pairs share keys)

## Utility Combos

| Combo  | Keys (position) | Action | Notes |
|--------|----------------|--------|-------|
| Esc    | W + E (2+3) | `&kp ESC` | |
| Tab    | S + F (14+16) | `&kp TAB` | |
| Enter  | J + ; (19+22) | `&kp ENTER` | Also on right thumb (position 44) |
| Backspace | I + O (8+9) | `&bs_del` | Shift + this combo → Delete (mod-morph) |
| Insert | J + O (19+9) | `&kp INS` | |
| `'` (SQT) | K + ; (20+22) | `&kp SQT` | |
| Space  | G + B (17+29) | `&kp SPACE` | Also available on thumb |
| IME Toggle | E + R (3+4) | `&my_macro_zenhan` | Sends Alt + `` ` `` (Windows IME) |

### Delete via Mod-Morph

There is **no dedicated Delete combo**. Delete is obtained by holding a Shift combo while pressing the Backspace combo:

| Operation | Result |
|-----------|--------|
| I + O alone | Backspace |
| (A+S) + (I+O) | Delete (Shift + Backspace morphs to DEL) |
| (U+I) + (I+O) | Delete |

This pattern uses ZMK's `mod-morph` behavior defined as `bs_del` in `config/cornix.keymap`.

## Brackets, Braces, and Comparison Symbols

**These are NOT combos anymore** — they are now direct key bindings on the **Symbols layer** (top row and home row).

| Symbol | Symbols Layer Position | Source key |
|--------|------------------------|------------|
| `<`    | top row pos 1 | Q |
| `>`    | top row pos 2 | W |
| `[`    | top row pos 3 | E |
| `]`    | top row pos 4 | R |
| `{`    | top row pos 5 | T |
| `}`    | top row pos 6 | Y |
| `(`    | home row pos 21 | L (also Shift+9 = `(`) |
| `)`    | home row pos 22 | ; (also Shift+0 = `)`) |

To use: hold thumb `mo 1` (position 42) to activate Symbols, then press the corresponding key.

The previous vertical-pair combos (e.g., T+G for `{`, R+F for `(`) were removed because vertical chording felt uncomfortable during typing.

## Layer Activation Combos

These activate a layer momentarily, similar to a hold-tap layer key.

| Combo | Keys (position) | Layer | Timeout |
|-------|----------------|-------|---------|
| → Symbols (mo 1) | C + X (27+26) | Symbols | 60 ms |
| → Symbols (mo 1) | , + . (34+35) | Symbols | 60 ms |
| → Symbols (mo 1) | F + Z (16+25) | Symbols | 60 ms |
| → Mix (mo 2) | J + / (19+36) | Mix | 60 ms |

> Note: `, + .` overlaps with R-GUI combo. Resolution depends on press timing. Adjust positions if conflicts arise.

## Editing Notes

- **ZMK Studio** can view layouts and edit static keymaps, but **cannot edit behaviors** (`bs_del`, `hm`, layer-tap, etc.). Modifications to behaviors require editing `config/cornix.keymap` and rebuilding.
- Keymap Editor is **not recommended** — past versions silently rewrote combo positions on save, leading to inconsistent state.
