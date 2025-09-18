#ifndef ALGORITMOS_CALCULADORA_H
#define ALGORITMOS_CALCULADORA_H

#include <iostream>
#include <string>
#include <stdbool.h>
#include <stdint.h>


using String = std::string;   // Alias para que String funcione
using boolean = bool;   // Alias
using byte = uint8_t;   // Alias


#define Suma              '+'
#define Resta             '-'
#define Multiplicacion    '*'
#define Division          '/'



////////////////////////// ----------------- OPERACIONES NUMERICAS ----------------------------------

byte Numero_operadores(String Texto);
int Encontrar_Index_Operador_Ascendente(String Texto);
int Encontrar_Index_Operador_Descendente(String Texto);
double Operacion_Matematica (byte Operacion, double A, double B);
String Realizar_Operacion(String Texto_Temporal);
String calcular_Operacion_Completa(String Texto_Digitado);

#endif