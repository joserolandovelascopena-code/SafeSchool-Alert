# SafeSchool Alert

El sistema **SafeSchool Alert** es una plataforma de alertas escolares desarrollada con **MIT App Inventor** y el microcontrolador **Arduino Uno**. El sistema funciona como una máquina de estados para sistemas embebidos, controlada por una aplicación móvil que se encarga de enviar comandos que el Arduino interpreta para tomar decisiones y actualizar el estado de una emergencia.

## Funciones Principales del Sistema

- **Comunicación serial** por medio de comandos (_parsers_).
- **Detección automática** de incendios.
- **Activación de emergencias manuales** por medio de pulsadores.
- **Detección automática** de la ubicación y el tipo de emergencia.
- **Mensajes de estado** por medio de pantalla LCD y monitor serial.
- **Sirena continua** y sirena intermitente.
- **LEDs de estado** para las diferentes emergencias.
- **Comunicación y actualización mutua** entre la aplicación y Arduino.

## Tecnologías Utilizadas

- **Tinkercad**
- **C++**
- **MIT App Inventor**

## Instalación y Configuración

Para poder instalar el proyecto de forma local desde una máquina con conexión a Internet, ejecuta las siguientes líneas de comandos en PowerShell o Git Bash:

### 1. Prerrequisitos

Asegúrate de tener instalado lo siguiente en tu ordenador:

- [Git](https://git-scm.com)
- [Arduino IDE](https://docs.arduino.cc/software/ide/)

### 2. Clonar el repositorio

Primero, clona este repositorio en tu máquina local usando la terminal:

```bash
git clone [https://github.com/joserolandovelascopena-code/SafeSchool-Alert.git](https://github.com/joserolandovelascopena-code/SafeSchool-Alert.git)
```
