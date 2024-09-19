#include <LiquidCrystal.h>

// Variables globales
bool *digital, *senaldesconocida;
int *canmaximo, *canminimo, *maximo, *minimo;
unsigned long int myTimer1, myTimer2; // variable de almacenamiento (unsigned long)
int analogPin = 0;
short int *n, *n2, *n3, *tipoonda;
float *voltajetotal;
int *frecuencia;
int comenzar = 2, terminar = 4;
int *toma;   // Arreglo dinámico para almacenar lecturas de datos
int *borrar;
int *analisis;  // Arreglo de análisis de datos

// Configuración de los pulsadores
const int botonInicio = 7; // Pin para el pulsador de inicio
const int botonApagar = 8; // Pin para el pulsador de apagado

// Inicializa el LCD (con los pines que estés usando)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Funciones
void encontrarMayorYMenor(int arr[], int tamaño, int& mayor, int& menor);
void determinarFrecuencia(int can_maximo, int can_minimo, int& frecuencia, bool& senal_desconocida);
void analizarDatos(int analisis[], int tamaño, int maximo, int minimo, int& canmaximo, int& canminimo, bool& digital);
void calcularAmplitud(int maximo, int minimo, float& voltaje);
int contarCambios(int datos[], int tamaño);
bool esOndaSinuoidal(int datos[], int tamaño);

void setup() {
    Serial.begin(9600); // Cambié la velocidad a 9600 baudios

    // Inicializa el LCD
    lcd.begin(16, 2);
    lcd.print("Listo"); // Muestra "Listo" en el LCD

    // Inicialización de variables de memoria dinámica
    digital = new bool(true); // variable que confirma si es una señal digital
    senaldesconocida = new bool(false); // variable que confirma si es una señal desconocida
    canmaximo = new int(0);
    canminimo = new int(0);
    maximo = new int(0);
    minimo = new int(0);
    frecuencia = new int(0);
    n = new short int(0);
    n2 = new short int(1);
    n3 = new short int(1);
    voltajetotal = new float(1.0);
    tipoonda = new short int(0);
    toma = new int[103]; // Inicializa `toma` para almacenar 103 lecturas
    analisis = new int[103]; // Inicializa `analisis` para almacenar 103 lecturas
  
    // Configura los pines de los pulsadores como entradas
    pinMode(botonInicio, INPUT_PULLUP);
    pinMode(botonApagar, INPUT_PULLUP);
}

void loop() {
  
    // Leer estado de los pulsadores
    if (digitalRead(botonInicio) == LOW) {
        // Si se presiona el botón de inicio
        *n2 = 0;
        *n3 = 1;
    }
    if (digitalRead(botonApagar) == LOW) { // Si se presiona el botón de apagado
        *n2 = 1;
        *n3 = 0;
    }

    // Comienza la recopilación de datos
    if (*n2 == 0) {
        if (millis() - myTimer2 >= 5) { // Temporizador de 5 ms (200 veces por segundo)
            myTimer2 = millis(); // reinicia
            (*n)++;
            toma[*n] = analogRead(analogPin);
        }
        if (millis() - myTimer1 >= 500) { // Procesa los datos cada 500 ms
            myTimer1 = millis();
            *n = 0;
            borrar = toma;
            analisis = borrar;
            toma = new int[103]; // Reservar nuevo espacio para almacenar datos
        }
    }

    // Procesamiento de datos
    if (*n3 == 0) {
        lcd.clear();
        lcd.print("Processing"); // Muestra "Processing" en el LCD

        encontrarMayorYMenor(analisis, 103, *maximo, *minimo);
        calcularAmplitud(*maximo, *minimo, *voltajetotal);
        analizarDatos(analisis, 103, *maximo, *minimo, *canmaximo, *canminimo, *digital);

        if (*digital == false) {
            determinarFrecuencia(*canmaximo, *canminimo, *frecuencia, *senaldesconocida);
        } else { // Determinar la frecuencia de una onda digital
            *frecuencia = contarCambios(analisis, 103) / 2 + 1;
        }

        if (*digital == false && *senaldesconocida == false) {
            if (esOndaSinuoidal(analisis, 103) == true) {
                *tipoonda = 3;  // Onda sinusoidal
            } else if (*digital == true) {
                *tipoonda = 1;  // Onda digital
            } else if (*senaldesconocida == true) {
                *tipoonda = 4;  // Onda desconocida
            }
        }
    }
}

// Función para encontrar el valor máximo y mínimo en un arreglo
void encontrarMayorYMenor(int arr[], int tamaño, int& mayor, int& menor) {
    mayor = arr[0];
    menor = arr[0];
    for (int i = 1; i < tamaño; i++) {
        if (arr[i] > mayor) {
            mayor = arr[i];
        }
        if (arr[i] < menor) {
            menor = arr[i];
        }
    }
}

// Función para determinar la frecuencia
void determinarFrecuencia(int can_maximo, int can_minimo, int& frecuencia, bool& senal_desconocida) {
    if (can_maximo == can_minimo) {
        senal_desconocida = true;  // Señal desconocida
        frecuencia = 0;
    } else {
        senaldesconocida = false;
        frecuencia = 1;  // Placeholder (puedes ajustar esto para calcular la frecuencia real)
    }
}

// Función para analizar los datos
void analizarDatos(int analisis[], int tamaño, int maximo, int minimo, int& canmaximo, int& canminimo, bool& digital) {
    canmaximo = 0;
    canminimo = 0;
    digital = true;  // Se asume que es digital al principio

    for (int i = 0; i < tamaño; i++) {
        int x = analisis[i];
        if (maximo == x) {
            canmaximo++;
        } else if (minimo == x) {
            canminimo++;
        } else {
            digital = false;  // Si el valor es diferente del máximo y mínimo, la señal no es digital
        }
    }
}

// Función para calcular la amplitud
void calcularAmplitud(int maximo, int minimo, float& voltaje) {
    if (maximo > 0 && minimo > 0) {
        voltaje = maximo - minimo;
    } else if (maximo < 0 && minimo < 0) {
        voltaje = (maximo * -1) + minimo;
        maximo *= -1;
        minimo *= -1;
    } else {
        voltaje = maximo + minimo;
        minimo *= -1;
    }
    voltaje = voltaje / 409.2;  // Escala de conversión de voltaje
}

// Función para contar los cambios en los datos
int contarCambios(int datos[], int tamaño) {
    int cambioCan = 0;
    int valorAnterior = datos[0];

    for (int i = 1; i < tamaño; i++) {
        if (datos[i] != valorAnterior) {
            cambioCan++;
            valorAnterior = datos[i];
        }
    }

    return cambioCan;
}

// Función para verificar si la onda es sinusoidal
bool esOndaSinuoidal(int datos[], int tamaño) {
    int umbral = 100; // Umbral para la detección de cambios
    for (int i = 1; i < tamaño; i++) {
        int diferencia = datos[i] - datos[i - 1];
        if (diferencia > umbral || diferencia < -umbral) {
            return false;  // Si el cambio es brusco, no es sinusoidal
        }
    }
    return true;
}