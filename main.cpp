#include <iostream>
#include <vector>
#include <iomanip>
#include "src/core_numeric.cpp"

// -----------------------------------------------------------------------------
// Clases personalizadas de prueba
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

    friend std::ostream& operator<<(std::ostream& os, const Coordenadas& c) {
        return os << "(" << c.x << ", " << c.y << ")";
    }
};

struct Vector3D {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vector3D operator+(const Vector3D& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vector3D operator-(const Vector3D& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vector3D operator*(const Vector3D& o) const { return {x * o.x, y * o.y, z * o.z}; }
    Vector3D operator/(std::size_t n) const { 
        return {x / static_cast<double>(n), y / static_cast<double>(n), z / static_cast<double>(n)}; 
    }
    bool operator<(const Vector3D& o) const { 
        return (x * x + y * y + z * z) < (o.x * o.x + o.y * o.y + o.z * o.z); 
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};

struct Esfera {
    double radio = 0.0;

    Esfera operator+(const Esfera& o) const { return {radio + o.radio}; }
    Esfera operator-(const Esfera& o) const { return {radio - o.radio}; }
    Esfera operator*(const Esfera& o) const { return {radio * o.radio}; }
    Esfera operator/(std::size_t n) const { return {radio / static_cast<double>(n)}; }
    bool operator<(const Esfera& o) const { return radio < o.radio; }

    friend std::ostream& operator<<(std::ostream& os, const Esfera& e) {
        return os << "Esfera(r=" << e.radio << ")";
    }
};

int main() {
    std::cout << "========================================\n";
    std::cout << "        CoreNumeric Demonstration       \n";
    std::cout << "========================================\n\n";

    // 1. Tipos fundamentales (std::vector<double>)
    std::vector<double> v_double {1.0, 2.0, 3.0, 4.0, 5.0};
    std::cout << "--- Contenedor std::vector<double> ---\n";
    std::cout << "Suma: " << core_numeric::sum(v_double) << "\n";
    std::cout << "Promedio (mean): " << core_numeric::mean(v_double) << "\n";
    std::cout << "Varianza (variance): " << core_numeric::variance(v_double) << "\n";
    std::cout << "Maximo (max): " << core_numeric::max(v_double) << "\n";
    
    auto sq = core_numeric::transform_reduce(v_double, [](double x) { return x * x; });
    std::cout << "Transform Reduce (suma de cuadrados): " << sq << "\n\n";

    // 2. Tipos enteros (std::vector<int>)
    std::vector<int> v_int {10, 20, 30, 40};
    std::cout << "--- Contenedor std::vector<int> ---\n";
    std::cout << "Suma: " << core_numeric::sum(v_int) << "\n";
    std::cout << "Promedio (mean, if constexpr int): " << core_numeric::mean(v_int) << "\n";
    std::cout << "Varianza (variance, if constexpr int): " << core_numeric::variance(v_int) << "\n";
    std::cout << "Maximo (max): " << core_numeric::max(v_int) << "\n\n";

    // 3. Funciones Variadic con Fold Expressions
    std::cout << "--- Funciones Variadic (Fold Expressions) ---\n";
    auto s1 = core_numeric::sum_variadic(1, 2, 33, 4);
    auto s2 = core_numeric::mean_variadic(0.1, 2, 3, 4);
    auto s3 = core_numeric::variance_variadic(1, 2, 3, 4);
    auto s4 = core_numeric::max_variadic(1, 2.7, 3, 4);

    std::cout << "sum_variadic(1, 2, 33, 4) = " << s1 << "\n";
    std::cout << "mean_variadic(0.1, 2, 3, 4) = " << s2 << "\n";
    std::cout << "variance_variadic(1, 2, 3, 4) = " << s3 << "\n";
    std::cout << "max_variadic(1, 2.7, 3, 4) = " << s4 << "\n\n";

    // 4. Objetos de clases personalizadas (Coordenadas, Vector3D, Esfera)
    std::cout << "--- Objetos de Clases Personalizadas ---\n";

    std::vector<Coordenadas> coords {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}};
    std::cout << "Coordenadas sum: " << core_numeric::sum(coords) << "\n";
    std::cout << "Coordenadas mean: " << core_numeric::mean(coords) << "\n";
    std::cout << "Coordenadas max: " << core_numeric::max(coords) << "\n\n";

    std::vector<Vector3D> vectores {{1.0, 0.0, 0.0}, {0.0, 2.0, 0.0}, {0.0, 0.0, 3.0}};
    std::cout << "Vector3D sum: " << core_numeric::sum(vectores) << "\n";
    std::cout << "Vector3D mean: " << core_numeric::mean(vectores) << "\n";
    std::cout << "Vector3D max: " << core_numeric::max(vectores) << "\n\n";

    std::vector<Esfera> esferas {{1.5}, {3.0}, {0.5}};
    std::cout << "Esferas sum: " << core_numeric::sum(esferas) << "\n";
    std::cout << "Esferas mean: " << core_numeric::mean(esferas) << "\n";
    std::cout << "Esferas max: " << core_numeric::max(esferas) << "\n\n";

    std::cout << "========================================\n";
    std::cout << " Execution completed successfully!     \n";
    std::cout << "========================================\n";

    return 0;
}
