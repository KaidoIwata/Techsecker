#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "M5StickCPlus.h"

#define REPORTING_PERIOD_MS     3000

#define HIGHHEART 90
#define LOWHEART 50

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

uint16_t getColor(uint8_t red, uint8_t green, uint8_t blue){
  return ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);
}

void onBeatDetected()
{
    Serial.print(pox.getHeartRate(), 2);  // 小数点第2位までの出力を追加
    Serial.print("bpm\n");

    float heatvalue = pox.getHeartRate();
/*
    if(heatvalue > HIGHHEART){
      Serial.print("面白い\n");
    } else if(heatvalue < LOWHEART){
      Serial.print("面白くない");
    } else {
      Serial.print("まあまあ\n");
    }

*/
}

void setup()
{
    M5.begin();
    M5.Lcd.setRotation(3); // 画面向きを設定

    // ハートを表示
    M5.Lcd.fillCircle(30, 40, 10, RED);
    M5.Lcd.fillCircle(50, 40, 10, RED);
    M5.Lcd.fillCircle(40, 41, 3, RED);
    M5.Lcd.fillTriangle(22, 45, 58, 45, 40, 65, RED);

    // O2を表示
    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(20, 80);
    M5.Lcd.print("O");
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(48, 100);
    M5.Lcd.print("2");

    Serial.begin(115200);
    Serial.println("Initializing pulse oximeter...");

    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    pox.update();

    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        M5.Lcd.setTextSize(3);

        // 心拍数を表示
        M5.Lcd.setCursor(75, 40);
        M5.Lcd.print(pox.getHeartRate());

        // 血中酸素量を表示（小数点第2位まで）
        M5.Lcd.setCursor(75, 90);
        M5.Lcd.print(pox.getSpO2());

        tsLastReport = millis();
    }
}
