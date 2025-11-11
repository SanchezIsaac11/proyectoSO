# proyectoSO

## Descripción  
Este proyecto es una simulación y gestor de componentes básicos de un sistema operativo, implementado en C++:  
- Gestión de memoria  
- Gestión de procesos  
- Simulador de tareas  

El objetivo es servir como práctica académica para la materia de Sistemas Operativos.

## Estructura del Proyecto  
- `main.cpp` — Punto de entrada al programa.  
- `GestorMemoria.h` / `GestorMemoria.cpp` — Módulo para la administración de la memoria (asignación, liberación, etc.).  
- `GestorProcesos.h` / `GestorProcesos.cpp` — Módulo para la creación, planificación y finalización de procesos.  
- `proceso.h` — Definición de la estructura o clase que representa un proceso.  
- `simulador.h` / `simulador.cpp` — Lógica de simulación del sistema operativo, orquestando los gestores.  
- `README.md` — Este archivo, con información general.  
- `ProyectoSO.exe`, `main.exe` — Ejecutables generados (si aplica).

## Requisitos  
- Compilador compatible con C++ (por ejemplo, GCC, Clang o MSVC).  
- Sistema operativo compatible (Windows, Linux o macOS) — verificar paths de archivos y dependencias.  
- Archivo de entrada / configuración (si aplica) para definir los procesos o la memoria simulada.

