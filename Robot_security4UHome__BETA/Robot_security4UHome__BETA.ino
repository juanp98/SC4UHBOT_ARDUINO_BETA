
// 20 de mayo del 2016
#include <Ping.h>
#include <IRremote.h>
#include <Servo.h>
int RECV_PIN = 7;// pin PH_IR
IRrecv irrecv(RECV_PIN);
decode_results results;
//HC-SR04
int trig = 7;
int echo = 9;
int hc_tiemp;
//
int indicador_bt = 40;
int estado = 'h';
int led_encendido = 32;
int led_apagado = 30;
int reset = 38;
int eleccion_app_bt;
int cronotime = 0;
int test_mov;
int EN_BT = 26;
int sensor = 36; // sensor PING
Ping ultra = Ping (sensor);
int distancia;
int estadosens = 0;
int conteo;
int choca = 70;
int libre = 70;
int num_movement;
int tiempo = 0;
int s_movimiento1 = 22;
int led_movimiento = 11;
int estado_mov1;
int led_calibracion =  24;
int s_movimiento2 = 28;
int OUT_ON = 31;
int OUT_OFF = 33;
int TEST_LEDS = 35;
int estado_mov2;


// *********************
int adelante_izq = 9;
int atras_izq = 10;
// *******************
int adelante_der = 5;
int atras_der = 6;
// *******************
Servo vision;

int adelante = 82;
int medioizq =120;
int izquierdo = 170;
int medioder = 45;
int derecho = 0;

//****************** velocidad de motores
int despacio = 100;
int medio = 160;
int rapido = 200;



void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);
  pinMode(TEST_LEDS,OUTPUT);
  pinMode(OUT_ON,OUTPUT);
  pinMode(OUT_OFF,OUTPUT);
  pinMode(reset,OUTPUT);
  pinMode(led_encendido,OUTPUT);
  pinMode(led_apagado,OUTPUT);
  pinMode(EN_BT,OUTPUT);
  pinMode(s_movimiento2,INPUT);
  pinMode(led_calibracion,OUTPUT);
  pinMode(s_movimiento1,INPUT);
  pinMode(led_movimiento,OUTPUT);
  pinMode(adelante_izq,OUTPUT);
  pinMode(adelante_der,OUTPUT);
  pinMode(atras_der,OUTPUT);
  pinMode(atras_izq,OUTPUT);
  vision.attach(12);
  delay(300);
  vision.write(adelante); // calibracion de inicio del servo
  digitalWrite(EN_BT,HIGH);
  digitalWrite(indicador_bt,LOW);
  
}

