#include "SoftwareSerial.h"

#include "TinyGPS.h"

// Constantes

#define rxPin 9    // Pin RX para el GPS
#define txPin 8    // Pin TX para el GPS
#define direccionNodo 0x1    // Valor de MY en XBee (ATMY)
#define idRed 0x3332    // Valor de ID en XBee (ATID)
#define canal 0xC    // Canal de comunicación XBee (ATCH)

int incomingByte;
boolean accelSleep;
boolean gpsActive;

// Objeto TinyGPS para decodificar los datos que llegan del GPS
TinyGPS gpsDecoder;

// Software Serial para usar los pines 8 y 9 como puerto serie
SoftwareSerial serialGPS(rxPin, txPin);

void setup()
{  
  // Configuración puerto Serie
  Serial.begin(9600); // 9600 bps
  accelSleep = HIGH;
  // Configuración del puerto serie GPS
  serialGPS.begin(4800);
  gpsActive = LOW;
  // Ponemos en alto la entrada ¬ SLEEP
  digitalWrite(4, accelSleep);
  // Para ganar precisión se usa una fuente de referencia externa (3.3V) 
  analogReference(EXTERNAL);
  pinMode(13,OUTPUT);

  // Apago el LED 13
  digitalWrite(13, LOW);
}

void loop()
{
  bool newData = false;

  // Variables para estadÃ­sticas
  unsigned long chars;
  unsigned short sentences, checksum; 

  float x,y,z;

  // Latitud y longitud
  float flat, flon;

  unsigned long age;

  if (Serial.available() > 0) {

    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    switch(incomingByte){
    case 'S':
      // Si recibo una S apago el acelerómetro y enciendo el LED
      accelSleep = LOW;
      digitalWrite(4, accelSleep);
      digitalWrite(13, HIGH);
      Serial.println('S');
      delay(1000);
      break;
    case 'W':
      // Si recibo una W enciendo el acelerómetro y apago el LED
      accelSleep = HIGH;
      digitalWrite(4, accelSleep);
      digitalWrite(13, LOW);
      Serial.println('W');
      delay(1000);
      break;
    case 'G':
      // Si recibo una G habilito la lectura del GPS
      gpsActive = HIGH;
      Serial.println('G');
      delay(1000);
      break;
    case 'D':
      // Si recibo una D deshabilito la lectura del GPS
      gpsActive = LOW;
      Serial.println('D');
      delay(1000);
      break;
    }
  }

  // Acelerómetro encendido: leo datos.
  if(accelSleep == HIGH){

    // Valor de x entre 0 y 1023
    x = analogRead(A5);
    // Valor de x entre 0 y 1023
    y = analogRead(A4);
    // Valor de z entre 0 y 1023
    z = analogRead(A3);

    // Cambio los valores a voltios
    x = x*3.3/1023;
    y = y*3.3/1023;
    z = z*3.3/1023;

    // Paso los valores a G
    x = (x - 0.85) * (2) / (2.45-0.85) -1;
    y = (y - 0.85) * (2) / (2.45-0.85) -1;
    z = (z - 0.60) * (2) / (2.21-0.60) -1;

  }

  if(gpsActive==HIGH){

    // Parseamos durante 50 milisegundos
    for (unsigned long start = millis(); millis() - start < 50;)
    {
      while (serialGPS.available())
      {
        // Lee datos enviados por el GPS y comprueba si son vÃ¡lidos
        char c = serialGPS.read();
        if (gpsDecoder.encode(c)){
          // Si son datos vÃ¡lidos ponemos a 'true' la variable newData
          newData = true;
        }
      }
    }
    if (newData)
    {

      // Recupero los datos de posiciÃ³n y altitud
      gpsDecoder.f_get_position(&flat, &flon, &age);

    }

  }

  // Imprimo los valores
  Serial.print("IT");
  // Datos de direccionamiento de XBee
  Serial.print("MY=");
  Serial.print(direccionNodo);
  Serial.print(",");
  Serial.print("ID=");
  Serial.print(idRed);
  Serial.print(",");
  Serial.print("CH=");
  Serial.print(canal);
  // Datos de acelerometro
  Serial.print(",x=");
  Serial.print(x);
  Serial.print(",y=");
  Serial.print(y); 
  Serial.print(",z=");
  Serial.print(z);
  // Datos de GPS
  Serial.print(",LAT=");
  Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
  Serial.print(",LON=");
  Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
  Serial.print(",SAT=");
  Serial.print(gpsDecoder.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gpsDecoder.satellites());
  Serial.print(",PREC=");
  Serial.print(gpsDecoder.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gpsDecoder.hdop());
  // Fin de trama
  Serial.println("FT");
}