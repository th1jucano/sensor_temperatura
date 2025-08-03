#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define sensor A0
#define ledR 11
#define ledG 10
#define ledB 9
#define pot A1

float temperatura = 0;
float offsetreal = -2;
int temparredondada = 0;

bool estadoLed = true;

unsigned long tempoAnterior = 0;
const unsigned long intervalo = 5000; // 5 segundos

//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // inicia lcd

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  byte exclamacao[] = {
  B10001,
  B11011,
  B01110,
  B00100,
  B11011,
  B00000,
  B01110,
  B10001
  };

  byte zeros[] = {
  B01110,
  B01110,
  B00000,
  B01110,
  B01110,
  B00000,
  B01110,
  B01110
};

  pinMode(sensor, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  lcd.init();
  lcd.setBacklight(1);
  lcd.createChar(1,exclamacao);
  lcd.createChar(2,zeros);

}

int lerPot() {

  int valorPot = analogRead(pot);
  valorPot = map(valorPot, 0, 1023, 0, 255); // mapeamento analógico para digital
  return valorPot;

}



int lertemperatura() {

  int leitura = analogRead(sensor);
  float tensao = leitura * (5.0 / 1024.0); // ajusta a voltagem à condição de temperatura, evita variações bruscas comuns a este tipo de sensor
  temperatura = tensao * 100 + offsetreal; // um grau celsiu = 10mV = 0.01v
  temparredondada = round(temperatura);
  return temparredondada;

}

void mostratemperatura() {

  lcd.setCursor(0, 0);
  //lcd.print("****  ");
  lcd.write(1);
  lcd.write(2);
  lcd.write(1);
  lcd.write(2);
  //lcd.write(1);
  lcd.print("  ");
  lcd.print(temparredondada);
  lcd.print((char)223);
  lcd.print("C  ");
  lcd.write(2);
  lcd.write(1);
  lcd.write(2);
  lcd.write(1);
  lcd.write(2);

  String mensagem;

  if (temparredondada <= 20) {
    mensagem = "Hipotermia";
  } else if (temparredondada <= 30) {
    mensagem = "Suave na nave";
  } else {
    mensagem = "Frigideira";
  }

  lcd.setCursor(0, 1);
  lcd.print("                ");  // limpa a linha
  int posicao = (16 - mensagem.length()) / 2;
  lcd.setCursor(posicao, 1);
  lcd.print(mensagem);
}

void red(int intensidade) {
  analogWrite(ledR, intensidade);
  analogWrite(ledG, 0);
  analogWrite(ledB, 0);

}

void green(int intensidade) {
  analogWrite(ledR, 0);
  analogWrite(ledG, intensidade);
  analogWrite(ledB, 0);

}

void blue(int intensidade) {
  analogWrite(ledR, 0);
  analogWrite(ledG, 0);
  analogWrite(ledB, intensidade);

}

void apagarLEDs() {

  analogWrite(ledR, 0);
  analogWrite(ledG, 0);
  analogWrite(ledB, 0);

}

void loop() {
  
  unsigned long tempoAtual = millis();
  int intensidade = lerPot();

  // Atualiza temperatura e LCD a cada 5 segundos
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    int t = lertemperatura();
    mostratemperatura();

    if (estadoLed) {
      if (t <= 21) {
        blue(intensidade);
      } else if (t <= 30) {
        green(intensidade);
      } else {
        red(intensidade);
      }
    } else {
      apagarLEDs();
    }
  }

  // Mesmo fora do intervalo de 5s, permite ajuste de intensidade em tempo real
  if (estadoLed) {
    if (temparredondada <= 20) {
      blue(intensidade);
    } else if (temparredondada <= 30) {
      green(intensidade);
    } else {
      red(intensidade);
    }
  } else {
    apagarLEDs();
  }
}