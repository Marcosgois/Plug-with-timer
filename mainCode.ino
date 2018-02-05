#include "Wire.h"
#include <LiquidCrystal.h>
#define DS1307_ADDRESS 0x68

LiquidCrystal lcd(12, 11, 5, 4, 3, 6);
byte zero = 0x00;

void setup()
{
  lcd.begin(16, 2);
  Wire.begin();
  pinMode(A3, OUTPUT);
  analogWrite(A3, 76);
  Serial.begin(9600);
  //A linha abaixo pode ser retirada apos setar a data e hora
  //SelecionaDataeHora();
}

void loop()
{
  int i;
  for (i = 0; i <= 14; i++) {
    lcd.clear();
    Mostrarelogio(0);
    delay(1000);
  }
  for (i = 0; i <= 3; i++) {
    lcd.clear();
    Mostrarelogio(1);
    delay(1000);
  }
}

void SelecionaDataeHora()   //Seta a data e a hora do DS1307
{
  byte segundos = 40; //Valores de 0 a 59
  byte minutos = 59; //Valores de 0 a 59
  byte horas = 23; //Valores de 0 a 23
  byte diadasemana = 1; //Valores de 0 a 6 - 0=Domingo, 1 = Segunda, etc.
  byte diadomes = 17; //Valores de 1 a 31
  byte mes = 4; //Valores de 1 a 12
  byte ano = 13; //Valores de 0 a 99
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //Stop no CI para que o mesmo possa receber os dados

  //As linhas abaixo escrevem no CI os valores de
  //data e hora que foram colocados nas variaveis acima
  Wire.write(ConverteParaBCD(segundos));
  Wire.write(ConverteParaBCD(minutos));
  Wire.write(ConverteParaBCD(horas));
  Wire.write(ConverteParaBCD(diadasemana));
  Wire.write(ConverteParaBCD(diadomes));
  Wire.write(ConverteParaBCD(mes));
  Wire.write(ConverteParaBCD(ano));
  Wire.write(zero); //Start no CI
  Wire.endTransmission();
}

byte ConverteParaBCD(byte val) { //Converte o número de decimal para BCD
  return ( (val / 10 * 16) + (val % 10) );
}

byte ConverteparaDecimal(byte val)  { //Converte de BCD para decimal
  return ( (val / 16 * 10) + (val % 16) );
}

void Mostrarelogio(int input)
{
  int secondsToON = 0, minutsToON = 0, hoursToON = 0, secondsToOFF = 0, minutsToOFF = 0, hoursToOFF = 0;
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int segundos = ConverteparaDecimal(Wire.read());
  int minutos = ConverteparaDecimal(Wire.read());
  int horas = ConverteparaDecimal(Wire.read() & 0b111111);
  int diadasemana = ConverteparaDecimal(Wire.read());
  int diadomes = ConverteparaDecimal(Wire.read());
  int mes = ConverteparaDecimal(Wire.read());
  int ano = ConverteparaDecimal(Wire.read());
  //Mostra a data no Serial Monitor
  if (input == 0) {
    //lcd.print("Data:");
    lcd.setCursor(4, 0);
    if (diadomes < 10) {
      lcd.print("0");
      lcd.print(diadomes);
    }
    else lcd.print(diadomes);
    lcd.print("/");
    if (mes < 10) {
      lcd.print("0");
      lcd.print(mes);
    }
    else lcd.print(mes);
    lcd.print("/");
    lcd.print(ano);
    lcd.setCursor(4, 1);
    if (horas < 10) {
      lcd.print("0");
      lcd.print(horas);
    }
    else lcd.print(horas);
    lcd.print(":");
    if (minutos < 10) {
      lcd.print("0");
      lcd.print(minutos);
    }
    else lcd.print(minutos);
    lcd.print(":");
    if (segundos < 10) {
      lcd.print("0");
      lcd.print(segundos);
    }
    else lcd.print(segundos);
  } else {
    if (horas > 0 && horas < 7) {
      hoursToON = 7 - horas;
      minutsToON = 59 - minutos;
      secondsToON = 60 - segundos;
      lcd.setCursor(4, 0);
      lcd.print("LIGA EM:");
      lcd.setCursor(4, 1);
      if (hoursToON < 10) {
        lcd.print("0");
        lcd.print(hoursToON);
      }
      else lcd.print(hoursToON);
      lcd.print(":");
      if (minutsToON < 10) {
        lcd.print("0");
        lcd.print(minutsToON);
      }
      else lcd.print(minutsToON);
      lcd.print(":");
      if (secondsToON < 10) {
        lcd.print("0");
        lcd.print(secondsToON);
      }
      else lcd.print(secondsToON);
    }
    if (horas > 7 && horas < 18) {
      hoursToOFF = 17 - horas;
      minutsToOFF = 59 - minutos;
      secondsToOFF = 60 - segundos;
      lcd.setCursor(1, 0);
      lcd.print("DESLIGAMENTO:");
      lcd.setCursor(4, 1);
      if (hoursToOFF < 10) {
        lcd.print("0");
        lcd.print(hoursToOFF);
      }
      else lcd.print(hoursToOFF);
      lcd.print(":");
      if (minutsToOFF < 10) {
        lcd.print("0");
        lcd.print(minutsToOFF);
      }
      else lcd.print(minutsToOFF);
      lcd.print(":");
      if (secondsToOFF < 10) {
        lcd.print("0");
        lcd.print(secondsToOFF);
      }
      else lcd.print(secondsToOFF);
    }
    if (horas > 18 && horas < 24) {
      hoursToON = (horas - 23) + 6;
      minutsToON = 59 - minutos;
      secondsToON = 60 - segundos;
      lcd.setCursor(4, 0);
      lcd.print("LIGA EM:");
      lcd.setCursor(4, 1);
      if (hoursToON < 10) {
        lcd.print("0");
        lcd.print(hoursToON);
      }
      else lcd.print(hoursToON);
      lcd.print(":");
      if (minutsToON < 10) {
        lcd.print("0");
        lcd.print(minutsToON);
      }
      else lcd.print(minutsToON);
      lcd.print(":");
      if (secondsToON < 10) {
        lcd.print("0");
        lcd.print(secondsToON);
      }
      else lcd.print(secondsToON);
    }
  }
  lcd.setCursor(11, 0);
  switch (diadasemana)
  {
    case 0: lcd.println("DOM  ");
      break;
    case 1: lcd.println("SEG  ");
      break;
    case 2: lcd.println("TER  ");
      break;
    case 3: lcd.println("QUA  ");
      break;
    case 4: lcd.println("QUI  ");
      break;
    case 5: lcd.println("SEX  ");
      break;
    case 6: lcd.println("SAB  ");
  }
}
