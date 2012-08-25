// Valor de MY en XBee (ATMY)
#define direccionMando 0x1
// Valor de ID en XBee (ATID)
#define idRed 0x3332
// Canal de comunicación XBee (ATCH)
#define canal 0xC
// Dirección del receptor (ATDL)
#define direccionReceptor 0x2

boolean accelSleep;
char inData[30]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character
String cadenaIdentificacion = "MY=" + String(direccionMando) + ",ID=" + String(idRed) + ",CH=" + String(canal) + ",DL=" + String(direccionReceptor);
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
char Comp(String This) {
  while (Serial.available() > 0) // Don't read unless
    // there you know there is data
  {
    if(index < 29) // One less than the size of the array
    {
      inChar = Serial.read(); // Read a character
      inData[index] = inChar; // Store it
      index++; // Increment where to write next
      inData[index] = '\0'; // Null terminate the string
    }
  }

  if ((String(inData).compareTo(This))  == 0) {
    for (int i=0;i<29;i++) {
      inData[i]=0;
    }
    index=0;
    return(0);
  }
  else{
    return (1);
  }
}
void loop()
{
  // Compruebo si la instrucción va dirigida a este mando.
  if (Comp(cadenaIdentificacion + ",S")==0) {
    accelSleep = LOW;
    digitalWrite(4, accelSleep);
    digitalWrite(13, HIGH);
    Serial.println('OK');
    delay(1000);
  }
  if (Comp(cadenaIdentificacion + ",W")==0) {
    accelSleep = HIGH;
    digitalWrite(4, accelSleep);
    digitalWrite(13, LOW);
    Serial.println('OK');
    delay(1000);
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
    Serial.print(",");
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

    delay(50);
  }
}


