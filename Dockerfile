# Usar Debian Bookworm Slim como imagen base
FROM debian:bookworm-slim

# Instalar g++ con soporte C++20 y make
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

# Establecer directorio de trabajo
WORKDIR /app

# Copiar el código fuente del proyecto
COPY . .

# Compilar la librería, la demostración main y el test suite
RUN make clean && make all

# Comando por defecto al ejecutar el contenedor
CMD ["sh", "-c", "./programa && ./test_prog"]
