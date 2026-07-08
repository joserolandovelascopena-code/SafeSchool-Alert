// C++
//


// Configuración del HARDWARE
// Pines utilizados,variables globales de estados y temporalizadores 

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
const int UMBRAL_SEGURO = 500;

char buffer[TAM_BUFFER];
int indice = 0;

bool estadoHumo = false;
bool yaCambioASeguro = false;

// Enum de estados de resivimiento de comandos de la app
enum estadoRecepcion {
  RECIBIENDO,
  MENSAJE_COMPLETO,
  ERROR_BUFFER,
};

//Enum de estados del sistema, se encarga de asignar los estados del sistema
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

// Emun de los tipos de emergencia, que el sistema puede detectar 
// administrar
enum tipoEmergencia{
  TE_NINGUNA,
  INCENDIO,
  MEDICA,
  SEGURIDAD,
};

// Emun de encanrgado de las ubicaciónes del instituto
enum zonaEmergencia {
  ZE_NINGUNA,
  EDIF_A,
  EDIF_B,
  EDIF_C,
};

//Variables globales de emuns para usar en el loop y funciones del sistema.
estadoRecepcion estado;
estadoEmergencia esEmerg;
tipoEmergencia tipoEmerg;
zonaEmergencia zona;


bool tonoAlto = true;
bool estadoBuzzer = false;

void setup() {
  //Configuración de pines de componestes, pines de pantalla LCD y iniciar serial
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

  delay(2500);
  lcd.clear();
}


// Loop BUCLE PRINCIPAL
// Ejecuta cada función que realiza una responsabilidad específica (MÓDULOS DEL SISTEMA)
void loop() {
  sensorHumo();
  btnsEmergencias();
  leerSerial();
  mensajeCompleto();
  estadoSeguroSystem();
  sirena();
  encenderLEDs();
  delay(300);
}

// Lógica del sensor de Humo, para dectar si hay un incendio automaticamente
void sensorHumo(){
  int humo = analogRead(sensor);
  
  if(humo >= UMBRAL_SEGURO && esEmerg == SEGURO){
    esEmerg = EMERGENCIA_ACTIVA;
    tipoEmerg = INCENDIO;
    mensajeText("Emerg: Incendio","Edificio A: 1C");
  }
  else if(humo < UMBRAL_SEGURO && tipoEmerg == INCENDIO &&
   esEmerg == EMERGENCIA_ACTIVA){
   tipoEmerg = TE_NINGUNA;
   esEmerg = INCENDIO_ATENDIDA;
   mensajeText("Emerg: Incendio","Ha sido Atendida");
   initCronometroSeguro();
  }
}

/* Modulo de Pulsadores, en cangarado de activar una emergencial al dar clic en un pulsador*/
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

/* RECEPCIÓN DE DATOS
   Buffer donde se almacena el comando recibido desde la App hasta 
   detectar el carácter de fin '#' */
        
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

//Detecta cuándo el mensaje fue resivido completamente, al recibir "#"

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

