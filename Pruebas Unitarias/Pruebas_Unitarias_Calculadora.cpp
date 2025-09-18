#include "src/unity.h"

#include "Algoritmos_Calculadora.h"

void setUp(void) {}

void tearDown(void) {}

void test_Numero_operadores(void) {
    TEST_ASSERT_EQUAL_UINT8(4, Numero_operadores("15+30*4/3-5"));
}


void test_Encontrar_Index_Operador_Ascendente(void) {
    TEST_ASSERT_EQUAL(2, Encontrar_Index_Operador_Ascendente("15+30*4/3"));
}


void test_Encontrar_Index_Operador_Descendente(void) {
    TEST_ASSERT_EQUAL(9, Encontrar_Index_Operador_Descendente("15+30*4/3-5"));
}


void test_Operacion_Matematica(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01, 14.3, Operacion_Matematica('+', 5.97 , 8.33));
}

void test_Operacion_Matematica_2(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.71, Operacion_Matematica('/', 5.97 , 8.33));
}


void test_Realizar_Operacion(void) {
    TEST_ASSERT_EQUAL_STRING("15+120.000000/3-5", Realizar_Operacion("15+30*4/3-5").c_str());
}

void test_calcular_Operacion_Completa(void) {
    TEST_ASSERT_EQUAL_STRING("50.000000", calcular_Operacion_Completa( "15+30*4/3-5").c_str());
}


int main() {
    UNITY_BEGIN();

    RUN_TEST(test_Numero_operadores);
    RUN_TEST(test_Encontrar_Index_Operador_Ascendente);
    RUN_TEST(test_Encontrar_Index_Operador_Descendente);
    RUN_TEST(test_Operacion_Matematica);
    RUN_TEST(test_Operacion_Matematica_2);
    RUN_TEST(test_Realizar_Operacion);
    RUN_TEST(test_calcular_Operacion_Completa);
    
    return UNITY_END();
}

// 1) Ejecutar: g++ Pruebas_Unitarias_Calculadora.cpp Algoritmos_Calculadora.cpp src/unity.c -o pruebas_unitarias.exe
// 2) Ejecutar: ./pruebas_unitarias.exe