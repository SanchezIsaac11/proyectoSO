// ---------------------------------------------------------------
// INICIO - Asegúrate de que estas líneas estén al principio
// ---------------------------------------------------------------
#include "GestorMemoria.h" // ¡Muy importante incluir nuestro archivo .h!
#include <iostream>        // Para std::cout y std::endl
#include <cmath>           // Para la función 'pow' (aunque la usamos con *)
#include <string>          // Para std::string
// ---------------------------------------------------------------
// FIN - De los includes
// ---------------------------------------------------------------


// --- PASO 4: Implementación del Constructor y Destructor ---

// Constructor
GestorMemoria::GestorMemoria(int tamano_total, int _tamano_minimo) {
    this->raiz = new BloqueMemoria(tamano_total);
    this->tamano_minimo = _tamano_minimo;
    
    std::cout << "Gestor de Memoria inicializado. Tamaño total: " 
              << tamano_total << " KB." << std::endl;
}

// Destructor
GestorMemoria::~GestorMemoria() {
    std::cout << "Liberando toda la memoria del gestor..." << std::endl;
    limpiarRecursivo(this->raiz);
    this->raiz = nullptr;
    std::cout << "Memoria liberada." << std::endl;
}

// Función de ayuda para el Destructor
void GestorMemoria::limpiarRecursivo(BloqueMemoria* nodo) {
    if (nodo == nullptr) {
        return; 
    }
    limpiarRecursivo(nodo->hijo_izquierdo);
    limpiarRecursivo(nodo->hijo_derecho);
    delete nodo;
}


// --- PASO 5: Implementar 'dividirBloque' ---
void GestorMemoria::dividirBloque(BloqueMemoria* bloque_padre) {
    
    if (bloque_padre->hijo_izquierdo != nullptr) {
        return; 
    }

    int tamano_hijo = bloque_padre->tamano / 2;
    if (tamano_hijo < this->tamano_minimo) {
        return; 
    }

    bloque_padre->esta_libre = false; 
    bloque_padre->hijo_izquierdo = new BloqueMemoria(tamano_hijo, bloque_padre);
    bloque_padre->hijo_derecho = new BloqueMemoria(tamano_hijo, bloque_padre);
}


// --- PASO 6 (Parte A): Implementar 'asignarMemoria' (La función PÚBLICA) ---
bool GestorMemoria::asignarMemoria(int id_proceso, int tamano_requerido) {
    
    int tamano_buddy = this->tamano_minimo; // Empezamos en 32 KB
    
    while (tamano_buddy < tamano_requerido) {
        tamano_buddy *= 2; 
    }

    if (tamano_buddy > this->raiz->tamano) {
        std::cout << "ERROR: Proceso " << id_proceso << " pide " << tamano_requerido
                  << "KB (necesita " << tamano_buddy << "KB), pero el total es " 
                  << this->raiz->tamano << "KB." << std::endl;
        return false;
    }

    BloqueMemoria* bloque_encontrado = buscarBloqueLibre(this->raiz, tamano_buddy);

    if (bloque_encontrado != nullptr) {
        bloque_encontrado->esta_libre = false;
        bloque_encontrado->id_proceso_asignado = id_proceso;

        std::cout << "MEMORIA: Proceso " << id_proceso << " asignado a bloque de " 
                  << bloque_encontrado->tamano << "KB." << std::endl;
        return true;
    } else {
        std::cout << "MEMORIA: No hay espacio para Proceso " << id_proceso 
                  << " (necesita " << tamano_buddy << "KB)." << std::endl;
        return false;
    }
}


