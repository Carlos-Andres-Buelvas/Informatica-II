#include <iostream>

bool digital= true;//varible que confirma si es una señal digital
bool senal_desconocida=false ;//varible que confirma si es una señal digital

unsigned long int myTimer1, myTimer2;// variable de almacenamienti(unsigned long)
int analogPin=0;
short int n=0,val=0, n2=1, n3=1;
float vol;
unsigned int frecuencia;
int comenzar= 2, terminar=4;
int datos_entoma[203],datos_analisis[203], maximo,minimo,voltaje,can_maximo=0,can_minimo=0; //estos arreglos y varibles se en la memoria  deben estar en la memoria dinamica 

//funciones 
encontrarMayorYMenor(arr, tamaño, mayor, menor);


void setup()
{
  Serial.begin(11200);
  int *toma= new int datos_entoma[103];
  int *analisis= new int datos_analisis[103];

}

void loop()
{
  // la varible n2 vincular de alguna manera a los pulsadores
  if(n2==0){// parte de la recopilacion de datos
    if (millis() - myTimer2 >= 5) {   // temporizador de 5 ms (200 veces por segundo)
    myTimer2 = millis();              // reinicia
    n++;
    toma[n]=analogRead(analogPin);    
    // realizar la acción 2
    // 200 veces por segundo
    
  }
    if (millis() - myTimer1 >= 500) {   // temporizador de 500 ms (2 veces por segundo)
    myTimer1 = millis(); 
    //reinicia
    // realizar la acción 1
    // 2 veces por segundo
  }
  
  
  // la varible n3 vincular de alguna manera a los pulsadores
  if(n3==0){// parte del tratamineto de datos
      n2=1;
      encontrarMayorYMenor(datos_analisis,103,&maximo,&minimo);
      
      //parte del codigo donde me da el amplitud sin procesar.
      if ( maximo>0 && minimo>0){//casos dos posotivos
          voltaje=maximo-minimo;
      }
      else if(maximo<0 && minimo<0){//casos de dos negativos
          volatje=(maximo*-1)+minimo;
          maximo*=-1;
          minimo*=-1;
      }
      else{// caso signos opuestos
          volatje=maximo+minimo;
          minimo*=-1;
      }
      
  }
  
  
  // cogido que me dicta la cantiadad de minimo y maximos. y deternimar si es cuadrada
  for(int i=0;i<223;i++){
      x=datos_analisis[i]
      
      if(maximo==x){//contador de maximos
          can_maximo+=1;
      }
      else if (minimo==x){//contador de minimos
          can_minimo+=1;
      }
      else{// si el valor es diferen del maximo y minimo, significa que no una señal digital.
          digital=false;
      }
  }
  
  
  //cogido que deternima la frecuencia. y determina si es desconocidad
  if((can_maximo-can_minimo)<2 || (can_maximo-can_minimo)<-2 ){//si la diferncia es 2 o mayor, es una onda desconociada 
      senal_desconocida=true;
      frecuencia=0;
  }
  
  else if(can_maximo==can_minimo){//si son iguales, ya puedo determinar la frcuencia
      frecuencia=2*can_maximo;
  }
  
  else{//se coje el valor mas alto y se multiplica por dos y se le suma uno
      if(can_minimo<can_maximo){
          frecuencia=can_maximo*2 + 1;
      }
      else{
          frecuencia=can_minimo*2 + 1;
      }
  }
  

  
  
  Serial.println(analogRead(analogPin));
  }
  
}  

void encontrarMayorYMenor(int arr[], int tamaño, int& mayor, int& menor) {
    // Inicializamos mayor y menor con el primer elemento del arreglo
    mayor = arr[0];
    menor = arr[0];

    // Iteramos sobre el arreglo para encontrar el mayor y menor número
    for (int i = 1; i < tamaño; ++i) {
        if (arr[i] > mayor) {
            mayor = arr[i];
        }
        if (arr[i] < menor) {
            menor = arr[i];
        }
    }
}
