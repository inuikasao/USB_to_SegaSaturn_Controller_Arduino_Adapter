# USB → Sega Saturn Controller Adapter

汎用ロジックIC（TC74HC153）とArduinoを組み合わせた、USB HIDコントローラーをセガサターンで使うためのアダプターです。

## 概要

セガサターンのコントローラープロトコルをTC74HC153（4to1マルチプレクサ）に丸投げし、ArduinoはGPIOのHi/Lo切り替えだけを担う構造です。ICがSaturnのページスキャンタイミングを自動処理するため、マイコン側でタイミング制御をする必要がありません。

```
[USB HIDコントローラー]
        ↓ USB
[Arduino UNO + USB Host Shield]
        ↓ GPIO 13本
[TC74HC153 × 2]
        ↓
[Saturnコントローラーポート]
```

## ハードウェア

### 必要部品

| 部品 | 数量 |
|------|------|
| TC74HC153（または互換品） | 2 |
| ICソケット 16ピン | 2 |
| Arduino UNO R3（または互換品） | 1 |
| USB Host Shield 2.0 | 1 |
| Saturnコントローラー延長ケーブル（オスコネクタ取り出し用） | 1 |
| ユニバーサル基板 | 1 |
| セラミックコンデンサ 0.1μF | 2（各IC Vcc-GND間。動作が不安定な場合に追加） |

## ICの接続先

### U1（d0/d1担当）

| ICピン | 信号 | 接続先 |
|--------|------|--------|
| pin1 (1G̅) | イネーブル（負論理） | GND（常時ON固定） |
| pin2 (B) | S1 | Saturnケーブル 緑（pin5） |
| pin3 (1C3) | d0のページ3入力 | GND（0固定） |
| pin4 (1C2) | d0のページ2入力 | Arduino D4（UP） |
| pin5 (1C1) | d0のページ1入力 | Arduino D0（Y） |
| pin6 (1C0) | d0のページ0入力 | Arduino D2（Z） |
| pin7 (1Y) | d0出力 | Saturnケーブル 灰（pin3） |
| pin8 (GND) | GND | GND |
| pin9 (2Y) | d1出力 | Saturnケーブル 白（pin2） |
| pin10 (2C0) | d1のページ0入力 | Arduino D3（B） |
| pin11 (2C1) | d1のページ1入力 | Arduino D6（C） |
| pin12 (2C2) | d1のページ2入力 | Arduino D5（DOWN） |
| pin13 (2C3) | d1のページ3入力 | GND（0固定） |
| pin14 (A) | S0 | Saturnケーブル 青（pin4） |
| pin15 (2G̅) | イネーブル（負論理） | GND（常時ON固定） |
| pin16 (Vcc) | 電源 | 5V |

### U2（d2/d3担当）

| ICピン | 信号 | 接続先 |
|--------|------|--------|
| pin1 (1G̅) | イネーブル（負論理） | GND（常時ON固定） |
| pin2 (B) | S1 | Saturnケーブル 緑（pin5）← U1と共通バス |
| pin3 (1C3) | d2のページ3入力 | Vcc（1固定） |
| pin4 (1C2) | d2のページ2入力 | Arduino A0（LEFT） |
| pin5 (1C1) | d2のページ1入力 | Arduino A5（A） |
| pin6 (1C0) | d2のページ0入力 | Arduino D1（X） |
| pin7 (1Y) | d2出力 | Saturnケーブル 赤（pin8） |
| pin8 (GND) | GND | GND |
| pin9 (2Y) | d3出力 | Saturnケーブル 橙（pin7） |
| pin10 (2C0) | d3のページ0入力 | Arduino A1（R） |
| pin11 (2C1) | d3のページ1入力 | Arduino A2（START） |
| pin12 (2C2) | d3のページ2入力 | Arduino A3（RIGHT） |
| pin13 (2C3) | d3のページ3入力 | Arduino A4（L） |
| pin14 (A) | S0 | Saturnケーブル 青（pin4）← U1と共通バス |
| pin15 (2G̅) | イネーブル（負論理） | GND（常時ON固定） |
| pin16 (Vcc) | 電源 | 5V |

### Arduino GPIO割り当て

| Arduino | ICピン | ボタン |
|---------|--------|--------|
| D0 | U1 pin5 (1C1) | Y |
| D1 | U2 pin6 (1C0) | X |
| D2 | U1 pin6 (1C0) | Z |
| D3 | U1 pin10 (2C0) | B |
| D4 | U1 pin4 (1C2) | UP |
| D5 | U1 pin12 (2C2) | DOWN |
| D6 | U1 pin11 (2C1) | C |
| A0 | U2 pin4 (1C2) | LEFT |
| A1 | U2 pin10 (2C0) | R |
| A2 | U2 pin11 (2C1) | START |
| A3 | U2 pin12 (2C2) | RIGHT |
| A4 | U2 pin13 (2C3) | L |
| A5 | U2 pin5 (1C1) | A |

> **注意**: D0/D1はシリアル通信（RX/TX）と共用です。スケッチの書き込みやシリアルモニタを使用する場合は、D0/D1（Y/X）のジャンパー線をIC基板から抜いてください。


## 電源・ピンの競合

### 電源

**SaturnコントローラーポートのVcc（pin1、pin6）はArduinoのUSB給電と競合するため同時に接続しないでください。**
コントローラーポートの電源のみで安定動作しないときはVccを浮かして、USB給電で駆動させる形にしてください。

### 使用禁止ピン

USB Host Shieldと競合するため以下のピンはIC基板に使用できません。

| ピン | USB Host Shieldでの用途 |
|------|------------------------|
| D7 | MAX3421E RESET |
| D8 | MAX3421E GPX |
| D9 | INT |
| D10 | SS |
| D11 | MOSI |
| D12 | MISO |
| D13 | SCK（LEDにも使用禁止） |

### USB Host Shield互換品の注意点

互換品はVBUS PWRジャンパを **3.3V側にブリッジ** しないと動作しない場合があります（74HC125の問題）。またINTとSSのジャンパも明示的にブリッジが必要な製品があります。

| ジャンパ | 設定 |
|---------|------|
| VBUS PWR | 3.3V側をブリッジ（5V側ではない） |
| 5V | ブリッジ |
| 3.3V | ブリッジ |
| INT | D9にブリッジ |
| SS | D10にブリッジ |

## 参考資料

- [SEGA Saturn ControlPAD 等価回路図 by H.Kashima](https://kaele.com/~kashima/games/saturn.html)
- [Saturn Controller - GameSX](https://gamesx.com/controldata/saturn.htm)
- [Saturn Controller - Hardware Book](https://www.hardwarebook.info/Saturn_controller)
- [Saturn I/O Pinout - plutiedev](https://www.plutiedev.com/io-pinout)
- [USB Host Shield Library 2.0](https://github.com/felis/USB_Host_Shield_2.0)

## ライセンス

MIT License — 詳細は [LICENSE](LICENSE) を参照してください。
