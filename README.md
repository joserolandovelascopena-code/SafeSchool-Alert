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

| Tecnología       | Descripción                       |
| ---------------- | --------------------------------- |
| Arduino Uno      | Microcontrolador principal        |
| C++              | Programación del sistema embebido |
| MIT App Inventor | Desarrollo de la aplicación móvil |
| Tinkercad        | Simulación del circuito           |
| Git              | Control de versiones              |
| GitHub           | Gestión del repositorio           |

---

## Protocolo de Comunicación

Todos los mensajes enviados entre la aplicación y Arduino siguen un formato establecido. Si la aplicación envía un comando que no cumple con el protocolo de comunicación, el Arduino ignorará todos los comandos enviados que no cumplan el formato establecido y mostrará un error de tipo `[COMMAND ERROR]` en serial. De igual manera, el sistema solo permite comandos de un máximo de 30 caracteres; si el comando sobrepasa el límite de caracteres, el Arduino ignorará el comando y mostrará un error de tipo `[COMMAND ERROR]` en serial.

### Comandos de comunicación:

#### 1. Cambiar estado de emergencia Médica a Atendida:

```bash
Medica,Atendida#
```

#### 2. Cambiar estado de emergencia Médica a En Proceso:

```bash
Medica,EnProceso#
```

#### 3. Cambiar estado de emergencia Médica a Falsa Alarma:

```bash
Medica,Falsa#
```

#### 4. Cambiar estado de emergencia de Seguridad a Atendida:

```bash
Seguridad,Atendida#
```

#### 5. Cambiar estado de emergencia de Seguridad a En Proceso:

```bash
Seguridad,EnProceso#
```

#### 6. Cambiar estado de emergencia de Seguridad a Falsa Alarma:

```bash
Seguridad,Falsa#
```

> [!IMPORTANT]
> Si los comandos enviados no llevan al final "#", el sistema no sabrá dónde termina el comando y se quedará esperando a que llegue "#". Siempre al final debe ir un hashtag.

## Link del proyecto en Tinkercad

Si se desea simular el sistema desde un entorno virtual, el proyecto esta disponible en la plataforma de Tinkercad. Solo tienes que ingresar al siguiente Link.

- [SafeSchool Alert](https://www.tinkercad.com/things/fP5tqtgbbGX-001-sistema-safeschool-alert-version-final/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard&sharecode=V_xCmYJnCPcHeqlrGn7uIIG3u9PSOKJf7-YNFGOO0YM)

> [!TIP]
> Para simular una emergencia por incendio en Tinkercad, de clic en en el sensor de Gas, de color blanco, y traslade la nube de uno al sensor y se activara la alarma automaticamente.

## Instalación y Configuración

Para poder instalar el proyecto de forma local desde una máquina con conexión a Internet, ejecuta las siguientes líneas de comandos en PowerShell o Git Bash:

### 1. Prerrequisitos

Asegúrate de tener instalado lo siguiente en tu ordenador:

- [Git](https://git-scm.com)
- [Arduino IDE](https://docs.arduino.cc/software/ide/)

### 2. Clonar el repositorio

Luego, clona este repositorio en tu máquina local usando la terminal:

```bash
git clone https://github.com/joserolandovelascopena-code/SafeSchool-Alert.git
```

## Integrantes

### José Rolando Velasco Peña

### Luis Mario Meléndez Escobar

### Ulises de Jesus Mercado Alberto
