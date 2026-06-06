# zmk-config

ZMK firmware configuration for [Cornix LP](https://github.com/hitsmaxft/zmk-keyboard-cornix) split keyboard with rotary encoders.

[日本語版はこちら](README.ja.md)

## Design

This keymap follows a **combo-centric design** — no homerow mods (HM). All modifier keys (Shift, Ctrl, Alt, GUI) are accessed via combos, and the keyboard ships with **ZMK Studio** support for live editing over USB.

See [Combo Reference](docs/combos.md), [Configuration Reference](docs/configuration.md), and [Keymap Design Log](docs/keymap-design.ja.md) (Japanese, full design history).

## Keymap

![Cornix LP Keymap](docs/keymap.svg)

## Build

Firmware is built automatically via GitHub Actions on push.
Download `.uf2` files from the [latest Actions run](https://github.com/LevNas/zmk-config/actions).

### Flashing

1. Put the keyboard half into UF2 bootloader mode (double-tap reset button)
2. Copy `cornix_left.uf2` to the left half, `cornix_right.uf2` to the right half
3. If switching from another firmware, flash `reset.uf2` to both halves first to clear BT bonding

### ZMK Studio (Live Editing)

After flashing, connect the **left half** to your PC via USB and open <https://zmk.studio/> in Chrome or Edge to live-edit the keymap. Note: Studio cannot edit behavior definitions (mod-morphs, layer-taps, encoder mappings) — those require source edits and rebuild.

## Customization

Want to create your own keymap? Click **"Use this template"** on GitHub to get started. See the [Setup Guide](docs/fork-guide.md) for details.

## Hardware

- Board: Cornix LP (split, nRF52840)
- Rotary encoders: 2 (left: volume / right: page up-down on base layer; per-layer overrides)
- No dongle

## Credits

- [hitsmaxft](https://github.com/hitsmaxft) — [Cornix LP](https://github.com/hitsmaxft/zmk-keyboard-cornix) board design and ZMK board definition
- [urob](https://github.com/urob) — [zmk-helpers](https://github.com/urob/zmk-helpers) macros (used for DTS conciseness; HM is not adopted in this config)
- [ZMK Contributors](https://zmk.dev/) — ZMK firmware and ZMK Studio