// --- PASO 6 (Parte B): Implementar 'buscarBloqueLibre' (La función PRIVADA recursiva) ---
BloqueMemoria* GestorMemoria::buscarBloqueLibre(BloqueMemoria* nodo, int tamano_requerido) {
    
    // --- CASOS DE FALLO (Podar el árbol) ---

    // 1. Si el nodo no existe (llegamos al final de una rama)
    if (nodo == nullptr) {
        return nullptr;
    }

    // 2. Si el nodo está ocupado
    if (!nodo->esta_libre) {
        return nullptr;
    }

    // 3. Si el nodo es más pequeño de lo que necesitamos
    if (nodo->tamano < tamano_requerido) {
        return nullptr;
    }

    // --- CASOS DE ÉXITO O BÚSQUEDA ---

    // 4. ¡CASO DE ÉXITO! El bloque es del tamaño exacto y está libre
    // (Y es una "hoja", no un padre dividido)
    if (nodo->tamano == tamano_requerido && nodo->hijo_izquierdo == nullptr) {
        return nodo; // Encontramos el bloque perfecto
    }

    // 5. CASO DE DIVISIÓN. El bloque es una "hoja" libre, pero es demasiado grande.
    if (nodo->tamano > tamano_requerido && nodo->hijo_izquierdo == nullptr) {
        
        // ¡Ojo! No podemos dividir si el resultado es menor al mínimo
        if ((nodo->tamano / 2) < tamano_requerido) {
           // Ej: Pedimos 100 (necesitamos 128). Encontramos 128.
           // Pero si hubiéramos pedido 100 y encontramos 64...
           // Este caso es complejo.
           // Si pedimos 33 (necesitamos 64) y encontramos 64.
           // (64 / 2) = 32. 32 < 33. No podemos dividir.
           // Ah, pero tamano_requerido ya es potencia de 2.
           // Si pedimos 33 (tamano_requerido=64). Encontramos 64.
           // Se va al caso 4.
           
           // Si pedimos 33 (tamano_req=64). Encontramos 128.
           // 128 > 64. 128 / 2 = 64. 64 !< 64.
           // Entonces SÍ dividimos.
           
           // Si pedimos 17 (tamano_req=32). Encontramos 64.
           // 64 > 32. 64 / 2 = 32. 32 !< 32.
           // Entonces SÍ dividimos.
           
           // La única vez que esta lógica se activa es si
           // tamano_minimo es, por ej, 64 y pedimos 33.
           // Pero tamano_requerido se vuelve 64.
           // La lógica de tamano_minimo en 'dividirBloque' es suficiente.
        }

        // Dividimos el bloque (¡usando el Paso 5!)
        dividirBloque(nodo);

        // Después de dividir, recursivamente buscamos en el hijo izquierdo
        return buscarBloqueLibre(nodo->hijo_izquierdo, tamano_requerido);
    }

    // 6. CASO DE BÚSQUEDA. El bloque es un "padre" (ya está dividido).
    if (nodo->hijo_izquierdo != nullptr) {
        BloqueMemoria* bloque_izq = buscarBloqueLibre(nodo->hijo_izquierdo, tamano_requerido);
        
        if (bloque_izq != nullptr) {
            return bloque_izq;
        }
        
        return buscarBloqueLibre(nodo->hijo_derecho, tamano_requerido);
    }

    // Si encontramos un bloque libre del tamaño exacto pero está dividido
    // (lo cual no debería pasar si la lógica de fusión es correcta)
    // o si algo más falla.
    return nullptr;
}



// --- Implementaciones Pendientes (Pasos 7-8) ---

// ... (Todo tu código anterior: includes, Constructores, dividirBloque, asignarMemoria, etc.) ...


