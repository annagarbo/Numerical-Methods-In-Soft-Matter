#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_MAX 100 // Maximum lattice size

// Function to calculate the energy of the system
double calculate_energy(int spins[N_MAX][N_MAX], int N) {
    double energy = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            energy += -spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] + spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
        }
    }
    return energy / 2; // Counted each interaction twice
}

// Function to initialize the spin configuration
void initialize_spins(int spins[N_MAX][N_MAX], int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            spins[i][j] = (rand() % 2) ? 1 : -1; // Randomly assign spin up or down
        }
    }
}

// Function to perform a Metropolis step
void metropolis_step(int spins[N_MAX][N_MAX], int N, double beta) {
    int i = rand() % N;
    int j = rand() % N;
    int delta_energy = 2 * spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] + spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
    if (delta_energy < 0 || ((double)rand() / RAND_MAX) < exp(-beta * delta_energy)) {
        spins[i][j] *= -1;
    }
}

int main() {
    int sizes[] = {25, 50, 100}; // Size of the lattice
    double temperatures[] = {0.5, 1.0, 2.5, 3.0}; // Temperature
    int n_steps = 10000; // Number of steps

    FILE *file;
    file = fopen("ising_simulation_data_10000_25.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    srand(time(NULL)); // Initialize random seed

    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int N = sizes[s];
        int spins[N_MAX][N_MAX];

        for (int t = 0; t < sizeof(temperatures) / sizeof(temperatures[0]); t++) {
            double T = temperatures[t];

            initialize_spins(spins, N);

            // Main simulation loop
            for (int step = 0; step < n_steps; step++) {
                double energy = calculate_energy(spins, N);
                double magnetization = 0;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++) {
                        magnetization += spins[i][j];
                    }
                }

                // Write simulation data to file
                fprintf(file, "%d %d %f %f %f\n", N, step, energy, magnetization, T);

                metropolis_step(spins, N, 1 / T);
            }
        }
    }

    fclose(file);
    return 0;
}


