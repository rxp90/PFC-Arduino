// Valor de MY en XBee (ATMY)
#define direccionMando 0x1
// Valor de ID en XBee (ATID)
#define idRed 0x3332
// Canal de comunicación XBee (ATCH)
#define canal 0xC
// Dirección del receptor (ATDL)
#define direccionReceptor 0x2

int incomingByte;
boolean accelSleep;

void setup()
{  
  // Configuración puerto Serie
  Serial.begin(9600); // 9600 bps
  accelSleep = HIGH;
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

  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    switch(incomingByte){
    case 'S':
      // Si recibo una S apago el acelerómetro y enciendo el LED
      accelSleep = LOW;
      digitalWrite(4, accelSleep);
      digitalWrite(13, HIGH);
      Serial.println('OK');
      delay(1000);
      break;
    case 'W':
      // Si recibo una W enciendo el acelerómetro y apago el LED
      accelSleep = HIGH;
      digitalWrite(4, accelSleep);
      digitalWrite(13, LOW);
      Serial.println('OK');
      delay(1000);
      break;
    }
  }

  // Acelerómetro encendido: envío datos.
  if(accelSleep == HIGH){

    // Valor de x entre 0 y 1023
    float x = analogRead(A5);
    // Valor de x entre 0 y 1023
    float y = analogRead(A4);
    // Valor de z entre 0 y 1023
    float z = analogRead(A3);
    // Cambio los valores a voltios
    x = x*3.3/1023;
    y = y*3.3/1023;
    z = z*3.3/1023;
    // Paso los valores a G

    x = (x - 0.85) * (2) / (2.45-0.85) -1;
    y = (y - 0.85) * (2) / (2.45-0.85) -1;
    z = (z - 0.60) * (2) / (2.21-0.60) -1;

    // Imprimo los valores
    Serial.print("IT");
    // Datos de direccionamiento de XBee
    Serial.print("MY=");
    Serial.print(direccionMando);
    Serial.print(",");
    Serial.print("ID=");
    Serial.print(idRed);
    Serial.print(",");
    Serial.print("CH=");
    Serial.print(canal);
    Serial.print(",");
    Serial.print("DL=");
    Serial.print(direccionReceptor);
    // Datos de sensores
    Serial.print("x=");
    Serial.print(x);
    Serial.print(",");
    Serial.print("y=");
    Serial.print(y); 
    Serial.print(",");
    Serial.print("z=");
    Serial.print(z);
    Serial.println("FT");

    delay(100);
  }
}