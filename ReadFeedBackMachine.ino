#include <M5StickC.h>

const int sensorPin = 36; // Grove温度センサーを接続するADCピン

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3); // ディスプレイの回転設定
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE);
}

void loop() {
  int sensorValue = analogRead(sensorPin); // 温度センサーからのアナログ値を読み取る
  float voltage = sensorValue * (3.3 / 4095.0); // 電圧に変換（3.3V参照、12ビットADC）
  
  // 温度センサーのデータシートに基づいて温度を計算
  float temperatureC = (voltage - 0.5) * 100.0; // 例：TMP36センサーの場合
  
  // ディスプレイに表示
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.printf("Temperature:\n%.2f C", temperatureC);
  
  delay(1000); // 1秒ごとに更新
}
