# Lorong-Pintar-IoT

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=arduino&logoColor=white)
![IoT](https://img.shields.io/badge/IoT-Project-blue)

Sistem pencahayaan otomatis untuk lorong berbasis IoT yang menggunakan sensor LDR dan sensor gerak untuk mengendalikan lampu LED. Dikembangkan sebagai proyek UAS mata kuliah Internet of Things (IoT) di Universitas Bina Insani.

## 🎯 Fitur Utama
- **Deteksi Cahaya Otomatis**: Menggunakan sensor LDR untuk mendeteksi kondisi gelap/terang
- **Deteksi Gerakan**: Menggunakan sensor IR untuk mendeteksi keberadaan orang di lorong
- **Pencahayaan Terarah**: Setiap lorong memiliki LED dan sensor gerak sendiri
- **Penghematan Energi**: Lampu hanya menyala saat ada gerakan dan kondisi gelap
- **Timeout Otomatis**: Lampu mati otomatis setelah 4 detik jika tidak ada gerakan

## 🛠️ Komponen yang Dibutuhkan
| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| Arduino Uno | 1 | Mikrokontroler utama |
| LED | 3 | Untuk pencahayaan lorong |
| Sensor LDR | 1 | Untuk deteksi cahaya |
| Sensor IR (Obstacle) | 3 | Untuk deteksi gerakan di setiap lorong |
| Resistor 220Ω | 3 | Pembatas arus untuk LED |
| Resistor 10KΩ | 1 | Untuk pembagi tegangan LDR |
| Kabel Jumper | 15 | Untuk koneksi |
| Breadboard | 1 | Tempat memasang komponen |

## 🔌 Skema Koneksi
| Komponen | Pin Arduino | Keterangan |
|----------|-------------|------------|
| LED Lorong 1 | Pin 2 | Digital Output |
| LED Lorong 2 | Pin 6 | Digital Output |
| LED Lorong 3 | Pin 4 | Digital Output |
| Sensor LDR | Pin A5 | Analog Input |
| Sensor IR Lorong 1 | Pin A0 | Analog Input |
| Sensor IR Lorong 2 | Pin A1 | Analog Input |
| Sensor IR Lorong 3 | Pin A2 | Analog Input |

**Catatan**: 
- Hubungkan kaki panjang LED ke pin Arduino melalui resistor 220Ω
- Hubungkan kaki pendek LED ke GND
- Sensor LDR terhubung dengan rangkaian pembagi tegangan menggunakan resistor 10KΩ
- Sensor IR terhubung langsung ke pin analog dengan VCC dan GND

## 📝 Kode Program
Kode program terdiri dari beberapa fungsi utama:

### Fungsi Setup
```cpp
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
```

### Fungsi Loop
```cpp
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
```

### Fungsi Pengecekan Sensor IR
```cpp
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
```

### Fungsi Timeout LED
```cpp
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
```

## Rangkaian
<img width="800" height="732" alt="image" src="https://github.com/user-attachments/assets/765c0d71-385d-4499-98c7-32279b6b651b" />

## 📊 Cara Kerja Sistem
| Kondisi | Aksi Sistem | Output |
|---------|-------------|--------|
| Cahaya Terang (LDR > 250) | Matikan semua LED | Semua LED mati |
| Cahaya Gelap (LDR ≤ 250) + Ada Gerakan (IR < 300) | Nyalakan LED yang sesuai | LED menyala |
| Cahaya Gelap (LDR ≤ 250) + Tidak Ada Gerakan (IR ≥ 300) | Tidak ada aksi | LED tetap mati |
| LED Menyala + 4 Detik Berlalu | Matikan LED | LED mati otomatis |

## 📹 Video Demo
Lihat video demonstrasi proyek di: [https://youtu.be/shaPSafIxrE](https://youtu.be/shaPSafIxrE)

## 👨‍💻 Tim Pengembang
Proyek ini dikembangkan oleh **Kelompok 5** sebagai proyek UAS mata kuliah Internet of Things:

| Nama | NIM | Peran |
|------|-----|------|
| Hanif Luqmanul Hakim | 2022310035 | Ketua, Presentasi & Video |
| Bagas Aditya | 2022310063 | Dokumentasi & Wireframe |
| Dedy Wijaya | 2022310052 | Programmer |
| Rayan Ikmal Amala | 2022310043 | Desain & Pemasangan |
| Muhammad Iqbal | 2022310089 | Simulasi & Testing |

---
**luqmanaru**

Proyek ini dikembangkan sebagai UAS mata kuliah Internet of Things. Sistem dirancang untuk meningkatkan efisiensi energi dan kenyamanan pengguna di area lorong.
