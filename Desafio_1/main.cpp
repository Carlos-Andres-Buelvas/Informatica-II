// C++ code
//

unsigned long int myTimer1, myTimer2, myTimer3;// variable de almacenamiento de tiempo (unsigned long)
int analogPin=0,val=0;
float vol;
int comenzar= 2, terminar=4;
int datos_entoma[203],datos_analisis[203]; //estos arreglos deben estar en la memoria

void setup()
{
  Serial.begin(11200);
  pinMode(comenzar, INPUT);
  pinMode(terminar, INPUT);
  

}

void loop()
{
  // la varible n2 vincular de alguna manera a los pulsadores
  if(n2==0){// parte de la recopilacion de datos
    if (millis() - myTimer2 >= 5) {   // temporizador de 5 ms (200 veces por segundo)
    myTimer2 = millis();              // reinicia
    // realizar la acción 199
    // 200 veces por segundo
    
  }
    if (millis() - myTimer1 >= 500) {   // temporizador de 500 ms (2 veces por segundo)
    myTimer1 = millis();              //reinicia
    // realizar la acción 1
    // 2 veces por segundo
  }
  
  Serial.println(analogRead(analogPin));
  
}