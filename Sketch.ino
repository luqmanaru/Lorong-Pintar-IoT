// Pin untuk LED
const int led = 2;      // LED untuk lorong 1
const int led1 = 6;     // LED untuk lorong 2
const int led2 = 4;     // LED untuk lorong 3

// Pin untuk sensor
const int ldr = A5;     // Sensor LDR (cahaya)
const int ir = A0;      // Sensor IR (gerak) untuk lorong 1
const int ir1 = A1;     // Sensor IR (gerak) untuk lorong 2
const int ir2 = A2;     // Sensor IR (gerak) untuk lorong 3

// Variabel untuk menyimpan waktu LED dinyalakan
unsigned long ledOnTime[3] = {0, 0, 0};
bool ledState[3] = {false, false, false}; // Status LED: ON/OFF

// Threshold untuk sensor LDR (jika nilai < 250 dianggap gelap)
const int ldrThreshold = 250;

// Threshold untuk sensor IR (jika nilai < 300 dianggap ada gerakan)
const int irThreshold = 300;

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi pin LED sebagai output
  pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  // Inisialisasi pin sensor sebagai input
  pinMode(ldr, INPUT);
  pinMode(ir, INPUT);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  
  // Matikan semua LED di awal
  digitalWrite(led, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  
  Serial.println("Sistem Pencahayaan Lorong Pintar diaktifkan");
}

void loop() {
  // Baca nilai sensor LDR
  int ldrStatus = analogRead(ldr);
  Serial.print("Nilai LDR: ");
  Serial.println(ldrStatus);
  
  // Cek apakah kondisi gelap
  if (ldrStatus <= ldrThreshold) {
    // Cek sensor IR untuk setiap lorong
    checkIRSensor(0, ir, led);     // Lorong 1
    checkIRSensor(1, ir1, led1);    // Lorong 2
    checkIRSensor(2, ir2, led2);    // Lorong 3
  } else {
    // Jika terang, matikan semua LED
    turnOffAllLEDs();
  }
  
  // Cek apakah LED harus dimatikan setelah 4 detik
  checkLEDTimeout();
  
  delay(100); // Delay untuk stabilisasi
}

// Fungsi untuk mengecek sensor IR dan mengontrol LED
void checkIRSensor(int index, int irPin, int ledPin) {
  int irValue = analogRead(irPin);
  
  // Jika ada gerakan dan LED dalam keadaan mati
  if (irValue < irThreshold && !ledState[index]) {
    digitalWrite(ledPin, HIGH);      // Nyalakan LED
    ledOnTime[index] = millis();    // Simpan waktu nyala
    ledState[index] = true;         // Update status
    Serial.print("LED ");
    Serial.print(index + 1);
    Serial.println(" dinyalakan");
  }
}

// Fungsi untuk mematikan semua LED
void turnOffAllLEDs() {
  for (int i = 0; i < 3; i++) {
    if (ledState[i]) {
      // Matikan LED sesuai index
      if (i == 0) digitalWrite(led, LOW);
      else if (i == 1) digitalWrite(led1, LOW);
      else if (i == 2) digitalWrite(led2, LOW);
      
      ledState[i] = false;  // Update status
      Serial.print("LED ");
      Serial.print(i + 1);
      Serial.println(" dimatikan (kondisi terang)");
    }
  }
}

// Fungsi untuk mengecek timeout LED
void checkLEDTimeout() {
  for (int i = 0; i < 3; i++) {
    // Jika LED menyala dan sudah melewati 4 detik
    if (ledState[i] && (millis() - ledOnTime[i] >= 4000)) {
      // Matikan LED sesuai index
      if (i == 0) digitalWrite(led, LOW);
      else if (i == 1) digitalWrite(led1, LOW);
      else if (i == 2) digitalWrite(led2, LOW);
      
      ledState[i] = false;  // Update status
      Serial.print("LED ");
      Serial.print(i + 1);
      Serial.println(" dimatikan (timeout)");
    }
  }
}
