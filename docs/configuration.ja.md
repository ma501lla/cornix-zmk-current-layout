# 設定リファレンス

このキーマップで使われている振る舞い (behavior)・マクロ・ファームウェア設定の詳細です。

## 設計思想

このキーマップは **コンボ中心設計**です。修飾キー (Shift / Ctrl / Alt / GUI) はホームロウ Mods ではなく**コンボ**で取得します。`behaviors` ブロックには HM 定義 (`hm_l` / `hm_r` / `hm_shift_l` / `hm_shift_r`) が残っていますが、これらは現状**どのレイヤーの bindings からも参照されていません** (歴史的経緯のため保存)。

コンボの配置と使い方は [コンボリファレンス](combos.ja.md)、設計判断の経緯は [キーマップ設計ログ](keymap-design.ja.md) を参照してください。

## 振る舞い (Behavior)

### Mod-Morph: `bs_del`

Shift が押されているかどうかで異なるキーコードを発射します。

```dts
bs_del: bs_del {
    compatible = "zmk,behavior-mod-morph";
    bindings = <&kp BSPC>, <&kp DEL>;
    mods = <(MOD_LSFT|MOD_RSFT)>;
};
```

- **タップ**: Backspace
- **Shift + タップ**: Delete

Backspace コンボ (`cb_kp_bs`, I + O) の bindings で使用しています。

### レイヤータップ: `lt`

ホールドでレイヤー起動、タップでキーコード送出。

```dts
&lt 5 ESCAPE    // タップ: ESC、ホールド: NumPad レイヤー
&lt 4 SPACE     // タップ: SPACE、ホールド: Navi レイヤー
&lt 1 BACKSPACE // タップ: BSPC、ホールド: Symbols レイヤー
```

### レイヤータップ (tap-preferred): `lt_g`

タップを優先する layer-tap。HM 撤廃後はどの bindings からも参照されていませんが、将来の利用に備えて定義は残しています。

### 未使用 HM 振る舞い (歴史的経緯)

以下はファイル内に定義されているが、コンボ中心設計への移行後は使用されていません。将来のクリーンアップで削除される可能性があります。

- `hm_l` / `hm_r` — 左右ホームロウ Mods
- `hm_shift_l` / `hm_shift_r` — Shift 専用 HM (短い tapping-term)
- `hm` — ポジショナル制約のない hold-tap

もし fork して HM を再有効化したい場合、これらの振る舞いを参照しつつ A/S/D/F や J/K/L/; のホームロウキーを書き換えてください。

## マクロ

### IME 切替: `my_macro_zenhan`

Alt + `` ` `` を発射して IME を ON/OFF します (Windows + US 配列向け)。

```dts
my_macro_zenhan: my_macro_zenhan {
    compatible = "zmk,behavior-macro";
    bindings =
        <&macro_press>, <&kp LEFT_ALT>,
        <&macro_tap>, <&kp GRAVE>,
        <&macro_release>, <&kp LEFT_ALT>;
};
```

E + R コンボで使用。

## レイヤー構成

現状は **7 レイヤー** (0–6) 構成：

| # | 名前 | 入口 | 用途 |
|---|------|------|------|
| 0 | Base | デフォルト | 文字入力、修飾キー・記号・編集系のコンボ |
| 1 | Symbols | 親指 `mo 1` (位置 42) またはレイヤー起動コンボ | 記号・数字・F キー |
| 2 | Mix | 親指 `mo 2` (位置 45) またはコンボ | F キー、数字行、編集系 |
| 3 | Adjust | Conditional (Symbols + Mix 同時押し) | BT 管理、システムコントロール |
| 4 | Navi | 親指 `lt 4 SPACE` ホールド (位置 43) | カーソル移動 |
| 5 | NumPad | 親指 `lt 5 ESCAPE` ホールド (位置 41) | テンキー |
| 6 | Debug | Navi/NumPad 内の `mo 6` | Bluetooth ペアリング、ブートローダー |

旧 Combo レイヤー (旧 index 6) は廃止済み。Debug は 7 → 6 に番号を繰り上げています。

## 条件付きレイヤー

Symbols (layer 1) と Mix (layer 2) が同時にアクティブな時、Adjust (layer 3) が自動的に起動します。

```dts
conditional_layers {
    adjust_layer {
        if-layers = <2 1>;
        then-layer = <3>;
    };
};
```

## ロータリーエンコーダ (Sensor Bindings)

各レイヤーで `sensor-bindings` を上書きできます。未定義のレイヤーは下位レイヤーへフォールバック。

| レイヤー | 左つまみ ↑/↓ | 右つまみ ↑/↓ |
|---------|-------------|-------------|
| Base    | 音量 Up / Down | Page Up / Down |
| Symbols | カーソル 右 / 左 | カーソル 下 / 上 |
| Mix     | Ctrl+Tab / Ctrl+Shift+Tab (ブラウザタブ切替) | 音量 Up / Down |
| Navi    | End / Home | Page Down / Up |
| NumPad  | KP_PLUS / KP_MINUS | KP_MULTIPLY / KP_DIVIDE |
| Adjust / Debug | (Base へフォールバック) | (Base へフォールバック) |

## ファームウェア設定 (`cornix.conf`)

| 設定 | 値 | 用途 |
|------|----|------|
| `CONFIG_BT_CTLR_TX_PWR_PLUS_8` | `y` | BT 送信出力を増加 (分割接続の安定化) |
| `CONFIG_ZMK_IDLE_SLEEP_TIMEOUT` | `1800000` | 30 分でスリープ (省電力) |
| `CONFIG_ZMK_KSCAN_DEBOUNCE_PRESS_MS` | `10` | キー押下デバウンス |
| `CONFIG_ZMK_KSCAN_DEBOUNCE_RELEASE_MS` | `10` | キーリリースデバウンス |
| `CONFIG_BT_PERIPHERAL_PREF_MIN_INT` | `6` | BT 接続インターバル最小 (7.5 ms) |
| `CONFIG_BT_PERIPHERAL_PREF_MAX_INT` | `12` | BT 接続インターバル最大 (15 ms) |
| `CONFIG_ZMK_STUDIO` | `y` | ZMK Studio 有効化 (USB 経由のライブ keymap 編集) |
| `CONFIG_ZMK_STUDIO_LOCKING` | `n` | Studio ロック無効 (個人利用) |
| `CONFIG_ZMK_USB_LOGGING` | `n` | USB ログ無効 |

## ZMK Studio 対応

このリポジトリは **ZMK Studio** 対応ファームウェアを Central (左半身) でビルドします。左半身を USB 接続し、Chrome または Edge で <https://zmk.studio/> を開くとライブで keymap を編集できます。

`build.yaml` に `studio-rpc-usb-uart` snippet を追加することで Studio 用 RPC 通信を有効化：

```yaml
include:
  - board: cornix_left//zmk
    snippet: studio-rpc-usb-uart
    artifact-name: cornix_left
```

**注意**: ZMK Studio は静的な keymap (どのキーがどのキーコードを発射するか) を編集できますが、**振る舞い定義 (`bs_del` / レイヤータップ / mod-morph / エンコーダのマッピング) は編集できません**。これらは `config/cornix.keymap` を直接編集して再ビルドする必要があります。
