//KODE BY ROBI
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4  // Reset pin (gunakan -1 jika tidak ada pin reset)

Adafruit_SH1106 display(OLED_RESET); // Menggunakan SH1106

// Pin input analog
int pinDioda = A1;       // Pin untuk pengukuran dioda
int pinVoltmeter = A3;   // Pin untuk pengukuran voltmeter
int pinBuzzer = 10;      // Pin untuk buzzer

void setup() {
  pinMode(pinDioda, INPUT);
  pinMode(pinVoltmeter, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW); // Pastikan buzzer mati saat awal
  
  // Inisialisasi OLED
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  
  // Menampilkan startup message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 15);
  display.println("M21 ELEKTRONIKA");
  display.drawLine(0, 28, 127, 28, WHITE);
  display.display();
  delay(3000);  // Menunggu selama 3 detik untuk startup
  display.clearDisplay();
}

void loop() {
  // --- Bagian Dioda ---
  int okunan = analogRead(pinDioda); // Membaca nilai dari pinDioda
  float R1 = 999.0; // Resistansi tetap
  float VT = 10.0;  // Tegangan input
  float V1 = okunan * VT / 1023.0; // Tegangan keluaran
  float R2 = (R1 * (VT - V1)) / V1; // Resistansi dioda

  // Kontrol buzzer
  if (R2 == 0.00) {  // Hanya bunyi ketika DHFE = 00.00
    tone(pinBuzzer, 1000); // Suara 1000 Hz
    delay(100);
    tone(pinBuzzer, 500);  // Suara 800 Hz
    delay(100);
    noTone(pinBuzzer);     // Matikan buzzer setelah irama selesai
  } else {
    digitalWrite(pinBuzzer, HIGH);  // Kondisi selain R2 == 0.00 buzzer menyala
  }
  
  // --- Bagian Voltmeter ---
  int value = analogRead(pinVoltmeter);
  float vout = (value * 5.0) / 1024.0; // Tegangan keluaran
  float R1_volt = 1000000.0; // Resistansi R1 pada pembagi tegangan
  float R2_volt = 100000.0;  // Resistansi R2 pada pembagi tegangan
  float vin = vout / (R2_volt / (R1_volt + R2_volt)); // Tegangan input
  
  if (vin < 0.09) {
    vin = 0.0; // Hilangkan noise
  }
  
  // --- Menampilkan pada OLED ---
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setCursor(25, 0);
  display.println("VOLTAGE");
  
  display.setTextSize(1);
  display.setCursor(20, 18);
  display.print(vin, 2);
  display.print("V");
  
  display.drawLine(0, 28, 127, 28, WHITE);
  
  display.setTextSize(2);
  display.setCursor(20, 35);
  display.println("TR DIODA");
  
  display.setTextSize(1);
  display.setCursor(20, 55);
  display.print("DHFE =");
  display.setCursor(60, 55);
  display.print(R2, 2);
  
  display.display();

  delay(200);
}
