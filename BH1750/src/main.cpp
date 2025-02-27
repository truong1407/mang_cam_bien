#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter(0x23);

void setup() {

  Serial.begin(9600);

  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {//chế độ đo liên tục với độ phân giải cao 1lx
    Serial.println(F("BH1750 Advanced begin"));//khởi tạo thành công
  } else {
    Serial.println(F("Error initialising BH1750"));// khởi tạo thất bại
  }
}

void loop() {
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();//đọc giá trị cảm biếnbiến
    Serial.print("Cường độ ánh sáng: ");
    Serial.print(lux);
    Serial.println(" lx");
    delay(1000);
  }
}