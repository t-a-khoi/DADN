#include "DHT20.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_SCL 22
#define I2C_SDA 21
#define SOIL_MOISTURE_PIN 33  // Chân analog cho cảm biến độ ẩm đất
#define LIGHT_SENSOR_PIN 27  // Chân analog cho cảm biến ánh sáng

DHT20 DHT(&Wire);  // Sử dụng bus I2C mặc định
LiquidCrystal_I2C lcd(0x21, 16, 2);  // Địa chỉ I2C của LCD có thể khác, kiểm tra datasheet của bạn

unsigned long previousMillis = 0;  // Lưu trữ thời gian trước đó
const long interval = 2000;        // Khoảng thời gian giữa các lần đọc (2 giây)

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("DHT20 LIBRARY VERSION: ");
  Serial.println(DHT20_LIB_VERSION);
  Serial.println();

  Wire.begin(I2C_SDA, I2C_SCL);  // Khởi tạo bus I2C với các chân SDA và SCL
  lcd.init();                    // Khởi tạo LCD
  lcd.backlight();               // Bật đèn nền LCD

  Serial.println("Type,\tStatus,\tHumidity (%),\tTemperature (C),\tSoil Moisture,\tLight Intensity");
  lcd.setCursor(0, 0);
  lcd.print("DHT20 Sensor");
}

void readDHT20()
{
  Serial.print("DHT20, \t");
  int status = DHT.read();
  switch (status)
  {
  case DHT20_OK:
    Serial.print("OK,\t");
    break;
  case DHT20_ERROR_CHECKSUM:
    Serial.print("Checksum error,\t");
    break;
  case DHT20_ERROR_CONNECT:
    Serial.print("Connect error,\t");
    break;
  case DHT20_MISSING_BYTES:
    Serial.print("Missing bytes,\t");
    break;
  case DHT20_ERROR_BYTES_ALL_ZERO:
    Serial.print("All bytes read zero");
    break;
  case DHT20_ERROR_READ_TIMEOUT:
    Serial.print("Read time out");
    break;
  case DHT20_ERROR_LASTREAD:
    Serial.print("Error read too fast");
    break;
  default:
    Serial.print("Unknown error,\t");
    break;
  }

  float humidity = DHT.getHumidity();
  float temperature = DHT.getTemperature();

  Serial.print(humidity, 1);
  Serial.print(",\t");
  Serial.print(temperature, 1);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 1);
  lcd.print("% T:");
  lcd.print(temperature, 1);
  lcd.print("C");
}

void readSoilMoisture()
{
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  Serial.print(",\t");
  Serial.print(soilMoistureValue);
}

void readLightIntensity()
{
  int lightIntensityValue = analogRead(LIGHT_SENSOR_PIN);
  Serial.print(",\t");
  Serial.println(lightIntensityValue);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    readDHT20();
    readSoilMoisture();
    readLightIntensity();
  }
}