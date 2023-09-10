#include <LiquidCrystal_I2C.h> //Membuka libary lcd
#include <SPI.h> //
#include <MFRC522.h> //Library
#include <Servo.h> //membuka library servo

#define BUZZER 8 //buzzer pin
#define ACCESS_DELAY 500 // lamanya buzzer bunyi
#define DENIED_DELAY 2000 // lamanya buzzer bunyi
#define LED_Hijau 7 //define Pin Led Hijau
#define LED_Merah 6 //define Pin Led Merah
#define SERVO_PIN 5
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
Servo myservo;

void setup() 
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.noCursor(); //biar gak ada cursor di lcd

  lcd.clear(); //clear lcd
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin(); // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  Serial.println("Dekatkan kartu...");
  Serial.println();
  lcd.setCursor(0,0);
  lcd.print("Dekatkan Kartu");
  delay(1000);
  pinMode(LED_Hijau, OUTPUT);
  pinMode(LED_Merah, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  myservo.attach(SERVO_PIN);
  myservo.write(0);
}
void loop()
{
// Melihat kartu baru
  if ( ! mfrc522.PICC_IsNewCardPresent())
{
    return;
}
// Pilih salah satu kartu
  if ( ! mfrc522.PICC_ReadCardSerial())
{
  return;
}
// Menampilkan UID di serial monitor
  lcd.setCursor(0,0);
  lcd.print("UID :");
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
{
    lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    lcd.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
  Serial.println();

  Serial.print("Pesan : ");
  content.toUpperCase();
  if (content.substring(1) == "34 62 4E 49") // ganti kode kartu jika ingin mendapat akses
{
    myservo.write(90);
    digitalWrite(LED_Hijau, HIGH);
    delay(3000);
    tone(BUZZER, 3000);
    delay(ACCESS_DELAY);
    lcd.setCursor(0,1);
    lcd.print("ID Diterima ");
    Serial.println("ID Diterima ");
    digitalWrite(LED_Hijau, LOW);
    noTone(BUZZER);
    myservo.write(0);
}
  else {
    lcd.setCursor(0,1);
    lcd.print("ID Ditolak ");
    Serial.println(" ID Ditolak ");
    delay(1000);
    digitalWrite(LED_Merah, HIGH);
    tone(BUZZER, 1500); // Frekuensi suara pada buzzer
    delay(DENIED_DELAY);
    digitalWrite(LED_Merah, LOW);
    noTone(BUZZER);
}
}
