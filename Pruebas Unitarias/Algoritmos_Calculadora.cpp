
#include "Algoritmos_Calculadora.h"




////////////////////////// ----------------- OPERACIONES NUMERICAS ----------------------------------

byte Numero_operadores(String Texto){

  byte N_Operaciones = 0;
  
  for(byte i=0; i < Texto.length(); i++){

      if(Texto[i] == '*' || Texto[i] == '/' || Texto[i] == '+' || Texto[i] == '-'){
        N_Operaciones ++;
      }
  }
  printf("N_Operaciones: %d", N_Operaciones);
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
      if(Texto[Index] == '*' || Texto[Index] == '/' || Texto[Index] == '+' || Texto[Index] == '-'){
        return Index;
      }
  }
  return -1;
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



String Realizar_Operacion(String Texto_Temporal){
  
  char operadores_jerarquia[] = {'*','/','+','-'};

  for(byte Jerarquia = 0; Jerarquia < 4; Jerarquia ++){
    
    for(byte i=0; i < Texto_Temporal.length(); i++){

        if(Texto_Temporal[i] == operadores_jerarquia[Jerarquia]){
          
          // encuentro el indice e identifico el operador
          char Operador =  Texto_Temporal[i];
          //Serial.println("Operador: " + String(Operador) + " *** Pos: " + String(i));
          
          // parto el string en antes y despues del indice
          String Texto_Izquierda = Texto_Temporal.substr(0, i);
          String Texto_Derecha = Texto_Temporal.substr(i + 1);
          //Serial.println("Texto_Izquierda: " + String(Texto_Izquierda));
          //Serial.println("Texto_Derecha: " + String(Texto_Derecha));
          
          //debo obtener el numero anterior y posterior al operador
          int index_Operador_Izquierda = Encontrar_Index_Operador_Descendente(Texto_Izquierda);    
          
          int index_Operador_Derecha = Encontrar_Index_Operador_Ascendente(Texto_Derecha);
          if(index_Operador_Derecha == -1){
            index_Operador_Derecha = Texto_Derecha.length(); 
          }
          
          //printf("\nindex_Operador_Izquierda: %d", index_Operador_Izquierda);
          //printf("\nindex_Operador_Derecha: %d", index_Operador_Derecha);
          
          double Numero_Anterior = stod(Texto_Izquierda.substr(index_Operador_Izquierda + 1));
          double Numero_Posterior = stod(Texto_Derecha.substr(0, index_Operador_Derecha));

          //printf("\nNumero_Anterior: %.2f", Numero_Anterior);
          //printf("\nNumero_Posterior: %.2f", Numero_Posterior);
          

          // se operan los numeros
          double Resultado = Operacion_Matematica(Operador, Numero_Anterior, Numero_Posterior);
          
          /// El resultado se concatena en medio.
          if(index_Operador_Izquierda == 0){
            // No hay texto a la izquierda
            Texto_Izquierda = "";
          }
          
          Texto_Temporal = Texto_Izquierda.substr(0, index_Operador_Izquierda+1) + std::to_string(Resultado) + Texto_Derecha.substr(index_Operador_Derecha);
          printf("\nTexto_Parcial: %s\n", Texto_Temporal.c_str());
          return Texto_Temporal;
        }
    }
  }
  return Texto_Temporal;
}

String calcular_Operacion_Completa(String Texto_Digitado){

  uint8_t N_Operaciones = Numero_operadores(Texto_Digitado);
  
  String Resultado_Final = Texto_Digitado;
  for(byte N = 0; N < N_Operaciones; N ++){
    Resultado_Final = Realizar_Operacion(Resultado_Final);
  }
  return Resultado_Final;
} 
