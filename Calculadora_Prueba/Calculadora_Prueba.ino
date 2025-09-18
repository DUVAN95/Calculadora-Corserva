#include <LiquidCrystal_I2C.h>
//


#define Suma              '+'
#define Resta             '-'
#define Multiplicacion    '*'
#define Division          '/'


String Texto_Digitado = "";
boolean Calcular_Resultado = false;

const uint8_t Pines_Filas_Teclados [] = { 25, 26,27, 14 };
const uint8_t Pines_Columnas_Teclados [] = { 2, 4, 32,33 };
uint8_t registro_Filas = 0b0001;

byte Cero_Calculadora = 12;

byte Buzzer = 23;
boolean Activar_Bip_Buzzer = false;



LiquidCrystal_I2C lcd(0x27,16,2);
boolean Actualizar_LCD = false;


xTaskHandle tarea_1 = NULL;
unsigned long Temporizaor_Interrupcion = 0;


unsigned long Temporizaor_Inactividad_Pantalla = 0;
unsigned long TimueOut_Inactividad_Pantalla = 10000;

unsigned long Temporizaor_Inactividad = 0;
unsigned long TimueOut_Inactividad = 20000;





////////////////////////// ----------------- Algoritmos para teclado ----------------------------------
  
void IRAM_ATTR Interrupcion_Columnas(void* arg) {
  
  int Pin_Columna = (int) arg;
  
  if(millis() - Temporizaor_Interrupcion > 250){
    Temporizaor_Interrupcion = millis();
    //Serial.print("Interrupcion en columna PIN : " + String(Pin_Columna));
    //Serial.println("En interrrupcion filas: ..... "); 
    //Serial.println(registro_Filas,BIN);
    
    
    char Tecla = Identificar_Tecla(Pin_Columna);
    
    if(Tecla == '='){
      Calcular_Resultado = true;
    }else{
      if(Pin_Columna == Cero_Calculadora){
        Texto_Digitado = "";
      }else{
        Texto_Digitado +=  Tecla;
        Serial.print("Interup::: Texto Digitado : " + String(Texto_Digitado));
      }
    }

    Activar_Bip_Buzzer = true;
    Actualizar_LCD = true;
    
    Temporizaor_Inactividad_Pantalla = millis() + TimueOut_Inactividad_Pantalla; // Extiende el tiempo para nno caer en inactividad
    Temporizaor_Inactividad = millis() + TimueOut_Inactividad; // Extiende el tiempo para nno caer en inactividad
    
  }
  
}



void barrido (void* args){

  unsigned long Temporizaor_Visualizacion = millis();
  
  for(;;){
    
    //Serial.println("Multitarea....");  
    vTaskDelay(60);

    //////////--------- Barrido Filas Teclado matricial ----------------------------
    
    if(registro_Filas == 0b1000){
      registro_Filas = 0b0001;
    }else{
      registro_Filas = registro_Filas << 1;
    }
    
    Asignar_Estado_Pines_Filas(registro_Filas); 


    // -------- Temporizador de inactividad -----------------
    
    if(millis() >= Temporizaor_Inactividad){
        // enntrar en sleep
        Temporizaor_Inactividad = millis() + TimueOut_Inactividad; // Extiende el tiempo para nno caer en inactividad
        esp_deep_sleep_start();
    }

    if(millis() >= Temporizaor_Inactividad_Pantalla){
        Serial.println("APAGAR Pantalla");
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print (" APAGAR ");
        lcd.setCursor(1,1);
        lcd.print ("PANTALLA"); 
    }


    // visualizar Temposirzador
    if(millis() >= Temporizaor_Visualizacion){
      Temporizaor_Visualizacion = millis() + 1000;
      //Actualizar_LCD = true;
      lcd.setCursor(14,1);
      lcd.print ("  ");
      lcd.setCursor(14,1);
      lcd.print (String((Temporizaor_Inactividad- millis())/1000)); 
    }

    
    
  }
}


