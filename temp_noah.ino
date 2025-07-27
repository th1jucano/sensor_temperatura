#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LED_RGB.h"

const int numLeituras = 5;
int leituras[numLeituras];
int indiceLeitura = 0;
bool bufferCheio = false;

// Cria objeto LED RGB com os pinos R, G, B
LED_RGB led(11, 10, 9);

// Define os pinos do sensor e potenciômetro
#define sensor A0
#define pot A1

// Variáveis para temperatura
float temperatura = 0;
float offsetreal = -2;
int temparredondada = 0;

// Timer para atualização periódica
unsigned long tempoAnterior = 0;
const unsigned long intervalo = 1000; // 5 segundos

// Inicializa o LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  pinMode(sensor, INPUT);
  lcd.begin(16, 2);
  lcd.setBacklight(1);
}

int lerPot() {
  int valorPot = analogRead(pot);
  valorPot = map(valorPot, 0, 1023, 0, 255); // Mapeia para faixa de PWM
  return valorPot;
}

int lertemperatura() {
  analogRead(sensor);            // Primeira leitura descartada
  delayMicroseconds(10);         // Estabiliza o ADC
  int leitura = analogRead(sensor);  // Leitura real do sensor

  float tensao = leitura * (5.0 / 1024.0);  // Converte leitura para tensão
  temperatura = tensao * 100 + offsetreal; // Converte tensão para °C
  temparredondada = round(temperatura);
  return temparredondada;
}

void mostratemperatura() {
  lcd.setCursor(0, 0);
  lcd.print("****  ");
  lcd.print(temparredondada);
  lcd.print((char)223);
  lcd.print("C  ****");

  String mensagem;

  if (temparredondada <= 21) {
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

void loop() {
  unsigned long tempoAtual = millis();
  int intensidade = lerPot();

  // Atualiza temperatura e LCD a cada 1 segundo
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    leituras[indiceLeitura] = lertemperatura();
    indiceLeitura++;
    //mostratemperatura();
    if (indiceLeitura >= numLeituras){
      indiceLeitura = 0;
      bufferCheio = true;
    }
    
    if (bufferCheio){
      int soma = 0;
      for (int i = 0; i < numLeituras; i++){
        soma += leituras[i];
    }

      temparredondada = round((float)soma / numLeituras);
      mostratemperatura();
      bufferCheio = false;  // Zera o buffer
  }

    if (temparredondada <= 21) {
      led.blue(intensidade);
    } else if (temparredondada <= 30) {
      led.green(intensidade);
    } else {
      led.red(intensidade);
    }
  }

  // Permite ajuste de intensidade em tempo real
  if (temparredondada <= 21) {
    led.blue(intensidade);
  } else if (temparredondada <= 30) {
    led.green(intensidade);
  } else {
    led.red(intensidade);
  }
}