void loop(){ // high = no pared   low = pared
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
  
  cronotime = 0; // reinicio del cronometro
 
if(Serial.available()>0){        // lee el bluetooth y almacena en estado
      estado = Serial.read();
      digitalWrite(indicador_bt,HIGH);
  }

  if(estado == 'a' || results.value == 3031751715){          
  Serial.println("Modo seguridad");
  eleccion_app_bt = 2;
  digitalWrite(led_encendido,HIGH);
  digitalWrite(led_apagado,LOW);
         
  }
  else if(estado == 'i' || results.value == 3031698675 ){         
    Serial.println("reiniciar");
    digitalWrite(reset,HIGH);  
    
  }
  else if(estado== 'c'){      
      Serial.println("ON_LED");
      digitalWrite(OUT_ON,HIGH);
      delay(500);
      digitalWrite(OUT_ON,LOW);    
  }
  else if(estado=='d'){         
       Serial.println("OFF_LED");
      digitalWrite(OUT_OFF,HIGH);
      delay(500);
      digitalWrite(OUT_OFF,LOW);
    
       
  } 
  
  else if(estado=='e' || results.value == 3031743555){    
      Serial.println("Modo Automatico (seguridad-limpieza)"); 
    
    
   
  }
  else if (estado =='f'){    
     Serial.println("Test movement"); 
     test_mov = 2;
     while(test_mov == 2){
     cronotime++;
     Serial.println(cronotime);
     if(cronotime == 3){
      test_mov = 0;
     }
     digitalWrite(adelante_der,rapido);
      digitalWrite(adelante_izq,rapido);  
      delay(500);
      digitalWrite(atras_der,rapido);
      digitalWrite(atras_izq,rapido);  
      delay(500);
      digitalWrite(adelante_izq,rapido);  
      delay(500);
      digitalWrite(adelante_der,rapido);  
      delay(500);
     }
  }
  
  else if  (estado=='g'){    
    Serial.println("GSM");  
    
  }

  else if(estado == 'b' || results.value == 3031710915){
    Serial.println("Modo limpieza");
  }
 
  
digitalWrite(led_apagado,HIGH);

eleccion_app_bt = 2; // Salto de eleccion en Android controller

while(eleccion_app_bt == 2){
                ////////////////---------------------------///////////////  Modo Seguridad ////
                
estado = Serial.read();                
digitalWrite(led_encendido,HIGH);
digitalWrite(led_apagado,LOW);
eleccion_app_bt = 2;
if(estado == 'i'){         
    Serial.println("detener");
    digitalWrite(reset,HIGH); 
    
}
delay(200);                 
analogWrite(led_movimiento,LOW);
/*ultra.fire(); // lectura del sensor PIR
distancia = ultra.centimeters();
Serial.println("leido");
Serial.println(tiempo);
*/

digitalWrite(trig,LOW); // lectura del sensor HC-SR04
delayMicroseconds(2);
digitalWrite(trig,HIGH);
delayMicroseconds(5);
//Conversion de tiempo de disparo en distancia (cm)
hc_tiemp = pulseIn(echo,HIGH);
distancia = int(0.01653*hc_tiemp);


if(tiempo == 3){ // calibracion del sensor de movimiento
 tiempo = 4;
 digitalWrite(led_calibracion,HIGH); 
 Serial.println("calibrando.....");
 analogWrite(adelante_izq,LOW);
 analogWrite(adelante_der,LOW);
 delay(500);
  digitalWrite(led_calibracion,LOW);
 delay(500);
 digitalWrite(led_calibracion,HIGH); 
 delay(8000);
  digitalWrite(led_calibracion,LOW);
 delay(500);
 digitalWrite(led_calibracion,HIGH); 
 Serial.println("calibrado");
 estado_mov2= digitalRead(s_movimiento2);
 estado_mov1 =  digitalRead(s_movimiento1);
 Read_movement();
 delay(500);
 digitalWrite(led_calibracion,LOW);
  }
  
 if(tiempo >= 8 && distancia > libre){ // contador para el movimiento aleatorio
 tiempo = 0;
 movement_random();
   
  }
else if (distancia > libre){
 analogWrite(adelante_izq,medio);
 analogWrite(adelante_der,medio);
 delay(500);
 tiempo++;
 }
               
else if(distancia < choca){ // ejecuta el movimiento del sensor de vision 
 tiempo = 0;
  analogWrite(atras_der,despacio);
  analogWrite(atras_izq,despacio);
  digitalWrite(adelante_izq,LOW);
  digitalWrite(adelante_der,LOW);
  analogWrite(atras_der,LOW);
  analogWrite(atras_izq,LOW);

  delay(300);

 vision.write(izquierdo); 
 delay(2000);
 
 //lectura_aprox();
 lectura_HCSR04();

 if (conteo == 1){
  vision.write(adelante);
  delay(2000);
  //***************************** instruccion motor derecho y atras (ejecuta si el lado izquierdo no hay pared)
  analogWrite(atras_der,medio);
  analogWrite(atras_izq,medio);
  delay(500);
  analogWrite(atras_der,LOW);
  analogWrite(atras_izq,LOW);
  delay(300);
  analogWrite(adelante_der,rapido);
  analogWrite(atras_izq,rapido);
  delay(900);
  analogWrite(adelante_der,LOW);
  analogWrite(atras_izq,LOW);
 }
else if (conteo == 2){
analogWrite(atras_der,despacio);
analogWrite(atras_izq,despacio);
digitalWrite(adelante_izq,LOW);
digitalWrite(adelante_der,LOW);  
analogWrite(atras_der,LOW);
 analogWrite(atras_izq,LOW);

delay(300);

vision.write(derecho);
delay(2000);

//lectura_aprox();
lectura_HCSR04();

if(conteo == 1){
vision.write(adelante);
delay(2000);
//******************************** intruccion motor izquierdo y atras (ejecuta si el lado derecho no hay pared)
  analogWrite(atras_der,medio);
  analogWrite(atras_izq,medio);
  delay(700);
  analogWrite(atras_izq,LOW);
  analogWrite(atras_der,LOW);
  delay(400);
  analogWrite(adelante_izq,rapido);
  analogWrite(atras_der,rapido);
  delay(900);
  analogWrite(adelante_izq,LOW);
  analogWrite(atras_der,LOW);
 }
 else if(conteo == 2){
  analogWrite(atras_der,despacio);
 analogWrite(atras_izq,despacio);
  digitalWrite(adelante_izq,LOW);
 digitalWrite(adelante_der,LOW);
 analogWrite(atras_der,LOW);
  analogWrite(atras_izq,LOW);

   delay(300);

   vision.write(adelante);
   delay(2000);
   //****************************** instruccion motor izq y der , hacia atras (ejecuta si en el lado izq y der hay pared)
   analogWrite(atras_der,medio);
  analogWrite(atras_izq,medio);
  delay(1200);
  analogWrite(atras_izq,LOW);
  analogWrite(atras_der,LOW);
  delay(300);
  analogWrite(adelante_izq,rapido);
  analogWrite(atras_der,rapido);
  delay(1800);
   analogWrite(adelante_izq,LOW);
  analogWrite(atras_der,LOW);
   }
  }
}
  if(estado== 'i'){         
    Serial.println("detener");
    delay(300);
    digitalWrite(reset,HIGH);
}
}
}