void Asignar_Estado_Pines_Filas(uint8_t bites){
  
  byte bit_posicion;
  
  for(byte i = 0; i < 4; i++){

    bit_posicion = (bites & 0b0001);    
    digitalWrite(Pines_Filas_Teclados [3-i], boolean (bit_posicion));
    bites = bites >> 1;
    
  }

  //Serial.println("Escribi los pines Filas:: "); 
  //Serial.println(registro_Filas,BIN);
  
}


  
char Identificar_Tecla(byte Pin_Columna){

  byte Columna = 255; 
  
  for(byte i = 0; i < 4; i++){
    if(Pines_Columnas_Teclados [i] == Pin_Columna){
      Columna = i;
      break;
    }
  }

  char Caracter = 'X';
  switch(registro_Filas){
    
    case 0b0001:
      switch(Columna){
        case 0:
          Caracter = '+'; break;
        case 1:
          Caracter = '3'; break;
        case 2:
          Caracter = '2'; break;
        case 3:
          Caracter = '1'; break;
      }
      break;
    
    case 0b0010:
      switch(Columna){
        case 0:
          Caracter = '-'; break;
        case 1:
          Caracter = '6'; break;
        case 2:
          Caracter = '5'; break;
        case 3:
          Caracter = '4'; break;
      }
      break;
    
    case 0b0100:
      switch(Columna){
        case 0:
          Caracter = '/'; break;
        case 1:
          Caracter = '9'; break;
        case 2:
          Caracter = '8'; break;
        case 3:
          Caracter = '7'; break;
      }
      break;
    
    case 0b1000:
      switch(Columna){
        case 0:
          Caracter = '='; break;
        case 1:
          Caracter = '.'; break;
        case 2:
          Caracter = '0'; break;
        case 3:
          Caracter = '*'; break;
      }
      break;
  }

  return Caracter;
}



////////////////////////// ----------------- OPERACIONES NUMERICAS ----------------------------------

// sintaxis error

byte Numero_operadores(){

  byte N_Operaciones = 0;
  
  for(byte i=0; i < Texto_Digitado.length(); i++){

      if(Texto_Digitado[i] == '*' || Texto_Digitado[i] == '/' || Texto_Digitado[i] == '+' || Texto_Digitado[i] == '-'){
        N_Operaciones ++;
      }
  }
  Serial.println("Numero de operaciones: " + String(N_Operaciones));
  return N_Operaciones;
}


int Encontrar_Index_Operador_Ascendente(String Texto){
  
  for(byte Index=0; Index < Texto.length(); Index++){
      if(Texto[Index] == '*' || Texto[Index] == '/' || Texto[Index] == '+' || Texto[Index] == '-'){
        return Index;
      }
  }
  return -1;
}

int Encontrar_Index_Operador_Descendente(String Texto){
  
  for(int Index=Texto.length()-1; Index >= 0; Index--){
      //Serial.println("charchar: " + String(Texto[Index]) + " *** Pos: " + String(Index));
      if(Texto[Index] == '*' || Texto[Index] == '/' || Texto[Index] == '+' || Texto[Index] == '-'){
        return Index;
      }
  }
  return -1;
}


void calcular_Operacion_Completa(){

  Serial.println("Operacion a Calcular:  = " + Texto_Digitado);
  uint8_t N_Operaciones = Numero_operadores();
  
  for(byte N = 0; N < N_Operaciones; N ++){
    Realizar_Operacion();
  }
  String Resultado_Final = Texto_Digitado;
  Serial.println("Resultado Final = " + Resultado_Final);
  Actualizar_LCD = true ;
}


int8_t Realizar_Operacion(){
  
  char operadores_jerarquia[] = {'*','/','+','-'};

  for(byte Jerarquia = 0; Jerarquia < 4; Jerarquia ++){
    
    for(byte i=0; i < Texto_Digitado.length(); i++){

        if(Texto_Digitado[i] == operadores_jerarquia[Jerarquia]){
          
          // encuentro el indice e identifico el operador
          char Operador =  Texto_Digitado[i];
          //Serial.println("Operador: " + String(Operador) + " *** Pos: " + String(i));
          
          // parto el string en antes y despues del indice
          String Texto_Izquierda = Texto_Digitado.substring(0, i);
          String Texto_Derecha = Texto_Digitado.substring(i + 1);
          //Serial.println("Texto_Izquierda: " + String(Texto_Izquierda));
          //Serial.println("Texto_Derecha: " + String(Texto_Derecha));
          
          //debo obtener el numero anterior y posterior al operador
          int index_Operador_Izquierda = Encontrar_Index_Operador_Descendente(Texto_Izquierda);    
          
          int index_Operador_Derecha = Encontrar_Index_Operador_Ascendente(Texto_Derecha);
          if(index_Operador_Derecha == -1){
            index_Operador_Derecha = Texto_Derecha.length(); 
          }
          
          //Serial.println("index_Operador_Izquierda: " + String(index_Operador_Izquierda));
          //Serial.println("index_Operador_Derecha: " + String(index_Operador_Derecha));
          
          double Numero_Anterior = Texto_Izquierda.substring(index_Operador_Izquierda + 1).toDouble();
          double Numero_Posterior = Texto_Derecha.substring(0, index_Operador_Derecha).toDouble();

          //Serial.println("Numero_Anterior: " + String(Numero_Anterior));
          //Serial.println("Numero_Posterior: " + String(Numero_Posterior));
          

          // se operan los numeros
          double Resultado = Operacion_Matematica(Operador, Numero_Anterior, Numero_Posterior);
          
          /// El resultado se concatena en medio.
          if(index_Operador_Izquierda == 0){
            // No hay texto a la izquierda
            Texto_Izquierda = "";
          }
          
          Texto_Digitado = Texto_Izquierda.substring(0, index_Operador_Izquierda+1) + Resultado + Texto_Derecha.substring(index_Operador_Derecha);

          //Texto_Digitado = "54+21+30+27";
          Serial.println("Resultado Parcial = " + Texto_Digitado);
          return 0;
        }
    }
  }
  return 0;
}


