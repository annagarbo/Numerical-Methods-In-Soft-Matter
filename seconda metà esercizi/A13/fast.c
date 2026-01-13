#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 16         // Dimensione della griglia N x N
#define EQ_STEPS 1024 // Numero di sweep per l'equilibrio
#define MC_STEPS 1024 // Numero di sweep per la misurazione
#define NT 88         // Numero di punti di temperatura

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
    return energy / 4.0;
}

// Funzione per calcolare la magnetizzazione
double calcMag(int config[N][N]) {
    double mag = 0.0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mag += config[i][j];
        }
    }
    return mag;
}

int main() {
    FILE *fileE = fopen("energy.txt", "w");
    FILE *fileM = fopen("magnetization.txt", "w");
    FILE *fileC = fopen("specific_heat.txt", "w");
    FILE *fileX = fopen("susceptibility.txt", "w");

    if (fileE == NULL || fileM == NULL || fileC == NULL || fileX == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    double T[NT];
    double E[NT], M[NT], C[NT], X[NT];
    double n1 = 1.0 / (MC_STEPS * N * N);
    double n2 = 1.0 / (MC_STEPS * MC_STEPS * N * N);

    for (int i = 0; i < NT; i++) {
        T[i] = 1.0 + i * (3.28 - 1.0) / (NT - 1);
    }

    for (int tt = 0; tt < NT; tt++) {
        double E1 = 0.0, M1 = 0.0, E2 = 0.0, M2 = 0.0;
        int config[N][N];
        double iT = 1.0 / T[tt];
        double iT2 = iT * iT;
        
        initialstate(config);

        for (int i = 0; i < EQ_STEPS; i++) {
            mcmove(config, iT);
        }

        for (int i = 0; i < MC_STEPS; i++) {
            mcmove(config, iT);
            double Ene = calcEnergy(config);
            double Mag = calcMag(config);
            E1 += Ene;
            M1 += Mag;
            M2 += Mag * Mag;
            E2 += Ene * Ene;
        }

        E[tt] = n1 * E1;
        M[tt] = n1 * M1;
        C[tt] = (n1 * E2 - n2 * E1 * E1) * iT2;
        X[tt] = (n1 * M2 - n2 * M1 * M1) * iT;

        fprintf(fileE, "%f\t%f\n", T[tt], E[tt]);
        fprintf(fileM, "%f\t%f\n", T[tt], fabs(M[tt]));
        fprintf(fileC, "%f\t%f\n", T[tt], C[tt]);
        fprintf(fileX, "%f\t%f\n", T[tt], X[tt]);
    }

    fclose(fileE);
    fclose(fileM);
    fclose(fileC);
    fclose(fileX);

    return EXIT_SUCCESS;
}

