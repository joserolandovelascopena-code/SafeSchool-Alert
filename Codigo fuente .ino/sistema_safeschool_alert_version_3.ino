// C++
//

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

int led_Rojo = 6;
int led_Amari = 5;
int led_Verde = 4;
int led_Azul = 3;

unsigned long ultimoCambioLedRoj = 0;
unsigned long ultimoCambioLedAma = 0;
unsigned long ultimoCambioLedVer = 0;
unsigned long ultimoCambioLedAzu = 0;
unsigned long ultimoCambioSeguro = 0;
unsigned long ultimoCambioSirenaCT = 0;
unsigned long ultimoCambioSirenaDT = 0;

bool estadoLedRoj = false;
bool estadoLedAma = false;
bool estadoLedVer = false;
bool estadoLedAzu = false;

int btn_Edi_A_Medica = 13;
int btn_Edi_A_Seguridad = A1;

int btn_Edi_B_Medica = A2;
int btn_Edi_B_Seguridad = A3;

int btn_Edi_C_Medica = A4;
int btn_Edi_C_Seguridad = A5;
 
int buzzer = 2;
int sensor = A0;

const byte TAM_BUFFER = 30;
const int UNBRAL_SEGURO = 500;

char buffer[TAM_BUFFER];
int indice = 0;

bool estadoHumo = false;
bool yaCambioASeguro = false;

enum estadoRecepcion {
  RECIBIENDO,
  MENSAJE_COMPLETO,
  ERROR_BUFFER,
};

enum estadoEmergencia{
  SEGURO,
  EMERGENCIA_ACTIVA,
  INCENDIO_ATENDIDA,
  MEDICA_PROCESO,
  MEDICA_ATENDIDA,
  MEDICA_FALSA,
  SEGURIDAD_PROCESO,
  SEGURIDAD_ATENDIDA,
  SEGURIDAD_FALSA,
};

enum tipoEmergencia{
  TE_NINGUNA,
  INCENDIO,
  MEDICA,
  SEGURIDAD,
};

enum zonaEmergencia {
  ZE_NINGUNA,
  EDIF_A,
  EDIF_B,
  EDIF_C,
};

estadoRecepcion estado;
estadoEmergencia esEmerg;
tipoEmergencia tipoEmerg;
zonaEmergencia zona;


bool tonoAlto = true;
bool estadoBuzzer = false;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  lcd.begin(16, 2);

  pinMode(led_Rojo, OUTPUT);
  pinMode(led_Amari, OUTPUT);
  pinMode(led_Verde, OUTPUT);
  pinMode(led_Azul, OUTPUT);
  
  pinMode(buzzer, OUTPUT);
  
  pinMode(btn_Edi_A_Medica, INPUT);
  pinMode(btn_Edi_A_Seguridad, INPUT);
  
  pinMode(btn_Edi_B_Medica, INPUT);
  pinMode(btn_Edi_B_Seguridad, INPUT);
  
  pinMode(btn_Edi_C_Medica, INPUT);
  pinMode(btn_Edi_C_Seguridad, INPUT);

  lcd.setCursor(0, 0);
  lcd.print(F("SafeSchool"));
  lcd.setCursor(0, 1);
  lcd.print(F("Sistema listo..."));

  delay(2000);
  lcd.clear();
}

void loop() {
  sensorHumo();
  btnsEmergencias();
  leerSerial();
  mensajeCompleto();
  estadoSeguroSystem();
  sirena();
}

void sensorHumo(){
  int humo = analogRead(sensor);
  
  if(humo >= UNBRAL_SEGURO && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = INCENDIO;
    mensajeText("Emerg: Incendio","Edificio A: 1C");
  }
  else if(humo < UNBRAL_SEGURO && tipoEmerg == INCENDIO &&
   esEmerg == EMERGENCIA_ACTIVA){
   tipoEmerg = TE_NINGUNA;
   esEmerg = INCENDIO_ATENDIDA;
   mensajeText("Emerg: Incendio","Ha sido Atendida");
   initCronometroSeguro();
  }
}