double Operacion_Matematica (byte Operacion, double A, double B){

  switch(Operacion){
    case Suma:
      return(A + B);
      break;
    case Resta:
      return(A - B);
      break;
    case Multiplicacion:
      return(A * B);
      break;
    case Division:
      return(A / B);
      break;
    default:
      return 0;
      break;
  }
}

void Bip_Buzzer(){
  
  digitalWrite(Buzzer,HIGH);
  delay(300);
  digitalWrite(Buzzer,LOW);
  
}

void Realizar_Actualizacion_LCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print (Texto_Digitado); 
    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print ("Bienvenido");
  delay(2000);

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print ("Corserva");
  lcd.setCursor(2,1);
  lcd.print ("Calculadora");
  delay(2000);

  for(byte i = 0; i < 3; i++){
    lcd.clear();
    delay(500);  
    lcd.setCursor(2,0);
    lcd.print ("Presiona");
    lcd.setCursor(2,1);
    lcd.print ("un Boton");
    delay(500);  
  }

  // -------------  Interrupciones para salir del Bajo consumo de energia ------- 
  
  Temporizaor_Inactividad = millis() + TimueOut_Inactividad;
  uint64_t Mascara_Pines_Activacion = (1ULL  << 2) | (1ULL  << 4) | (1ULL  << 32) | (1ULL  << 33) | (1ULL  << Cero_Calculadora) ; // { 2, 4, 32,33 }
  esp_sleep_enable_ext1_wakeup(Mascara_Pines_Activacion, ESP_EXT1_WAKEUP_ANY_HIGH);

  // ------------------------ Temporizador Inactividad Pantallla --------------------
  Temporizaor_Inactividad_Pantalla = millis() + TimueOut_Inactividad_Pantalla; // Extiende el tiempo para nno caer en inactividad
  
  // ------------------------ Multitarea : Barrido Filas y Temporizadores--------------------
  xTaskCreatePinnedToCore(barrido, "tarea_1", 2048, NULL, 0, &tarea_1, 1);
  
  
  /// configurar Buzzer
  pinMode(Buzzer,OUTPUT);
  digitalWrite(Buzzer,LOW);
 
  // Configurar salidas de filas.
  for(byte i = 0; i < 4; i++){
    pinMode(Pines_Filas_Teclados [i],OUTPUT);
  }

  // Configurar interrupcion en columnas.
  for(byte i = 0; i < 4; i++){
    pinMode(Pines_Columnas_Teclados[i], INPUT_PULLDOWN);
    attachInterruptArg( Pines_Columnas_Teclados[i], Interrupcion_Columnas, (void*) Pines_Columnas_Teclados[i],RISING);
  }

  pinMode(Cero_Calculadora, INPUT_PULLDOWN);
  attachInterruptArg( Cero_Calculadora, Interrupcion_Columnas, (void*) Cero_Calculadora,RISING);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("empezamos....123");
  delay(1000); // this speeds up the simulation

  if(Calcular_Resultado == true){
    Calcular_Resultado = false;
    calcular_Operacion_Completa();
    Serial.println("Orden de calcular Resultado");
  }

  if(Activar_Bip_Buzzer == true){
    Activar_Bip_Buzzer = false;
    Bip_Buzzer();
    Serial.println("Orden de ACtivar Buzzer");
  }

  if(Actualizar_LCD == true){
    Actualizar_LCD = false;
    Realizar_Actualizacion_LCD();
    Serial.println("Orden de Actualizar LCD");
  }

  
}
