#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>
#include <cstddef>
#include <utility>
#include <cmath>

namespace core_numeric {

// 1. Concept: Iterable
// Restringe el tipo C a contenedores o rangos que poseen std::begin y std::end
template <typename C>
concept Iterable = requires (C c) {
    std::begin(c);
    std::end(c);
};

// Trait auxiliar para obtener el tipo de elemento subyacente de un contenedor Iterable
template <Iterable C>
using container_value_t = std::remove_cvref_t<decltype(*std::begin(std::declval<C&>()))>;

// 2. Concept: Addable
// Restringe el tipo T a aquellos que soportan el operador + retornando el mismo tipo T
template <typename T>
concept Addable = requires (T a, T b) {
    { a + b } -> std::same_as<T>;
};

// 3. Concept: Divisible
// Restringe el tipo T a aquellos que pueden dividirse entre un entero std::size_t retornando un tipo convertible a T
template <typename T>
concept Divisible = requires (T a, std::size_t n) {
    { a / n } -> std::convertible_to<T>;
};

// 4. Concepts Personalizados (Requirement #1)
// Comparable: Restringe el tipo T a aquellos que soportan comparación menor que <
template <typename T>
concept Comparable = requires (T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

// Subtractable: Restringe el tipo T a aquellos que soportan resta a - b -> T
template <typename T>
concept Subtractable = requires (T a, T b) {
    { a - b } -> std::same_as<T>;
};

// Multiplicable: Restringe el tipo T a aquellos que soportan multiplicación a * b -> T
template <typename T>
concept Multiplicable = requires (T a, T b) {
    { a * b } -> std::same_as<T>;
};

// Numeric: Concept compuesto que requiere todas las operaciones numéricas principales
template <typename T>
concept Numeric = Addable<T> && Divisible<T> && Subtractable<T> && Multiplicable<T>;

// -----------------------------------------------------------------------------
// Función auxiliar: count_elements
// Cuenta los elementos de cualquier contenedor Iterable
// -----------------------------------------------------------------------------
template <Iterable C>
std::size_t count_elements(const C& container) {
    std::size_t count = 0;
    for (const auto& _ : container) {
        (void)_;
        ++count;
    }
    return count;
}

// -----------------------------------------------------------------------------
// 1. sum
// Suma los elementos de un contenedor Iterable cuyos elementos cumplen Addable
// -----------------------------------------------------------------------------
template <Iterable C>
requires Addable<container_value_t<C>>
auto sum(const C& container) {
    using T = container_value_t<C>;
    T result{};
    for (const auto& value : container) {
        result = result + value;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 2. mean
// Calcula el promedio reutilizando 'sum' y requiriendo 'Divisible'.
// Utiliza 'if constexpr' para diferenciar enteros y flotantes.
// -----------------------------------------------------------------------------
template <Iterable C>
requires Addable<container_value_t<C>> && Divisible<container_value_t<C>>
auto mean(const C& container) {
    using T = container_value_t<C>;
    std::size_t n = count_elements(container);
    if (n == 0) return T{};
    
    auto s = sum(container); // Reutilización de sum
    
    if constexpr (std::is_integral_v<T>) {
        // Diferenciación en tiempo de compilación para tipos enteros
        return static_cast<T>(s / n);
    } else {
        // Diferenciación en tiempo de compilación para tipos flotantes y objetos
        return s / n;
    }
}

// -----------------------------------------------------------------------------
// 3. variance
// Calcula la varianza reutilizando 'mean' y restringiendo mediante concepts.
// Utiliza 'if constexpr' para la división final.
// -----------------------------------------------------------------------------
template <Iterable C>
requires Addable<container_value_t<C>> && Divisible<container_value_t<C>> && 
         Subtractable<container_value_t<C>> && Multiplicable<container_value_t<C>>
auto variance(const C& container) {
    using T = container_value_t<C>;
    std::size_t n = count_elements(container);
    if (n == 0) return T{};
    
    auto m = mean(container); // Reutilización de mean
    
    T sum_sq_diff{};
    for (const auto& value : container) {
        auto diff = value - m;
        sum_sq_diff = sum_sq_diff + (diff * diff);
    }
    
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(sum_sq_diff / n);
    } else {
        return sum_sq_diff / n;
    }
}

// -----------------------------------------------------------------------------
// 4. max
// Obtiene el elemento máximo utilizando el concept 'Comparable' y numéricos.
// -----------------------------------------------------------------------------
template <Iterable C>
requires Comparable<container_value_t<C>> && Addable<container_value_t<C>> && Divisible<container_value_t<C>>
auto max(const C& container) {
    using T = container_value_t<C>;
    auto it = std::begin(container);
    auto end_it = std::end(container);
    if (it == end_it) return T{};
    
    T max_val = *it;
    ++it;
    for (; it != end_it; ++it) {
        if (max_val < *it) {
            max_val = *it;
        }
    }
    return max_val;
}

// -----------------------------------------------------------------------------
// 5. transform_reduce
// Aplica una función a cada elemento y reduce los resultados sumándolos.
// La función se recibe como parámetro de tipo genérico/template.
// -----------------------------------------------------------------------------
template <Iterable C, typename Func>
requires Addable<std::invoke_result_t<Func, container_value_t<C>>>
auto transform_reduce(const C& container, Func func) {
    using ResultType = std::invoke_result_t<Func, container_value_t<C>>;
    ResultType acc{};
    bool first = true;
    for (const auto& item : container) {
        if (first) {
            acc = func(item);
            first = false;
        } else {
            acc = acc + func(item);
        }
    }
    return acc;
}

// -----------------------------------------------------------------------------
// 6. Funciones Variadic usando Fold Expressions y Concepts
// -----------------------------------------------------------------------------

// sum_variadic: Fold expression de suma unaria a derecha/izquierda
template <typename T, typename... Args>
requires Addable<std::common_type_t<T, Args...>>
auto sum_variadic(T first, Args... args) {
    using CommonT = std::common_type_t<T, Args...>;
    return (static_cast<CommonT>(first) + ... + static_cast<CommonT>(args));
}

// mean_variadic: Reutiliza sum_variadic y aplica if constexpr con fold expression
template <typename T, typename... Args>
requires Addable<std::common_type_t<T, Args...>> && Divisible<std::common_type_t<T, Args...>>
auto mean_variadic(T first, Args... args) {
    using CommonT = std::common_type_t<T, Args...>;
    constexpr std::size_t n = 1 + sizeof...(Args);
    CommonT total = sum_variadic(first, args...);
    if constexpr (std::is_integral_v<CommonT>) {
        return static_cast<CommonT>(total / n);
    } else {
        return total / n;
    }
}

// variance_variadic: Fold expression para suma de diferencias al cuadrado
template <typename T, typename... Args>
requires Addable<std::common_type_t<T, Args...>> && Divisible<std::common_type_t<T, Args...>> && 
         Subtractable<std::common_type_t<T, Args...>> && Multiplicable<std::common_type_t<T, Args...>>
auto variance_variadic(T first, Args... args) {
    using CommonT = std::common_type_t<T, Args...>;
    constexpr std::size_t n = 1 + sizeof...(Args);
    CommonT m = mean_variadic(first, args...);
    
    // Fold expression sumando (arg - m)^2
    CommonT sq_diff_sum = (((static_cast<CommonT>(first) - m) * (static_cast<CommonT>(first) - m)) + 
                          ... + 
                          ((static_cast<CommonT>(args) - m) * (static_cast<CommonT>(args) - m)));
    
    if constexpr (std::is_integral_v<CommonT>) {
        return static_cast<CommonT>(sq_diff_sum / n);
    } else {
        return sq_diff_sum / n;
    }
}

// max_variadic: Fold expression con operador coma para encontrar el máximo
template <typename T, typename... Args>
requires Comparable<std::common_type_t<T, Args...>> && Addable<std::common_type_t<T, Args...>> && Divisible<std::common_type_t<T, Args...>>
auto max_variadic(T first, Args... args) {
    using CommonT = std::common_type_t<T, Args...>;
    CommonT max_val = static_cast<CommonT>(first);
    auto update_max = [&max_val](CommonT val) {
        if (max_val < val) max_val = val;
    };
    (update_max(static_cast<CommonT>(args)), ...);
    return max_val;
}

} // namespace core_numeric
