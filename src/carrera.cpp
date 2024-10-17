#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <atomic>

// Mutex para sincronizar la impresión en la terminal
std::mutex mtx;

// Variable atómica para rastrear el lugar de los autos
std::atomic<int> place(1);

// Función que simula la carrera de un auto
void raceCar(int id, int distance_total) {
    std::random_device rd;                // Semilla aleatoria
    std::mt19937 gen(rd());               // Generador de números aleatorios
    std::uniform_int_distribution<> dist(1, 10);  // Distancia aleatoria entre 1 y 10 metros
    std::uniform_int_distribution<> delay(100, 500); // Tiempo de espera entre 100 y 500 ms

    int distance_covered = 0;  // Distancia total recorrida por el auto

    while (distance_covered < distance_total) {
        int advance = dist(gen); // Generar avance aleatorio
        int wait_time = delay(gen); // Generar tiempo de espera aleatorio

        // Actualizar la distancia recorrida
        distance_covered += advance;
        if (distance_covered > distance_total) {
            distance_covered = distance_total; // Evitar que sobrepase la meta
        }

        // Bloquear la impresión para que solo un hilo imprima a la vez
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Auto" << id << " avanza " << advance << " metros (total: " 
                      << distance_covered << " metros)" << std::endl;
        }

        // Pausa antes de la siguiente iteración (simula la velocidad del auto)
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    }

    // Al llegar a la meta, imprimir el lugar de llegada
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Auto" << id << " ha terminado la carrera en el lugar " << place++ << "!" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Validar la cantidad de argumentos
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_autos>" << std::endl;
        return 1;
    }

    // Parsear los argumentos
    int distance_total = std::stoi(argv[1]);  // Distancia total de la carrera
    int num_cars = std::stoi(argv[2]);        // Número de autos participantes

    // Verificar que los parámetros sean válidos
    if (distance_total <= 0 || num_cars <= 0) {
        std::cerr << "La distancia total y el número de autos deben ser mayores que cero." << std::endl;
        return 1;
    }

    // Vector para almacenar los hilos de los autos
    std::vector<std::thread> threads;

    // Imprimir información inicial
    std::cout << "Distancia total carrera: " << distance_total << " metros" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    // Crear y lanzar los hilos para cada auto
    for (int i = 0; i < num_cars; ++i) {
        threads.emplace_back(raceCar, i, distance_total);  // Crear hilos
    }

    // Esperar que todos los hilos terminen (autos terminen la carrera)
    for (auto& t : threads) {
        t.join();  // Unir los hilos al hilo principal
    }

    // Carrera finalizada
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "¡La carrera ha finalizado!" << std::endl;

    return 0;
}
