
#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,10,11,12,13);

int ph_pin = A2; // A2 -> Po

int m_4 = 623;
int m_7 = 605; //agua

void setup() {
  Serial.begin(9600);

  lcd.begin(16,2);
  lcd.setCursor(3,0);
  lcd.print("Electrotec");
  delay(2000);
  
  lcd.clear();
}

void loop() {
  
int measure = 0;
int prom = 0;

    for(int i=0;i<20;i++)
  {  measure = analogRead(ph_pin);
     prom = prom + measure;
     delay(50);
  }

  prom = prom/20;
  
  Serial.print("Medida: ");
  Serial.print(prom);

  //calibracion
  float Po = 7 + ((measure - m_7 ) * 3 / ( m_7 - m_4 ));
  Serial.print("\tPH: ");
  Serial.print(Po, 2);
  Serial.println("");

  lcd.setCursor(4,0);
  lcd.print("PH:");
  lcd.setCursor(7,0);
  lcd.print(Po,2);
  delay(100);
}
