# キーマップ設計判断ログ

`config/cornix.keymap` の構成と、その背後にある設計判断の記録。

## 設計目標 (現行)

1. **打鍵リズムを乱さない** — ホームロウ Mods (HM) の「ブレーキ感」を排除
2. **修飾キーは左右両手から打てる** — Shift / Ctrl / Alt / GUI
3. **外周列に依存しない** — 小指外側・親指外側を捨てた 36-key 有効レイアウト
4. **エンコーダはレイヤーごとに振る舞いを変える**
5. **ZMK Studio でライブ編集**できるようにする (build variant として組み込み)

## 設計の歴史と現在地

### Phase 1: filterpaper 流 HM 採用 (一時的、廃止)

最初の試みは [filterpaper/zmk-config](https://github.com/filterpaper/zmk-config) を参考に「H は素キー、JKL/SQT は HM」という非対称 HM 配置を採用：

- `tapping-term-ms = 225` / `require-prior-idle-ms = 105` (filterpaper 流)
- `hm_shift_l/r` は `require-prior-idle-ms = 0` で即時 Shift 化 (Townk 流)
- `hold-trigger-on-release` で同手 mod 併用を許可

**結果**: 打鍵リズム不適合により**左右両手で頻発する誤発火**が発生。リモート操作中に Win / Alt 系ショートカットが暴発し Wi-Fi 設定 OFF からのリモート切断事故に至る。`require-prior-idle-ms` を緩めても根本解決せず。

### Phase 2: HM 全撤廃 (採用中)

`A/S/D/F` と `J/K/L/;` を素キーへ戻し、修飾キーへのアクセスをすべて再設計。

調査で判明した事実：

- ZMK コミュニティの主流派は **urob 流 timeless HM** (`tapping-term 280ms` 等、より緩い設定) を採用しているが、本質的な機構 (`hold-trigger-on-release` + `balanced` flavor) は filterpaper と同じ。**ブレーキ感の解決にはならない**
- HM 撤廃派は 3 系統:
  - **Callum 流** (sticky-on-layer): [dxmh/zmk-config](https://github.com/dxmh/zmk-config), [braindefender/wellum](https://github.com/braindefender/wellum)
  - **Combo 置換**: [alex-tdrn/zmk-config](https://github.com/alex-tdrn/zmk-config), [casuanoob/zmk-config-bkb](https://github.com/casuanoob/zmk-config-bkb)
  - **anachron 流** (combo 多用 + thumb sticky shift + sticky-on-layer): [theol0403/anachron-zmk-config](https://github.com/theol0403/anachron-zmk-config)

### Phase 3: 完全コンボ主義 (採用)

調査結果と本人の打鍵スタイルから「**修飾キーを完全に combo に集約する**」方針へ確定。

設計上のポイント：

- 修飾コンボの bindings は **`&kp LXXX` (直接キープレス)** を使用 (`&sk` ではなく)
  - 押し続けで修飾保持 (連続キー操作対応)
  - 素早く 2 回タップで IDE の Ctrl-Ctrl 等のダブルタップ検出
  - 複数コンボ同時保持で多重修飾の sustained ホールド
- **mod-morph による DEL アクセス**: Backspace コンボ (`bs_del`) に Shift コンボを重ねると Delete が発射される。Delete 専用コンボは設けない

### Phase 4: G→Navi 復活 + 括弧の Symbols 集約 + J+; Enter (現行 2026-05-29)

実機検証を経て、以下の調整を実施:

- **G キーの layer-tap 復活**: `lt_g 4 G` (tap=G / hold=Navi) を main ブランチ準拠の `tapping-term=250 / require-prior-idle-ms=70` で再導入。HM 撤廃後も唯一「操作感が良い」とユーザが評価した hold-tap で、Navi レイヤーへの主要な入口を担う
- **括弧の Symbols レイヤー集約**: `< > [ ] { } ( )` を Symbols レイヤーへ移動。これまで base 層の **vertical pair combo** (T+G で `{` 等) で取得していたが、垂直チョーディングの打鍵感が悪く撤去。Symbols 上段に `< > [ ] { }` の 6 個を新規配置 (`( )` は既に home 行末尾にあり)
- **J + ; → Enter コンボ追加**: Base 層に右手 home 行末端のコンボとして Enter を追加。既存の右親指 Enter (位置 44) に加えて home position から手を離さずに Enter が打てる代替経路
- **Symbols 下段の旧 Shift キー除去**: 旧 LEFT_SHIFT (位置 25) / RIGHT_SHIFT (位置 37) は combo Shift で代替されたため `&none` 化

## 物理レイアウト前提

Cornix LP (54 キーポジション + 左右ロータリーエンコーダ 2 個)。実機にはすべてのキーが存在するが、論理的には外周を `&none` に落として 36-key 相当として運用する。

`&none` 化したポジション:

| 位置 | 元の役割 | 代替手段 |
|------|---------|---------|
| 0    | ESC     | combo (3+2 = W+E) |
| 11   | `\`     | Symbols レイヤー |
| 12   | Tab + Ctrl HM | Tab combo (14+16 = S+F)、Ctrl combo (15+16 = D+F) |
| 23   | `'`     | combo (20+22 = K+;) |
| 24   | LSHIFT  | combo (13+14 = A+S) |
| 37   | RSHIFT  | combo (7+8 = U+I) |
| 38-40, 47-49 | 外側親指 | (元から `&none`) |

## 修飾コンボ配置

### 配置の制約

- **物理的に隣接していないキーペア**を優先 (1 つ飛ばし以上)
- **Vim 操作の hjkl と衝突しない** (右手は top row または bottom row を活用)
- **同手 Shift + Ctrl 同時保持**を成立させるため、両者が異なる 4 指を使う配置にする

### 左手

| 修飾 | 位置 | 指 | 備考 |
|------|------|-----|------|
| L-Shift | A + S (13+14) | 小指 + 薬指 | Ctrl と組合可 |
| L-Ctrl  | D + F (15+16) | 中指 + 人差し指 | Shift と組合可 |
| L-Alt   | A + D (13+15) | 小指 + 中指 | 単独のみ |
| L-GUI   | Z + X (25+26) | 底行 小指 + 薬指 | TAB コンボ (S+F) との衝突回避で底行へ |

### 右手

| 修飾 | 位置 | 指 | 備考 |
|------|------|-----|------|
| R-Shift | U + I (7+8) | 人差し指 + 中指 | top 行、hjkl 衝突なし |
| R-Ctrl  | O + P (9+10) | 薬指 + 小指 | top 行、BS (I+O) との衝突回避でずらし |
| R-Alt   | M + , (33+34) | 底行 人差し指 + 中指 | hjkl 衝突なし |
| R-GUI   | , + . (34+35) | 底行 中指 + 薬指 | 頻度低いので底行 |

## ユーティリティコンボ

| Combo  | 位置 | 動作 | 備考 |
|--------|------|------|------|
| Esc    | W + E (2+3) | `&kp ESC` | |
| Tab    | S + F (14+16) | `&kp TAB` | |
| Backspace | I + O (8+9) | `&bs_del` | Shift + これで Delete |
| Insert | J + O (19+9) | `&kp INS` | |
| `'`    | K + ; (20+22) | `&kp SQT` | |
| Space  | G + B (17+29) | `&kp SPACE` | 親指でも取得可 |
| IME 切替 | E + R (3+4) | `&my_macro_zenhan` | |

## レイヤー構成 (7 レイヤー)

| # | 名前 | 入口 | 用途 |
|---|------|------|------|
| 0 | Base | デフォルト | 文字入力 |
| 1 | Symbols (LOWER) | 親指 `mo 1` (位置 42) | 記号・数字・F-keys |
| 2 | Mix (RAISE) | 親指 `mo 2` (位置 45) | F-keys、数字行、編集系 |
| 3 | Adjust | conditional (1+2 同時押し) | BT / 電源 |
| 4 | Navi | 親指 `lt 4 SPACE` ホールド (位置 43) | カーソル移動 |
| 5 | NumPad | 親指 `lt 5 ESCAPE` ホールド (位置 41) | テンキー |
| 6 | Debug | Navi/NumPad 内の `mo 6` | デバッグ |

**廃止: 旧 Combo レイヤー (旧 index 6)** — sticky modifier combos は HM (今は撤廃) の代替として用意されていたが、現行の修飾コンボ設計で不要となった。Debug は 7 → 6 に番号を繰り上げ。

## エンコーダのレイヤー別マッピング

ZMK の `sensor-bindings` はレイヤー未定義時に下位レイヤーへフォールバックする。レイヤーごとに必要な操作だけ上書き。

| レイヤー | 左つまみ | 右つまみ |
|---------|---------|---------|
| Base    | 音量 (Vol Up/Dn) | ページ (PgUp/PgDn) |
| Symbols | カーソル左右 | カーソル上下 |
| Mix     | ブラウザタブ (Ctrl+Tab) | 音量 |
| Navi    | 行頭/行末 (End/Home) | ページ (PgDn/PgUp) |
| NumPad  | +/- | ×/÷ |
| Adjust / Debug | (Base へフォールバック) | (Base へフォールバック) |

## ZMK Studio との運用

`config/cornix.conf` で `CONFIG_ZMK_STUDIO=y` + `CONFIG_ZMK_STUDIO_LOCKING=n` を設定し、`build.yaml` の cornix_left に `studio-rpc-usb-uart` snippet を追加して **Central (左半身) のみ**で Studio 機能を有効化。

注意点：

- **Studio で編集できないもの**: `behaviors` ブロック (mod-morph / レイヤータップ / HM 定義など)、`conditional_layers`、`macros`、`sensor-bindings`、`.conf` 設定
- **Studio で編集できるもの**: 各レイヤーのキー割当、レイヤー名、一部のコンボ
- **Studio 編集の落とし穴**: Studio 編集後にコード側を変更してファーム焼き直しすると、Studio で詰めた配置が古いコード版で上書きされる。**コード側を Single Source of Truth** として運用するのが安全

## 参考事例

設計判断の参考になった ZMK 公開設定：

- [theol0403/anachron-zmk-config](https://github.com/theol0403/anachron-zmk-config) — HM 撤廃 + ~100 combo + mod-morph の集大成
- [dxmh/zmk-config](https://github.com/dxmh/zmk-config) — Callum 流 sticky-on-layer の参照実装
- [braindefender/wellum](https://github.com/braindefender/wellum) — Callum を ZMK に素直に移植
- [alex-tdrn/zmk-config](https://github.com/alex-tdrn/zmk-config) — 「層を減らす」combo 派の代表
- [SethMilliken/zmk-config](https://github.com/SethMilliken/zmk-config) — 30+ combo の物理配置設計
- [urob/zmk-config](https://github.com/urob/zmk-config) — timeless HM (本構成では HM そのものは不採用、helpers ライブラリのみ利用)
- [filterpaper/zmk-config](https://github.com/filterpaper/zmk-config) — Phase 1 で参考にした非対称 HM (実装は撤廃)

## 撤退した設計判断 (記録)

- **filterpaper 流 HM**: 打鍵リズム不適合で撤退
- **「H だけ素キー」非対称 HM**: H が素キーになっている設計の名残はあるが、現状は H 含めて全文字キーが素キー
- **Combo レイヤー (旧 index 6)**: sticky modifier combo の専用入口だったが、新コンボ設計で不要となり廃止
- **Shift 専用親指キー (anachron 流)**: 親指の役割が既に飽和しているため不採用、Shift もコンボに集約
