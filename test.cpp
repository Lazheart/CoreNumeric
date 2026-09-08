#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>
#include "src/core_numeric.cpp"

// Helper para comparar floats con tolerancia
bool is_close(double a, double b, double tol = 1e-5) {
    return std::abs(a - b) < tol;
}

// -----------------------------------------------------------------------------
// Clases para pruebas
// -----------------------------------------------------------------------------

struct Coordenadas {
    double x = 0.0;
    double y = 0.0;

    Coordenadas operator+(const Coordenadas& o) const { return {x + o.x, y + o.y}; }
    Coordenadas operator-(const Coordenadas& o) const { return {x - o.x, y - o.y}; }
    Coordenadas operator*(const Coordenadas& o) const { return {x * o.x, y * o.y}; }
    Coordenadas operator/(std::size_t n) const { 
        return {x / static_cast<double>(n), y / static_cast<double>(n)}; 
    }
    bool operator<(const Coordenadas& o) const { 
        return (x * x + y * y) < (o.x * o.x + o.y * o.y); 
    }
    bool operator==(const Coordenadas& o) const {
        return is_close(x, o.x) && is_close(y, o.y);
    }
};

struct SinOperadores {
    std::string nombre;
    // Esta clase NO define +, -, *, /, <
};

struct SinComparador {
    int valor;
    SinComparador operator+(const SinComparador& o) const { return {valor + o.valor}; }
    SinComparador operator/(std::size_t n) const { return {valor / static_cast<int>(n)}; }
    // NO define operator<
};

// -----------------------------------------------------------------------------
// PRUEBAS DE CASOS VÁLIDOS (Compilan y ejecutan correctamente)
// -----------------------------------------------------------------------------

void test_sum() {
    std::vector<int> v1 {1, 2, 3, 4};
    assert(core_numeric::sum(v1) == 10);

    std::vector<double> v2 {1.5, 2.5, 3.0};
    assert(is_close(core_numeric::sum(v2), 7.0));

    std::cout << "[PASS] test_sum\n";
}

void test_mean() {
    std::vector<double> v1 {1.0, 2.0, 3.0};
    assert(is_close(core_numeric::mean(v1), 2.0));

    std::vector<int> v2 {10, 20, 30};
    assert(core_numeric::mean(v2) == 20);

    std::cout << "[PASS] test_mean\n";
}

void test_variance() {
    std::vector<double> v {1.0, 2.0, 3.0};
    // Promedio = 2.0. Diferencias cuadraticas: (1-2)^2 + (2-2)^2 + (3-2)^2 = 1 + 0 + 1 = 2
    // Varianza = 2.0 / 3 = 0.666666...
    assert(is_close(core_numeric::variance(v), 2.0 / 3.0));

    std::cout << "[PASS] test_variance\n";
}

void test_max() {
    std::vector<double> v {1.0, 2.7, 0.3};
    assert(is_close(core_numeric::max(v), 2.7));

    std::vector<int> v_int {5, 12, -3, 8};
    assert(core_numeric::max(v_int) == 12);

    std::cout << "[PASS] test_max\n";
}

void test_transform_reduce() {
    std::vector<double> v {1.0, 2.0, 3.0};
    auto res = core_numeric::transform_reduce(v, [](double x) { return x * x; });
    // 1^2 + 2^2 + 3^2 = 1 + 4 + 9 = 14
    assert(is_close(res, 14.0));

    std::cout << "[PASS] test_transform_reduce\n";
}

void test_variadic() {
    auto s1 = core_numeric::sum_variadic(1, 2, 33, 4);
    assert(s1 == 40);

    auto s2 = core_numeric::mean_variadic(0.1, 2, 3, 4);
    assert(is_close(s2, 9.1 / 4.0));

    auto s3 = core_numeric::variance_variadic(1.0, 2.0, 3.0, 4.0);
    // mean = 2.5, sq_diffs = 2.25 + 0.25 + 0.25 + 2.25 = 5.0, var = 5.0/4 = 1.25
    assert(is_close(s3, 1.25));

    auto s4 = core_numeric::max_variadic(1, 2.7, 3, 4);
    assert(is_close(s4, 4.0));

    std::cout << "[PASS] test_variadic\n";
}

void test_custom_classes() {
    std::vector<Coordenadas> coords {{1.0, 2.0}, {3.0, 4.0}};
    Coordenadas s = core_numeric::sum(coords);
    assert(s == Coordenadas(4.0, 6.0));

    Coordenadas m = core_numeric::mean(coords);
    assert(m == Coordenadas(2.0, 3.0));

    Coordenadas mx = core_numeric::max(coords);
    assert(mx == Coordenadas(3.0, 4.0));

    std::cout << "[PASS] test_custom_classes\n";
}

