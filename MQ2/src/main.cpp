#include <MQUnifiedsensor.h>

#define BOARD "ESP32"
#define Voltage_Resolution 3.3  // ESP32 dùng điện áp ADC 3.3V
#define PIN 34  // Chân analog ESP32 để đọc dữ liệu từ MQ-2 (GPIO34)
#define TYPE "MQ-2"  
#define ADC_Bit_Resolution 12  // ESP32 có độ phân giải ADC 12-bit
#define RatioMQ2CleanAir 9.83  // Giá trị tỉ lệ trong không khí sạch

// Khai báo đối tượng cảm biến MQ-2
MQUnifiedsensor MQ2(BOARD, Voltage_Resolution, ADC_Bit_Resolution, PIN, TYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("Khởi động cảm biến MQ-2 trên ESP32...");

  // Thiết lập mô hình tính toán nồng độ khí (PPM)
  MQ2.setRegressionMethod(1);  // PPM = a * ratio^b
  MQ2.setA(605.18); MQ2.setB(-3.937);  // Thông số tính toán cho MQ-2

  // Khởi tạo cảm biến
  MQ2.init();

  // Tiến hành hiệu chuẩn cảm biến
  Serial.print("Đang hiệu chuẩn, vui lòng chờ...");
  float calcR0 = 0;
  for (int i = 0; i < 10; i++) {
    MQ2.update();  // Cập nhật dữ liệu
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
    delay(500);
  }
  MQ2.setR0(calcR0 / 10);  // Gán giá trị R0 trung bình
  Serial.println(" Hoàn tất!");

  // Kiểm tra lỗi kết nối
  if (isinf(calcR0)) {
    Serial.println("Lỗi: Kết nối cảm biến không hợp lệ, vui lòng kiểm tra dây nối!");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Lỗi: Cảm biến có vấn đề, kiểm tra lại dây và nguồn cấp!");
    while (1);
  }

  // Bật chế độ debug để xem dữ liệu chi tiết trên Serial Monitor
  MQ2.serialDebug(true);
}

void loop() {
  // Đọc dữ liệu từ cảm biến
  MQ2.update();
  float smokePPM = MQ2.readSensor();  // Đọc giá trị khí khói (PPM)

  // Hiển thị cảnh báo nếu nồng độ khói cao
  if (smokePPM > 1000) {
    Serial.println("⚠️ Cảnh báo: Nồng độ khói cao!");
  }

  // Xuất dữ liệu ra Serial Monitor
  MQ2.serialDebug();
  delay(1000); 
}
