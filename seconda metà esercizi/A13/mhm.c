#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_TEMPERATURES 20

// Funzione per leggere i dati delle energie
double* read_energies(const char* filename, int* num_entries) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return NULL;
    }

    // Contare il numero di righe nel file
    *num_entries = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        (*num_entries)++;
    }
    rewind(file);

    // Allocazione memoria per le energie
    double *energies = (double *)malloc(*num_entries * sizeof(double));
    if (!energies) {
        perror("Unable to allocate memory");
        fclose(file);
        return NULL;
    }

    // Lettura dei dati delle energie
    for (int i = 0; i < *num_entries; i++) {
        fscanf(file, "%lf", &energies[i]); // Supponendo che il file contenga solo energie
    }

    fclose(file);
    return energies;
}

// Funzione per calcolare Z(beta)
double compute_Z_beta(int num_temperatures, double M, double *combined_data, double *Z_k, double *betas, double beta) {
    double Z_beta = 0.0;
    // Estrai le temperature e le energie da combined_data
    double *temperatures = combined_data; // Prima colonna
    double *energies = combined_data + num_temperatures; // Seconda colonna

    // Itera sulle temperature uniche
    for (int i = 0; i < num_temperatures; i++) {
        double temp = temperatures[i];
        double denominator = 0.0;
        for (int j = 0; j < num_temperatures; j++) {
            denominator += M * exp((beta - betas[j]) * energies[i]) / Z_k[j];
        }
        Z_beta += 1.0 / denominator;
    }

    return Z_beta;
}

// Funzione per calcolare E(beta)
double compute_E_beta(double *combined_data, int num_temperatures, double M, double *betas, double *Z_k, double Z_beta, double beta) {
    double E_beta = 0.0;
    // Estrai le temperature e le energie da combined_data
    double *temperatures = combined_data; // Prima colonna
    double *energies = combined_data + num_temperatures; // Seconda colonna

    // Itera sulle temperature uniche
    for (int i = 0; i < num_temperatures; i++) {
        double temp = temperatures[i];
        double denominator = 0.0;
        for (int j = 0; j < num_temperatures; j++) {
            denominator += M / Z_k[j] * exp((beta - betas[j]) * energies[i]);
        }
        E_beta += energies[i] / denominator;
    }

    E_beta /= Z_beta;
    return E_beta;
}

int main() {
    // Variabili per i dati
    int num_entries;
    double *energies = read_energies("energy.txt", &num_entries);
    if (!energies) {
        return 1;
    }

    // Combinare dati: temperature e energie
    double combined_data[NUM_TEMPERATURES][2]; // Supponendo che ci siano NUM_TEMPERATURES temperature e ogni riga ha (temperatura, energia)
    
    // Inizializza le temperature
    double betas[NUM_TEMPERATURES];
    for (int i = 0; i < NUM_TEMPERATURES; i++) {
        combined_data[i][0] = 1.0 + 0.15 * i; // Esempio di temperature da 1.0 a 4.0
        combined_data[i][1] = energies[i % num_entries]; // Esempio: ripetizione delle energie
        betas[i] = 1.0 / combined_data[i][0];
    }

    double Z_final[NUM_TEMPERATURES] = {1.0}; // Inizializza Z a 1.0
    double E_expanded[NUM_TEMPERATURES];

    // Calcola E(beta) per ogni beta
    for (int k = 0; k < NUM_TEMPERATURES; k++) {
        double beta = betas[k];
        
        // Z(beta)
        double Z_beta = compute_Z_beta(NUM_TEMPERATURES, 80000.0, (double *)combined_data, Z_final, betas, beta);
        
        // E(beta)
        E_expanded[k] = compute_E_beta((double *)combined_data, NUM_TEMPERATURES, 80000.0, betas, Z_final, Z_beta, beta);
    }

    // Scrivi i risultati su un file
    FILE *output_file = fopen("results.txt", "w");
    if (!output_file) {
        perror("Unable to open output file");
        free(energies);
        return 1;
    }

    for (int k = 0; k < NUM_TEMPERATURES; k++) {
        fprintf(output_file, "Temperature: %.2f, E(beta): %.6f\n", combined_data[k][0], E_expanded[k]);
    }

    fclose(output_file);
    free(energies);
    return 0;
}

