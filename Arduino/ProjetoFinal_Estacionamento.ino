#include <Ultrasonic.h>

//Vaga 1
#define pino_trigger_1 8
#define pino_echo_1 9
int led_disp_1 = 13;
int led_ocup_1 = 12;
Ultrasonic vaga1(pino_trigger_1, pino_echo_1);

//Vaga 2
#define pino_trigger_2 7
#define pino_echo_2 6
int led_disp_2 = 11;
int led_ocup_2 = 10;
Ultrasonic vaga2(pino_trigger_2, pino_echo_2);

//Vaga 3 - Deficiente/Idoso
#define pino_trigger_3 4
#define pino_echo_3 5
int led_disp_3 = 1;
int led_ocup_3 = 0;
Ultrasonic vaga3(pino_trigger_3, pino_echo_3);

//Entrada
int count_entrada = 0;
int entrada = 0;
int pin_entrada = A0;

//Saida
int count_saida = 0;
int saida = 0;
int pin_saida = A1;

void setup() {
  Serial.begin(9600);
  pinMode(led_disp_1, OUTPUT);
  pinMode(led_ocup_1, OUTPUT);
  pinMode(led_disp_2, OUTPUT);
  pinMode(led_ocup_2, OUTPUT);
  pinMode(led_disp_3, OUTPUT);
  pinMode(led_ocup_3, OUTPUT);
}

void loop() {
  //Le as informacoes do sensor de pressão
  //para registrar a entrada de veículos
  entrada = analogRead(pin_entrada);
  if (entrada > 100) {
      count_entrada++;
  }
  //para registrar a saída de veículos
  saida = analogRead(pin_saida);
  Serial.println(count_entrada);
  if (saida > 100) {
      count_saida++;
  }
  Serial.println(count_saida);
  
  
  //Le as informacoes dos sensores ultrasonicos em cm
  //para registrar se a vaga está disponivel ou ocupada
  float vg1cmMsec, vg2cmMsec, vg3cmMsec;
  long microsec1 = vaga1.timing();
  long microsec2 = vaga2.timing();
  long microsec3 = vaga3.timing();
  vg1cmMsec = vaga1.convert(microsec1, Ultrasonic::CM);
  vg2cmMsec = vaga2.convert(microsec2, Ultrasonic::CM);
  vg3cmMsec = vaga3.convert(microsec3, Ultrasonic::CM);
  
  // Verifica se está disponiviel ou ocupado e atualiza o status
  Serial.print("V1-");
  disponivel(vg1cmMsec, led_disp_1, led_ocup_1, "V1");
  ocupado(vg1cmMsec, led_disp_1, led_ocup_1, "V1");
  Serial.print("V2-");
  disponivel(vg2cmMsec, led_disp_2, led_ocup_2, "V2");
  ocupado(vg2cmMsec, led_disp_2, led_ocup_2, "V2");
  Serial.print("V3-");
  disponivel(vg3cmMsec, led_disp_3, led_ocup_3, "V3");
  ocupado(vg3cmMsec, led_disp_3, led_ocup_3, "V3");
 
  delay(500);

}

// Método que altera status da vaga para ocupado
void ocupado(int x, int disp, int ocup, String vaga){
   if (x <= 12){
      digitalWrite(disp, LOW);
      digitalWrite(ocup, HIGH);
      Serial.println("Ocupado");
   }
}

// Método que altera status da vaga para disponível
void disponivel(int x, int disp, int ocup, String vaga){
   if (x > 12){
      digitalWrite(ocup, LOW);
      digitalWrite(disp, HIGH);
      Serial.println("Disponivel");
   }
}
