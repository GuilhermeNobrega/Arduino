#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Inicializa o display no endereço padrão (geralmente 0x20 ou 0x27)
Adafruit_LiquidCrystal lcd(0);

// Pinos do projeto
const byte PIN_SENSOR = A0;
const byte PIN_LED = 7;
const int LIMIAR = 30; // % abaixo do qual o LED acende (necessário irrigar)

// Calibração do sensor (ajuste conforme os valores reais)
const int RAW_SECO    = 0;     // leitura com solo seco (menor valor)
const int RAW_MOLHADO = 1023;  // leitura com solo molhado (maior valor)

void setup() {
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600); // Para depuração e calibração

  lcd.begin(16, 2);   // LCD 16x2
  lcd.setBacklight(HIGH);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Sensor Umidade");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  delay(1500);
}

void loop() {
  int raw = analogRead(PIN_SENSOR);

  // Converte leitura bruta (0–1023) em porcentagem direta (0 seco, 100 molhado)
  int umidade = map(raw, RAW_SECO, RAW_MOLHADO, 0, 100);
  umidade = constrain(umidade, 0, 100);

  // Exibe valor no monitor serial (para calibração)
  Serial.print("Leitura bruta: ");
  Serial.print(raw);
  Serial.print("  -> Umidade: ");
  Serial.print(umidade);
  Serial.println("%");

  // Atualiza display LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  if (umidade < 10) lcd.print(" ");
  lcd.print(umidade);
  lcd.print("%");

  lcd.setCursor(0, 1);
  if (umidade < LIMIAR) {
    digitalWrite(PIN_LED, HIGH);
    lcd.print("Nec. irrigar!");
  } else {
    digitalWrite(PIN_LED, LOW);
    lcd.print("OK: sem irrig.");
  }

  delay(700);
}