//******* FUNCIONES ********
void lectura_aprox(){
Serial.write(distancia); 
  conteo = 0;

ultra.fire();
distancia = ultra.centimeters();
delay(30);
  
   if (distancia > libre){
conteo = 1;
 }
else if(distancia < choca){
conteo = 2;
}
   
}


void movement_random(){
  num_movement = random(2);
  Serial.write("movimiento aleatorio activado");
 if(num_movement ==  1){
   Serial.write("movimiento 1"); 
  digitalWrite(adelante_izq,LOW);
  digitalWrite(adelante_der,LOW);
  delay(1000);
  analogWrite(atras_der,medio);
  analogWrite(atras_izq,medio);
  delay(700);
  analogWrite(atras_izq,LOW);
  analogWrite(atras_der,LOW);
  analogWrite(adelante_izq,rapido);
  analogWrite(atras_der,rapido);
  delay(900);
  analogWrite(adelante_izq,LOW);
  analogWrite(atras_der,LOW);
  
 } 
 if(num_movement == 2){
   Serial.write("movimiento 2");
   digitalWrite(adelante_izq,LOW);
  digitalWrite(adelante_der,LOW);
  delay(1000);
  analogWrite(atras_der,medio);
  analogWrite(atras_izq,medio);
  delay(700);
  analogWrite(atras_izq,LOW);
  analogWrite(atras_der,LOW);
  analogWrite(adelante_der,rapido);
  analogWrite(atras_izq,rapido);
  delay(900);
  analogWrite(adelante_der,LOW);
  analogWrite(atras_izq,LOW);
  
 }
 if(num_movement == 0){
   Serial.write("movimiento 3");
   digitalWrite(adelante_izq,LOW);
   digitalWrite(adelante_der,LOW);
   delay(1000);
   analogWrite(atras_izq,medio);
   analogWrite(atras_der,medio);
   delay(600);
   analogWrite(atras_der,LOW);
   analogWrite(atras_izq,LOW);
   delay(200);
   analogWrite(adelante_izq,rapido);
   analogWrite(atras_der,rapido);
   delay(1600);
   analogWrite(adelante_izq,LOW);
   analogWrite(atras_der,LOW);
  
 }
 
}

void Read_movement(){
  
 if(estado_mov1 == 1 || estado_mov2 == 1){  // instrucciones de Alarma
 analogWrite(led_movimiento,100);
 Serial.println("Alarma activada"); 
 
 
}
else{
  analogWrite(led_movimiento,LOW); 
 Serial.println("Alarma pasiva");
}  
}

void lectura_HCSR04(){
Serial.write(distancia);
conteo = 0;
  
digitalWrite(trig,LOW); // lectura del sensor HC-SR04
delayMicroseconds(2);
digitalWrite(trig,HIGH);
delayMicroseconds(5);
hc_tiemp = pulseIn(echo,HIGH);
distancia = int(0.01653*hc_tiemp);
delay(200);
if(distancia > libre){
  conteo = 1;
}
else if(distancia < choca){
  conteo = 2;
 }  
}