// -----------------------------------------------------------------------------
// DOCUMENTACIÓN Y EJEMPLOS DE CASOS INVÁLIDOS (NO COMPILAN)
// -----------------------------------------------------------------------------
/*
 * A continuación se describen los casos de código que fallan al compilar,
 * especificando exactamente qué Concept falla en cada situación:
 *
 * 1. CASO INVALIDO: std::string no cumple Divisible
 * ------------------------------------------------
 * Código:
 *   std::vector<std::string> words {"Hola", "Mundo"};
 *   auto m = core_numeric::mean(words);
 *
 * FALLO DE CONCEPT: 'Divisible<std::string>'
 * Razón: std::string no tiene definido el operador '/' para dividirse entre un std::size_t.
 * La cláusula requires Divisible<container_value_t<C>> de 'mean' falla en tiempo de compilación.
 *
 * 2. CASO INVALIDO: std::string en variance
 * ----------------------------------------
 * Código:
 *   std::vector<std::string> words {"a", "b", "c"};
 *   auto v = core_numeric::variance(words);
 *
 * FALLO DE CONCEPT: 'Divisible<std::string>', 'Subtractable<std::string>', 'Multiplicable<std::string>'
 * Razón: std::string no soporta división, resta ni multiplicación entre cadenas.
 * La función 'variance' rechaza el tipo std::string en tiempo de compilación.
 *
 * 3. CASO INVALIDO: std::string en max de CoreNumeric
 * -------------------------------------------------
 * Código:
 *   std::vector<std::string> words {"a", "b", "c"};
 *   auto m = core_numeric::max(words);
 *
 * FALLO DE CONCEPT: 'Divisible<std::string>'
 * Razón: En CoreNumeric, los algoritmos numéricos requieren la categoría completa de tipo numérico
 * (que incluye Divisible y Addable). std::string no es un tipo numérico apto para CoreNumeric.
 *
 * 4. CASO INVALIDO: Tipo no Iterable (int primitivo)
 * -------------------------------------------------
 * Código:
 *   int x = 5;
 *   auto s = core_numeric::sum(x);
 *
 * FALLO DE CONCEPT: 'Iterable<int>'
 * Razón: int no posee los métodos std::begin(x) ni std::end(x).
 * La restricción template <Iterable C> rechaza tipos no contenedores.
 *
 * 5. CASO INVALIDO: Clase sin sobrecarga de operador < en max
 * ----------------------------------------------------------
 * Código:
 *   std::vector<SinComparador> lista {{1}, {2}};
 *   auto m = core_numeric::max(lista);
 *
 * FALLO DE CONCEPT: 'Comparable<SinComparador>'
 * Razón: La clase SinComparador no implementa operator<, por lo que no cumple
 * con el concept personalizado Comparable.
 *
 * 6. CASO INVALIDO: Clase sin operadores en sum
 * --------------------------------------------
 * Código:
 *   std::vector<SinOperadores> personas {{"Juan"}, {"Maria"}};
 *   auto s = core_numeric::sum(personas);
 *
 * FALLO DE CONCEPT: 'Addable<SinOperadores>'
 * Razón: SinOperadores no implementa operator+, impidiendo su uso en sum.
 */

// Se pueden activar las macros TEST_FAIL_* al compilar para verificar manualmente
// que el compilador rechaza las operaciones inválidas.

#ifdef TEST_FAIL_STRING_DIVISIBLE
void test_fail_string_mean() {
    std::vector<std::string> words {"Hola", "Mundo"};
    core_numeric::mean(words); // ERROR: std::string no cumple Divisible
}
#endif

#ifdef TEST_FAIL_NON_ITERABLE
void test_fail_non_iterable() {
    int x = 5;
    core_numeric::sum(x); // ERROR: int no cumple Iterable
}
#endif

#ifdef TEST_FAIL_NON_COMPARABLE
void test_fail_non_comparable() {
    std::vector<SinComparador> lista {{1}, {2}};
    core_numeric::max(lista); // ERROR: SinComparador no cumple Comparable
}
#endif

int main() {
    std::cout << "========================================\n";
    std::cout << "        CoreNumeric Test Suite          \n";
    std::cout << "========================================\n\n";

    test_sum();
    test_mean();
    test_variance();
    test_max();
    test_transform_reduce();
    test_variadic();
    test_custom_classes();

    std::cout << "\n========================================\n";
    std::cout << " All tests passed successfully!        \n";
    std::cout << "========================================\n";

    return 0;
}
