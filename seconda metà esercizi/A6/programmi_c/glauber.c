
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 25  // Dimensione della griglia
#define T 1  // Temperatura
#define N_STEPS 100000 // Numero di passi

// Funzione per inizializzare i valori degli spin
void initialize_spins(int spins[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            spins[i][j] = (rand() % 2) * 2 - 1;  // Assegna -1 o 1
        }
    }
}

// Funzione per calcolare l'energia del sistema
double calculate_energy(int spins[N][N]) {
    double energy = 0.0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            energy -= spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] +
                                     spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
        }
    }
    return energy / 2.0;  // Ogni interazione è stata contata due volte
}

// Funzione per eseguire un passo di Metropolis
void metropolis_step(int spins[N][N], double beta) {
    int i = rand() % N;
    int j = rand() % N;
    int delta_energy = 2 * spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] +
                                          spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
    if (delta_energy < 0 || ((double)rand() / RAND_MAX) < exp(-beta * delta_energy)) {
        spins[i][j] *= -1;
    }
}

// Funzione per eseguire un passo di Glauber
void glauber_step(int spins[N][N], double beta) {
    int i = rand() % N;
    int j = rand() % N;
    int delta_energy = 2 * spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] +
                                          spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
    double p_flip = 1.0 / (1.0 + exp(-2 * beta * delta_energy));
    
    // Generare un numero casuale e decidere se flipare lo spin
        if ((double)rand() / RAND_MAX < p_flip) {
            spins[i][j] = 1;
        } else {
            spins[i][j] = -1;
        }
    
}

int main() {
    srand((unsigned)time(NULL));  // Inizializza il generatore di numeri casuali

    int spins[N][N];
    initialize_spins(spins);

    FILE *file = fopen("ising_data.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    double beta = 1.0 / T;
    for (int step = 0; step < N_STEPS; ++step) {
        metropolis_step(spins, beta);
        double energy_metropolis = calculate_energy(spins);
        double magnetization_metropolis = 0.0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                magnetization_metropolis += spins[i][j];
            }
        }
        

        fprintf(file, "Metropolis %d %lf %lf\n", step, energy_metropolis, magnetization_metropolis);

        glauber_step(spins, beta);
        double energy_glauber = calculate_energy(spins);
        double magnetization_glauber = 0.0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                magnetization_glauber += spins[i][j];
            }
        }
        

        fprintf(file, "Glauber %d %lf %lf\n", step, energy_glauber, magnetization_glauber);
    }

    fclose(file);
    return 0;
}
