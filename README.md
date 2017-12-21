# dakoku-board

これは勤怠の出社/退社をサーバーに送信するためのマイコンボードプログラムです。

## ハードウェア

- [ESPr Developer](https://www.switch-science.com/catalog/2500/)
- [FeliCa リーダー・ライター RC-S620S](https://www.switch-science.com/catalog/353/)
- [FeliCa Plug ピッチ変換基板](https://www.switch-science.com/catalog/1030/)
- [I2C接続の小型LCD搭載ボード(3.3V版)](https://www.switch-science.com/catalog/1405/)

## 開発環境

- Arduino IDE 1.6.12 以上
- Fritzing

ESPr Developer を Arduino IDE から利用する方法は、スイッチサイエンスの[ESP-WROOM-02開発ボードをArduino IDEで開発する方法](http://trac.switch-science.com/wiki/esp_dev_arduino_ide)を参照してください。

### EspSoftwareSerial ライブラリ

また、Felicaの接続に [EspSoftwareSerial](https://github.com/plerup/espsoftwareserial) を使っています。
上記でインストールされた ESPr ボードの中にも含まれているのですが、少し古いので新しいバージョンを利用します。

1. Githubのページ右上の「Clone or download」から「Download ZIP」を選んでZIPファイルをダウンロード
1. Arduino IDEを起動
1. 「スケッチ」メニューの「ライブラリをインクルード」→「.ZIP形式のライブラリをインストール…」でダウンロードしたZIPファイルを選択

また、このままだとライブラリがコンフリクトするので、ボードライブラリに同梱されている古いものを削除します。

```
$ rm -rf ~/Library/Arduino15/packages/esp8266/hardware/esp8266/2.3.0/libraries/SoftwareSerial
```

少しディレクトリ名が違うかもしれないので、コンパイル時にコンフリクトエラーが発生したディレクトリで `/esp8266/hardware/esp8266/2.3.0/libraries` の方を削除してください

### I2CLiquidCrystal ライブラリ

Arduinoでは `LiquidCrystal` というライブラリでLCDの制御ができますが、これはI2C接続に対応していないので [ストロベリー・リナックス/秋月電子のI2C液晶/OLEDほかをArduinoで使う](http://n.mtng.org/ele/arduino/i2c.html) というサイトで公開されている `I2CLiquidCrystal` ライブラリを利用します。

サイト下部の `ファイル` から最新版を取得し、EspSoftwareSerial と同様に「.ZIP形式のライブラリをインストール」を実行してください。

## 回路イメージ

![](dakoku_breadboard.png)

- 8x2 のLCD部品がなかったので、 16x2 の部品を使っています
- FeliCa Plug ピッチ変換基板 もなかったので、一般のFCCコネクタ部品を使っています

## 環境設定

`config.h.example` を `config.h` にコピーして、WIFIの設定を変更してください。

## TODO

WiFi環境でない場所でも使えるように、[Wio LTE](https://soracom.jp/products/wio_lte/) でも利用可能にする