void btnsEmergencias(){
  int estadoBtnMe1 = digitalRead(btn_Edi_A_Medica);
  int estadoBtnSe1 = digitalRead(btn_Edi_A_Seguridad);
  int estadoBtnMe2 = digitalRead(btn_Edi_B_Medica);
  int estadoBtnSe2 = digitalRead(btn_Edi_B_Seguridad);
  int estadoBtnMe3 = digitalRead(btn_Edi_C_Medica);
  int estadoBtnSe3 = digitalRead(btn_Edi_C_Seguridad);
  
  if(estadoBtnMe1 == HIGH && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = MEDICA;
    zona = EDIF_A;
    actualizarLCD(); 
  }

  if(estadoBtnSe1 == HIGH && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = SEGURIDAD;
    zona = EDIF_A;
    actualizarLCD();
  }

  if(estadoBtnMe2 == HIGH && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = MEDICA;
    zona = EDIF_B;
    actualizarLCD();
  }

  if(estadoBtnSe2 == HIGH && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = SEGURIDAD;
    zona = EDIF_B;
    actualizarLCD();
  }

  if(estadoBtnMe3 == HIGH && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = MEDICA;
    zona = EDIF_C;
    actualizarLCD();
  }

  if(estadoBtnSe3 == HIGH && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = SEGURIDAD;
    zona = EDIF_C;
    actualizarLCD();
  }
}
        
void leerSerial() {
  
 while (Serial.available() > 0) {
   char recibido = Serial.read();
   
   switch(estado){
     case ERROR_BUFFER:
     
     if (recibido == '#') {
      buffer[0] = '\0';
      indice = 0;
      estado = RECIBIENDO;
     }
     
     break;   
     
     case RECIBIENDO:
     
     if (recibido == '#') {

      buffer[indice] = '\0';
      estado = MENSAJE_COMPLETO;
      return;
     }
     
     if (indice < TAM_BUFFER - 1) {

      buffer[indice] = recibido;
      indice++;
     }
     else {
      Serial.println(F("[COMMAND ERROR] El comando sobrepasa el limite permitido.\n"));
      estado = ERROR_BUFFER;
     }
     
     break;

   }
 
  }
}

void mensajeCompleto(){
  if(estado == MENSAJE_COMPLETO){ 
    enviarMensaje();
    indice = 0;
    estado = RECIBIENDO;
  }
}

void enviarMensaje() {
  Serial.println(F("[SUCCESS] Comunicacion con App establecida sin errores."));
  descifrarEstados(buffer);
}

void descifrarEstados(String comando) {
  if(esEmerg != SEGURO){
    if (comando == "Medica,EnProceso") {
      esEmerg = MEDICA_PROCESO;
      actualizarEstados();
      Serial.println(F("Medica en proceso\n"));
    } 
    else if (comando == "Medica,Atendida") {
      esEmerg = MEDICA_ATENDIDA;
      initCronometroSeguro();
      actualizarEstados();
      Serial.println(F("Medica Atendida\n"));
    } 
    else if (comando == "Medica,Falsa") {
      esEmerg = MEDICA_FALSA;
      initCronometroSeguro();
      actualizarEstados();
      Serial.println(F("Medica Falsa Alarma\n"));
    } 
    else if (comando == "Seguridad,EnProceso") {
      esEmerg = SEGURIDAD_PROCESO;
      actualizarEstados();
      Serial.println(F("Seguridad en proceso\n"));
    } 
    else if (comando == "Seguridad,Atendida") {
      esEmerg = SEGURIDAD_ATENDIDA;
      initCronometroSeguro();
      actualizarEstados();
      Serial.println(F("Seguridad Atendida\n"));
    } 
    else if (comando == "Seguridad,Falsa") {
      esEmerg = SEGURIDAD_FALSA;
      initCronometroSeguro();
      actualizarEstados();
      Serial.println(F("Seguridad Falsa Alarma\n"));
    }else{
      Serial.print(F("[COMMAND ERROR] El comando '"));
      Serial.print(comando);                             
      Serial.println(F("' no es reconocido.\n"));
      return;
    }
  }
  else{
    Serial.println(F("[STATUS ERROR] Accion denegada: El sistema se encuentra en estado SEGURO.\n"));
    return;
  }
}

void actualizarEstados(){
 if(esEmerg != SEGURO){
   actualizarLCD();
  }
}

void initCronometroSeguro(){
   ultimoCambioSeguro = millis();
   yaCambioASeguro = true;
}

void estadoSeguroSystem(){
  
 if(yaCambioASeguro && millis() - ultimoCambioSeguro >= 600){
    yaCambioASeguro = false;
   
    esEmerg = SEGURO;
    tipoEmerg = TE_NINGUNA;
    zona = ZE_NINGUNA;
    mensajeText("Estado Instituto","Seguro");
    digitalWrite(led_Rojo, LOW);
    digitalWrite(led_Amari, LOW);
    digitalWrite(led_Verde, LOW);
    digitalWrite(led_Azul, LOW);
    
    ultimoCambioSeguro = millis();   
 }
}

