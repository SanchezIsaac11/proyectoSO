#pragma once
#include <string> // Necesario para la función obtenerEstadoMemoria

// --- PASO 3: Definir la Estructura del Árbol de Memoria ---
// (Este es tu código, ¡está perfecto!)
struct BloqueMemoria {
    // --- Atributos de Estado ---
    int id_proceso_asignado; // ID del proceso que ocupa este bloque.
                             // 0 (o -1) significa que está libre.
    
    int tamano;              // Tamaño en KB de este bloque (ej. 4096, 2048...)
    
    bool esta_libre;         // true si el bloque NO está asignado a un proceso.
                             // Un bloque puede estar 'libre=false' pero no tener
                             // un ID de proceso si está DIVIDIDO.
    
    // --- Atributos del Árbol ---
    BloqueMemoria* padre;
    BloqueMemoria* hijo_izquierdo;
    BloqueMemoria* hijo_derecho;

    // --- Constructor para el nodo RAÍZ ---
    // (Se usa solo una vez para crear la memoria completa)
    BloqueMemoria(int _tamano)
        : id_proceso_asignado(0),
          tamano(_tamano),
          esta_libre(true),
          padre(nullptr),
          hijo_izquierdo(nullptr),
          hijo_derecho(nullptr) {}

    // --- Constructor para los nodos HIJOS ---
    // (Se usa cada vez que dividimos un bloque)
    BloqueMemoria(int _tamano, BloqueMemoria* _padre)
        : id_proceso_asignado(0),
          tamano(_tamano),
          esta_libre(true),
          padre(_padre),
          hijo_izquierdo(nullptr),
          hijo_derecho(nullptr) {}
}; // <--- ¡Tu código termina aquí!


// --- PASO 4 (Parte A): Definir la Clase GestorMemoria ---
//
// ESTA ES LA PARTE QUE FALTABA.
// Esta clase le dice al compilador "Oye, existe algo
// llamado GestorMemoria y tiene estas funciones".
//
class GestorMemoria {
private:
    // --- Miembros Privados ---
    BloqueMemoria* raiz;
    int tamano_minimo;

    // --- Funciones de Ayuda Privadas ---
    BloqueMemoria* buscarBloqueLibre(BloqueMemoria* nodo, int tamano_requerido);
    void dividirBloque(BloqueMemoria* bloque_a_dividir);
    void fusionarBloques(BloqueMemoria* bloque_liberado);
    BloqueMemoria* buscarBloquePorID(BloqueMemoria* nodo, int id_proceso);
    void imprimirEstadoRecursivo(BloqueMemoria* nodo, std::string& output);
    void limpiarRecursivo(BloqueMemoria* nodo);

public:
    // --- Funciones Públicas ---
    GestorMemoria(int tamano_total, int _tamano_minimo = 32);
    ~GestorMemoria();
    bool asignarMemoria(int id_proceso, int tamano_requerido);
    bool liberarMemoria(int id_proceso);
    std::string obtenerEstadoMemoria();
}; // <--- Y debe terminar con este punto y coma.