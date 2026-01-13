#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 16         // Dimensione della griglia N x N
#define EQ_STEPS 1024 // Numero di sweep per l'equilibrio
#define MC_STEPS 1024 // Numero di sweep per la misurazione
#define NT 88         // Numero di punti di temperatura
#define TARGET_T 2.5  // Temperatura di interesse

// Funzione per generare un numero casuale tra 0 e 1
double rand_double() {
    return (double)rand() / (double)RAND_MAX;
}

// Funzione per inizializzare lo stato della griglia
void initialstate(int config[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            config[i][j] = 2 * (rand() % 2) - 1; // -1 o 1
        }
    }
}

// Funzione per eseguire un passo di Monte Carlo
void mcmove(int config[N][N], double beta) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int a = rand() % N;
            int b = rand() % N;
            int s = config[a][b];
            int delta_energy = config[(a+1)%N][b] + config[a][(b+1)%N] +
                               config[(a-1+N)%N][b] + config[a][(b-1+N)%N];
            int cost = 2 * s * delta_energy;
            // Applicazione dell'algoritmo di Metropolis
            if (cost < 0) {
                s *= -1;
            } else if (rand_double() < exp(-cost * beta)) {
                s *= -1;
            }
            config[a][b] = s;
        }
    }
}

// Funzione per calcolare l'energia
double calcEnergy(int config[N][N]) {
    double energy = 0.0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int S = config[i][j];
            int nb = config[(i+1)%N][j] + config[i][(j+1)%N] +
                     config[(i-1+N)%N][j] + config[i][(j-1+N)%N];
            energy += -nb * S;
        }
    }
    return energy / (2);
}

int main() {
    FILE *fileE = fopen("energy_target_temp.txt", "w");
    FILE *fileTimeEvol = fopen("energy_evolution_T2.5.txt", "w");
    
    if (fileE == NULL || fileTimeEvol == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    double T[NT];
    double E[NT];
    double n1 = 1.0 / (MC_STEPS * N * N);

    for (int i = 0; i < NT; i++) {
        T[i] = 1.0 + i * (3.28 - 1.0) / (NT - 1);
    }

    for (int tt = 0; tt < NT; tt++) {
        double E1 = 0.0;
        int config[N][N];
        double iT = 1.0 / T[tt];

        initialstate(config);

        for (int i = 0; i < EQ_STEPS; i++) {
            mcmove(config, iT);
        }

        for (int i = 0; i < MC_STEPS; i++) {
            mcmove(config, iT);
            double Ene = calcEnergy(config);
            E1 += Ene*n1;

            // Scrive l'evoluzione temporale dell'energia solo per T = 2.5
            
            if (fabs(T[tt] - TARGET_T) < 1e-2) {
                fprintf(fileTimeEvol, "%d\t%f\n", i, E1);
            }
        }

        E[tt] =  E1;

        fprintf(fileE, "%f\t%f\n", T[tt], E[tt]);
    }

    fclose(fileE);
    fclose(fileTimeEvol);

    return EXIT_SUCCESS;
}

