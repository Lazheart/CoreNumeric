# CoreNumeric

Librería numérica genérica para C++20, construida sobre concepts, variadic templates y fold expressions. Provee un conjunto de algoritmos numéricos (suma, promedio, varianza, máximo, transform-reduce) con restricciones de tipo verificadas en tiempo de compilación.

## Descripción

CoreNumeric explora cómo diseñar una API numérica genérica, segura y expresiva usando las herramientas modernas de C++20. En lugar de sobrecargas o checks en tiempo de ejecución, cada algoritmo declara explícitamente qué debe cumplir un tipo para poder usarlo, y el compilador rechaza cualquier uso que no cumpla esas condiciones.

```cpp
std::vector<double> v{1.0, 2.0, 3.0};

auto s = core_numeric::sum(v);        // 6.0
auto m = core_numeric::mean(v);       // 2.0
auto var = core_numeric::variance(v); // 0.666...
auto mx = core_numeric::max(v);       // 3.0

auto sq = core_numeric::transform_reduce(v, [](double x) { return x * x; });

auto s1 = core_numeric::sum_variadic(1, 2, 33, 4);
```

## Arquitectura

La librería es *header-only* y está organizada en tres capas:

```
┌─────────────────────────────┐
│   Algoritmos públicos        │  sum, mean, variance, max,
│   (core_numeric::*)          │  transform_reduce, *_variadic
├─────────────────────────────┤
│   Concepts                   │  Addable, Divisible, Iterable,
│   (restricciones de tipo)    │  + concept propio
├─────────────────────────────┤
│   Soporte de compilación     │  if constexpr, variadic
│                               │  templates, fold expressions
└─────────────────────────────┘
```

**Concepts** — restringen en tiempo de compilación qué tipos puede aceptar cada algoritmo:

| Concept | Restringe | Usado en |
|---|---|---|
| `Addable` | Tipos que soportan `+` | `sum`, variadic, `transform_reduce` |
| `Divisible` | Tipos que soportan `/` por un entero | `mean` |
| `Iterable` | Tipos recorribles (`begin`/`end`) | contenedores en general |

Sobre estos concepts se construyen los algoritmos de mayor nivel: `mean` reutiliza `sum`, y `variance` reutiliza `mean`, cada uno añadiendo solo la restricción adicional que necesita.

**Diferenciación por tipo** — donde el comportamiento numérico difiere entre enteros y flotantes, la rama se resuelve en tiempo de compilación con `if constexpr`, sin costo ni ramas en tiempo de ejecución.

**Interfaz variadic** — las versiones `*_variadic` (`sum_variadic`, `mean_variadic`, `variance_variadic`, `max_variadic`) aceptan un número arbitrario de argumentos directamente, implementadas con variadic templates y fold expressions sobre los mismos concepts.

Todo el código vive dentro del namespace `core_numeric`.

## Estructura del repositorio

```
CoreNumeric/
├── src/
│   └── core_numeric.cpp   # Concepts + algoritmos
├── test.cpp                # Casos de uso válidos e inválidos
├── Makefile
└── README.md
```

## Casos de uso y validación de tipos

`test.cpp` documenta, con comentarios, tanto los usos válidos como los inválidos: tipos fundamentales (`int`, `double`, `char`, `std::string`) y tipos definidos por el usuario (`Coordenadas`, `Vector3D`, `Esfera`), señalando qué concept falla en cada caso que no compila.

## Requisitos

- Compilador con soporte para C++20 (GCC 10+, Clang 12+, MSVC 19.29+)

## Compilación

```bash
make run
```
