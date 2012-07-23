int incomingByte;
void setup()
{  
  // Configuración puerto Serie
  Serial.begin(9600); // 9600 bps
  // Ponemos en alto la entrada ¬ SLEEP
  digitalWrite(4, HIGH);
  // Para ganar precisión se usa una fuente de referencia externa (3.3V) 
  analogReference(EXTERNAL);
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
}

void loop()
{


  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'S') {
      digitalWrite(4, LOW);    
      digitalWrite(13, HIGH);
    } 
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'W') {
      digitalWrite(4, HIGH);    
      digitalWrite(13, LOW);
    }
  }
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

    Serial.print(x);
    Serial.print(",");
    Serial.print(y); 
    Serial.print(",");
    Serial.println(z);
    delay(100);
}