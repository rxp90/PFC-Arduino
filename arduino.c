// Valor de MY en XBee (ATMY)
#define direccionMando 0x1
// Valor de ID en XBee (ATID)
#define idRed 0x3332
// Canal de comunicación XBee (ATCH)
#define canal 0xC
// Dirección del receptor (ATDL)
#define direccionReceptor 0x2

boolean accelSleep;
char inData[100]; // Espacio para los datos entrantes
char inChar; // Caracter leído
byte index = 0; // Índice del array

String cadenaIdentificacion = "MY=" + String(direccionMando) + ",ID=" + String(idRed) + ",CH=" + String(canal) + ",DL=" + String(direccionReceptor);

unsigned int delayMuestreo = 50; // Delay de muestreo por defecto

int instruccion; // Variable para guardar la instrucción a ejecutar
unsigned int aux = 0; // Variable para almacenar el segundo parámetro recibido (delay de muestreo) 
char identificadorEntrante[100]; // Array para guardar la cadena que comprobará si la instrucción es o no para este mando


void setup()
{  
  // Configuración puerto Serie
  Serial.begin(9600); // 9600 bps
  delay(1000);
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

  recibeInstrucciones();

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

  }
  delay(delayMuestreo);

}

void recibeInstrucciones(){
  //  Leo mientras haya datos
  while (Serial.available() > 0)
  {
    if(index < 99) 
    {
      inChar = Serial.read(); // Leo el caracter
      inData[index] = inChar; // Lo guardo
      index++; // Aumento el índice del array
      inData[index] = '\0'; // Cierro la cadena

      // Parseo la cadena entrante
      sscanf (inData,"%s %d %d",identificadorEntrante,&instruccion,&aux);
    }
  }

  // Si el primer dato recibido coindice con el identificador del mando
  if ((String(identificadorEntrante).compareTo(cadenaIdentificacion))  == 0) {
    // "Limpio" los arrays
    for (int i=0;i<99;i++) {
      inData[i]=0;
      identificadorEntrante[i]=0;
    }
    index=0;
    // Ejecuto la instrucción asociada
    switch(instruccion){

    case 1:
      // Apagar acelerómetro

      accelSleep = LOW;
      delay(1000); 

      digitalWrite(4, accelSleep);
      digitalWrite(13, accelSleep);
      Serial.println("OK");
      break;

    case 2:
      // Encender acelerómetro

      accelSleep = HIGH;
      delay(1000); 

      digitalWrite(4, accelSleep);
      digitalWrite(13, accelSleep);
      Serial.println("OK");
      break;

    case 3:      
      // Cambiar velocidad de muestreo
      if(aux>0){
        delayMuestreo = aux;
      }
      Serial.println("OK");
      break;
    }
  }  

}