// --- PASO 7: Implementar 'fusionarBloques' (La función PRIVADA recursiva) ---
//
// Esta es la segunda función NÚCLEO. Se llama después de liberar un bloque
// para intentar fusionarlo con su "buddy" y crear un bloque más grande.
void GestorMemoria::fusionarBloques(BloqueMemoria* bloque) {
    
    // --- CASOS BASE (Cuándo detener la recursión) ---

    // 1. Si es el bloque raíz, no tiene padre ni buddy. No hay nada que fusionar.
    if (bloque->padre == nullptr) {
        return;
    }

    // 2. Determinar quién es el "buddy"
    BloqueMemoria* padre = bloque->padre;
    BloqueMemoria* buddy = nullptr;

    if (bloque == padre->hijo_izquierdo) {
        // 'bloque' es el hijo izquierdo, su 'buddy' es el derecho
        buddy = padre->hijo_derecho;
    } else {
        // 'bloque' es el hijo derecho, su 'buddy' es el izquierdo
        buddy = padre->hijo_izquierdo;
    }

    // 3. Si el 'buddy' no existe (error raro) o NO está libre, no podemos fusionar.
    if (buddy == nullptr || !buddy->esta_libre) {
        return;
    }

    // --- LÓGICA DE FUSIÓN ---
    // ¡Éxito! 'bloque' y su 'buddy' están ambos libres.
    
    std::cout << "MEMORIA: Fusionando dos bloques de " << bloque->tamano 
              << "KB para crear uno de " << padre->tamano << "KB." << std::endl;

    // 1. Borrar los dos nodos hijos
    delete padre->hijo_izquierdo;
    delete padre->hijo_derecho;

    // 2. Marcar al padre como "hoja" (sin hijos) y "libre"
    padre->hijo_izquierdo = nullptr;
    padre->hijo_derecho = nullptr;
    padre->esta_libre = true;
    
    // 3. ¡Llamada recursiva! Intentar fusionar al padre con SU buddy
    // 
    fusionarBloques(padre); 
}


// --- PASO 8 (Parte A): Implementar 'buscarBloquePorID' (Función PRIVADA de ayuda) ---
//
// Recorre el árbol buscando un bloque que tenga un id_proceso específico.
BloqueMemoria* GestorMemoria::buscarBloquePorID(BloqueMemoria* nodo, int id_proceso) {
    // Caso base 1: El nodo no existe
    if (nodo == nullptr) {
        return nullptr;
    }

    // Caso base 2: ¡Encontrado! Es una hoja y tiene el ID.
    if (nodo->hijo_izquierdo == nullptr && nodo->id_proceso_asignado == id_proceso) {
        return nodo;
    }

    // Caso recursivo: No es una hoja, buscar en los hijos
    if (nodo->hijo_izquierdo != nullptr) {
        BloqueMemoria* encontrado_izq = buscarBloquePorID(nodo->hijo_izquierdo, id_proceso);
        if (encontrado_izq != nullptr) {
            return encontrado_izq;
        }

        BloqueMemoria* encontrado_der = buscarBloquePorID(nodo->hijo_derecho, id_proceso);
        if (encontrado_der != nullptr) {
            return encontrado_der;
        }
    }
    
    // No se encontró en esta rama
    return nullptr;
}


// --- PASO 8 (Parte B): Implementar 'liberarMemoria' (La función PÚBLICA) ---
//
// Esta es la función que llamará el simulador (main) para liberar un proceso.
bool GestorMemoria::liberarMemoria(int id_proceso) {
    
    // 1. Encontrar el bloque que queremos liberar
    BloqueMemoria* bloque_a_liberar = buscarBloquePorID(this->raiz, id_proceso);

    // 2. Verificar si se encontró
    if (bloque_a_liberar == nullptr) {
        std::cout << "ERROR: No se pudo liberar el Proceso " << id_proceso 
                  << ". No se encontró en memoria." << std::endl;
        return false;
    }

    // 3. Marcar el bloque como libre
    bloque_a_liberar->esta_libre = true;
    bloque_a_liberar->id_proceso_asignado = 0; // Quitar el ID del proceso

    std::cout << "MEMORIA: Proceso " << id_proceso << " liberado del bloque de "
              << bloque_a_liberar->tamano << "KB." << std::endl;

    // 4. Intentar fusionar el bloque liberado con su buddy (y hacia arriba)
    fusionarBloques(bloque_a_liberar);

    return true;
}


// --- Implementaciones Pendientes (Paso 15) ---

std::string GestorMemoria::obtenerEstadoMemoria() {
    // --- Lógica del PASO 15 irá aquí ---
    return "[Estado de memoria no implementado]";
}