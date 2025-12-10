#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- PINOS ---
const int trigPin = 3;
const int echoPin = 2;
const int buzzerPin = 7;
const int bombaPin = 9;  // <-- bomba aqui

// --- VARIÁVEIS ---
long duracao;
int distancia;
int distanciaCritica = 12; // cm

void setup() {
  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Iniciando...");
  delay(1500);
  lcd.clear();

  // Sensor e atuadores
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(bombaPin, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(bombaPin, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Sistema VNV OK");
  delay(1500);
  lcd.clear();
}

void loop() {
  // --- MEDIÇÃO ULTRASSÔNICA ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH);
  distancia = duracao * 0.034 / 2;

  // --- EXIBE DISTÂNCIA ---
  lcd.setCursor(0, 0);
  lcd.print("Distancia: ");
  lcd.print(distancia);
  lcd.print("cm   ");

  // ---------------------------
  //      CONTROLE DA BOMBA
  // ---------------------------

  if (distancia > 0 && distancia <= distanciaCritica) {
    // *** NÍVEL CRÍTICO - PARAR BOMBA ***
    digitalWrite(bombaPin, LOW); // desliga bomba
    tone(buzzerPin, 1000);

    lcd.setCursor(0, 1);
    lcd.print("Nivel Alto! ");
  } 
  else if (distancia > distanciaCritica) {
    // *** ENCHENDO ***
    digitalWrite(bombaPin, HIGH); // liga bomba
    noTone(buzzerPin);

    lcd.setCursor(0, 1);
    lcd.print("Enchendo...     ");
  } 
  else {
    lcd.setCursor(0, 1);
    lcd.print("Sem leitura     ");
  }

  delay(500);
}