// Interpreta el comando recibido desde la aplicación
// y actualiza la máquina de estados del sistema.
void descifrarEstados(String comando) {
  if(esEmerg != SEGURO){
    if (comando == "Medica,EnProceso" && tipoEmerg == MEDICA) {
      esEmerg = MEDICA_PROCESO;
      actualizarEstados();
      Serial.println(F("Medica en proceso\n"));
    } 
    else if (comando == "Medica,Atendida" && tipoEmerg == MEDICA) {
      esEmerg = MEDICA_ATENDIDA;
      initCronometroSeguro();
      actualizarEstados();
      Serial.println(F("Medica Atendida\n"));
    } 
    else if (comando == "Medica,Falsa" && tipoEmerg == MEDICA) {
      esEmerg = MEDICA_FALSA;
      initCronometroSeguro();
      actualizarEstados();
      Serial.println(F("Medica Falsa Alarma\n"));
    } 
    else if (comando == "Seguridad,EnProceso" && tipoEmerg == SEGURIDAD) {
      esEmerg = SEGURIDAD_PROCESO;
      actualizarEstados();
      Serial.println(F("Seguridad en proceso\n"));
    } 
    else if (comando == "Seguridad,Atendida" && tipoEmerg == SEGURIDAD) {
      esEmerg = SEGURIDAD_ATENDIDA;
      initCronometroSeguro();
      actualizarEstados();
      Serial.println(F("Seguridad Atendida\n"));
    } 
    else if (comando == "Seguridad,Falsa" && tipoEmerg == SEGURIDAD) {
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

// Inicia el temporizador que devolverá el sistema
// automáticamente al estado SEGURO.
void actualizarEstados(){
 if(esEmerg != SEGURO){
   actualizarLCD();
  }
}

// Control central de alarmas.
// Dependiendo del estado actual de la emergencia, activa LEDs,
// buzzer y demás indicadores.

void sirena(){

  switch (esEmerg){
    case EMERGENCIA_ACTIVA:
      sirenaContinuaAsync(400, 900, 200);
      break;

    case MEDICA_PROCESO:
      sirenaDiscontinuaAsync(900, 500, 300);
      break;

    case SEGURIDAD_PROCESO:
      sirenaDiscontinuaAsync(900, 500, 500);
      break;

    default:
      noTone(buzzer);
      break;
  }
}

//BUZZER
//Lógica para crear melodias del Buzzer Continua o Descontinua

void sirenaContinuaAsync(int frec1, int frec2, int intervalo){
  
 if (millis() - ultimoCambioSirenaCT >= intervalo){
   ultimoCambioSirenaCT = millis();
   tonoAlto = !tonoAlto;

   if (tonoAlto){ 
     tone(buzzer, frec1);
   }
   else{
   tone(buzzer, frec2);
   }
 }
  
}

void sirenaDiscontinuaAsync(int frecuencia, int duracionTono, int tiempoApagado){
 unsigned long tiempoActual = millis();
 unsigned long intervaloActual = estadoBuzzer ? duracionTono : tiempoApagado;

   if (tiempoActual - ultimoCambioSirenaDT >= intervaloActual) {
     ultimoCambioSirenaDT = tiempoActual;
     estadoBuzzer = !estadoBuzzer;

     if (estadoBuzzer) {
       tone(buzzer, frecuencia);
     } else {
       noTone(buzzer);
     }
  }
}

// Actualiza la información mostrada en la pantalla LCD
// según el estado actual del sistema
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

//LEDS INDICADORES
//Función que se encarga de activar y desactivar LEDs indicadores de estados
//Verifica continuamente, si a tambiado un estado de una emergencia para activar LEDs
// utilizando Milli();
void encenderLEDs(){
   switch (esEmerg){
    case SEGURO:
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Amari, LOW);
     digitalWrite(led_Verde, LOW);
     digitalWrite(led_Azul, LOW);  
    break;
    
    case EMERGENCIA_ACTIVA:
     if(millis() - ultimoCambioLedRoj >= 100){
       estadoLedRoj = !estadoLedRoj;
       digitalWrite(led_Rojo, estadoLedRoj);
       ultimoCambioLedRoj = millis();
     }
     digitalWrite(led_Amari, LOW);
     digitalWrite(led_Verde, LOW);
     digitalWrite(led_Azul, LOW);  
    break;
    
    case INCENDIO_ATENDIDA:
     if(millis() - ultimoCambioLedVer >= 100){
       estadoLedVer = !estadoLedVer;
       digitalWrite(led_Verde, estadoLedVer);
       ultimoCambioLedVer = millis();
     }
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Amari, LOW);
     digitalWrite(led_Azul, LOW);
    break;
     
    case MEDICA_PROCESO:
     if(millis() - ultimoCambioLedAma >= 100){
       estadoLedAma = !estadoLedAma;
       digitalWrite(led_Amari, estadoLedAma);
       ultimoCambioLedAma = millis();
     }
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Verde, LOW);
     digitalWrite(led_Azul, LOW);
    break;
    
    case MEDICA_ATENDIDA:
     if(millis() - ultimoCambioLedVer >= 100){
       estadoLedVer = !estadoLedVer;
       digitalWrite(led_Verde, estadoLedVer);
       ultimoCambioLedVer = millis();
     }
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Amari, LOW);
     digitalWrite(led_Azul, LOW);
    break;
    
    case MEDICA_FALSA:
     if(millis() - ultimoCambioLedAzu >= 100){
       estadoLedAzu = !estadoLedAzu;
       digitalWrite(led_Azul, estadoLedAzu);
       ultimoCambioLedAzu = millis();
     }
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Amari, LOW);
     digitalWrite(led_Verde, LOW);
    break;
    
    case SEGURIDAD_PROCESO:
     if(millis() - ultimoCambioLedAma >= 100){
       estadoLedAma = !estadoLedAma;
       digitalWrite(led_Amari, estadoLedAma);
       ultimoCambioLedAma = millis();
     }
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Verde, LOW);
     digitalWrite(led_Azul, LOW);
    break;
    
    case SEGURIDAD_ATENDIDA:
     if(millis() - ultimoCambioLedVer >= 100){
       estadoLedVer = !estadoLedVer;
       digitalWrite(led_Verde, estadoLedVer);
       ultimoCambioLedVer = millis();
     }
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Amari, LOW);
     digitalWrite(led_Azul, LOW);
    break;
    
    case SEGURIDAD_FALSA:
     if(millis() - ultimoCambioLedAzu >= 100){
       estadoLedAzu = !estadoLedAzu;
       digitalWrite(led_Azul, estadoLedAzu);
       ultimoCambioLedAzu = millis();
     }
     digitalWrite(led_Rojo, LOW);
     digitalWrite(led_Amari, LOW);
     digitalWrite(led_Verde, LOW);
    break;
  }
}


// Función para escribir mensajes
// de dos líneas en la pantalla LCD.

void mensajeText(String mensaje1, String mensaje2){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(mensaje1);
    lcd.setCursor(0, 1);
    lcd.print(mensaje2);
}

//Función encargada de inicar el contador para regresar a estado SEGURO, al pasar medio segundo.


void initCronometroSeguro(){
   ultimoCambioSeguro = millis();
   yaCambioASeguro = true;
}

//Función que detecta cuando es hora de activar el Estado Seguro del Sistema
void estadoSeguroSystem(){
  
 if(yaCambioASeguro && millis() - ultimoCambioSeguro >= 1000){
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