void sirena(){

  switch (esEmerg){
    case EMERGENCIA_ACTIVA:
    LED_Alarma_Activa();
    break;
    
    case INCENDIO_ATENDIDA:
    noTone(buzzer);
    LED_Estado_Atendida();
    break;
    
    case MEDICA_ATENDIDA:
    noTone(buzzer);
    LED_Estado_Atendida();
    break;
    
    case MEDICA_FALSA:
    noTone(buzzer);
    LED_Estado_falsaAlar();
    break;
    
    case SEGURIDAD_ATENDIDA:
    noTone(buzzer);
    LED_Estado_Atendida();
    break;
    
    case SEGURIDAD_FALSA:
    noTone(buzzer);
    LED_Estado_falsaAlar();
    break;
    
    case MEDICA_PROCESO:
    LED_Alarma_EnProgreso();
    break;

    case SEGURIDAD_PROCESO:
    LED_Alarma_EnProgreso();
    break;
    
    default:
        noTone(buzzer);
    break;
  }
}


void LED_Alarma_Activa(){
  digitalWrite(led_Rojo, HIGH);
  digitalWrite(led_Amari, LOW);
  digitalWrite(led_Verde, LOW);
  digitalWrite(led_Azul, LOW);
  for (int i = 0; i < 3; i++) {
    tone(buzzer, 300);
    delay(300);
    tone(buzzer, 900);
    delay(300);
  }
}

void LED_Alarma_EnProgreso(){
  digitalWrite(led_Amari, HIGH);
  digitalWrite(led_Rojo, LOW);
  digitalWrite(led_Verde, LOW);
  digitalWrite(led_Azul, LOW);
  for (int i = 0; i < 3; i++) {
   tone(buzzer, 900);
   delay(500);
   noTone(buzzer);
   delay(1000);
  }
}

void LED_Estado_Atendida(){
  digitalWrite(led_Verde, HIGH);
  digitalWrite(led_Rojo, LOW);
  digitalWrite(led_Amari, LOW);
  digitalWrite(led_Azul, LOW);
}

void LED_Estado_falsaAlar(){
  digitalWrite(led_Azul, HIGH);
  digitalWrite(led_Rojo, LOW);
  digitalWrite(led_Amari, LOW);
  digitalWrite(led_Verde, LOW);
}

void actualizarLCD(){
  
  if(esEmerg == EMERGENCIA_ACTIVA){

    if(zona == EDIF_A && tipoEmerg == MEDICA){
      mensajeText("Emerg: Medica","Edificio A: 1A");
    }

    else if(zona == EDIF_A && tipoEmerg == SEGURIDAD){ 
      mensajeText("Emerg: Seguridad","Edificio A: 1B");
    }

    else if(zona == EDIF_B && tipoEmerg == MEDICA){
      mensajeText("Emerg: Medica","Edificio B: 2C");
    }

    else if(zona == EDIF_B && tipoEmerg == SEGURIDAD){
      mensajeText("Emerg: Seguridad","Edificio B: 2A");
    }

    else if(zona == EDIF_C && tipoEmerg == MEDICA){
      mensajeText("Emerg: Medica","Edificio C: 1Sa");
    }

    else if(zona == EDIF_C && tipoEmerg == SEGURIDAD){
      mensajeText("Emerg: Seguridad","Edificio C: 3Sa");
    }
  }
 
  switch (esEmerg){
    
    case MEDICA_PROCESO:
    mensajeText("Emerg: Medica", "Esta en proceso");
    break;
    
    case MEDICA_ATENDIDA:
    mensajeText("Emerg: Medica", "Ha sido Atendida");
    break;
    
    case MEDICA_FALSA:
    mensajeText("Emerg: Medica", "Es falsa Alarma");
    break;
    
    case SEGURIDAD_PROCESO:
    mensajeText("Emerg: Seguridad", "Esta en proceso");
    break;
    
    case SEGURIDAD_ATENDIDA:
    mensajeText("Emerg: Seguridad", "Ha sido Atendida");
    break;
    
    case SEGURIDAD_FALSA:
    mensajeText("Emerg: Seguridad", "Es falsa Alarma");
    break;
  }
}

void mensajeText(String mensaje1, String mensaje2){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(mensaje1);
    lcd.setCursor(0, 1);
    lcd.print(mensaje2);
}


