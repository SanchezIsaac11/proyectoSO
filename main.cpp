#include <iostream>
#include "GestorMemoria.h"

int main() {
    std::cout << "--- Iniciando prueba de ASIGNAR y LIBERAR ---" << std::endl;
    
    // Memoria total de 1024 KB, mínimo de 32 KB
    GestorMemoria memoria(1024, 32); 
    std::cout << "---------------------------------------" << std::endl;

    // Asignamos 4 procesos
    std::cout << "\nPID 1 pide 100 KB (necesita 128)" << std::endl;
    memoria.asignarMemoria(1, 100); // Usa [0] (128)

    std::cout << "\nPID 2 pide 200 KB (necesita 256)" << std::endl;
    memoria.asignarMemoria(2, 200); // Usa [1] (256)

    std::cout << "\nPID 3 pide 30 KB (necesita 32)" << std::endl;
    memoria.asignarMemoria(3, 30);  // Usa [2] (32)

    std::cout << "\nPID 4 pide 50 KB (necesita 64)" << std::endl;
    memoria.asignarMemoria(4, 50);  // Usa [3] (64)
    
    std::cout << "\n--- ESTADO DESPUES DE ASIGNAR ---" << std::endl;
    
    // Memoria usada: 128 + 256 + 32 + 64 = 480 KB
    // Memoria restante: 1024 - 480 = 544 KB
    // (Repartidos en varios bloques libres)


    // --- PRUEBA DE LIBERACION Y FUSION ---
    
    std::cout << "\nLiberando PID 3 (32 KB)..." << std::endl;
    memoria.liberarMemoria(3);
    // (El bloque de 32KB queda libre. Su 'buddy' (otro 32KB) está libre.)
    // (Deberían fusionarse en un bloque de 64KB)

    std::cout << "\nLiberando PID 4 (64 KB)..." << std::endl;
    memoria.liberarMemoria(4);
    // (El bloque de 64KB queda libre. Su 'buddy' (el bloque fusionado de 64KB) está libre.)
    // (Deberían fusionarse en un bloque de 128KB)
    
    std::cout << "\nLiberando PID 1 (128 KB)..." << std::endl;
    memoria.liberarMemoria(1);
    // (El bloque de 128KB queda libre. Su 'buddy' (el bloque fusionado de 128KB) está libre.)
    // (Deberían fusionarse en un bloque de 256KB)

    std::cout << "\nLiberando PID 2 (256 KB)..." << std::endl;
    memoria.liberarMemoria(2);
    // (El bloque de 256KB queda libre. Su 'buddy' (el bloque fusionado de 256KB) está libre.)
    // (Deberían fusionarse en un bloque de 512KB)
    // (Y este 512KB se fusiona con el otro 512KB libre)
    // (Resultado final: ¡un solo bloque libre de 1024KB!)

    std::cout << "\n--- PRUEBA DE RE-ASIGNACION ---" << std::endl;
    std::cout << "\nPID 5 pide 1000 KB (necesita 1024)" << std::endl;
    memoria.asignarMemoria(5, 1000);
    // (Esto debería funcionar ahora, ya que la memoria se fusionó)


    std::cout << "\n---------------------------------------" << std::endl;
    std::cout << "--- Prueba de liberacion terminada ---" << std::endl;
    
    return 0; // El destructor se llamará aquí
}