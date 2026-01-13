
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_ITER 1000000

double rand_uniform() {
    return (double)rand() / RAND_MAX;
}

void gillespie_lotka_volterra(double k1, double k2, double k3, int prey, int predators, double T, double *times, int *prey_count, int *predator_count) {
    double t = 0.0;
    int i = 0;

    while (t < T && i < MAX_ITER) {
        double rate1 = k1 * prey;
        double rate2 = k2 * prey * predators;
        double rate3 = k3 * predators;

        double total_rate = rate1 + rate2 + rate3;
        if (total_rate <= 0) break;

        double r1 = rand_uniform();
        double r2 = rand_uniform();

        t += -log(r1) / total_rate;

        if (r2 < rate1 / total_rate) {
            prey++;
        } else if (r2 < (rate1 + rate2) / total_rate) {
            prey--;
            predators++;
        } else {
            predators--;
        }

        if (i < MAX_ITER) {
            times[i] = t;
            prey_count[i] = prey;
            predator_count[i] = predators;
            i++;
        } else {
            break; // Ensure we don't go out of bounds
        }
    }
}

void gillespie_brusselator(double a, double b, int X, int Y, double volume, double T, double *times, int *X_count, int *Y_count) {
    double t = 0.0;
    int i = 0;

    while (t < T && i < MAX_ITER) {
        double rate1 = a * volume;
        double rate2 = b * X;
        double rate3 = X * X * Y / volume;
        double rate4 = X;

        double total_rate = rate1 + rate2 + rate3 + rate4;
        if (total_rate <= 0) break;

        double r1 = rand_uniform();
        double r2 = rand_uniform();

        t += -log(r1) / total_rate;

        if (r2 < rate1 / total_rate) {
            X++;
        } else if (r2 < (rate1 + rate2) / total_rate) {
            X--;
            Y++;
        } else if (r2 < (rate1 + rate2 + rate3) / total_rate) {
            X -= 2;
            Y++;
        } else {
            X++;
        }

        if (i < MAX_ITER) {
            times[i] = t;
            X_count[i] = X;
            Y_count[i] = Y;
            i++;
        } else {
            break; // Ensure we don't go out of bounds
        }
    }
}

int main() {
    srand(time(NULL));

    // Parameters for Lotka-Volterra model
    double k1 = 3.0, k2 = 0.01, k3 = 5.0;
    int prey = 50, predators = 20;
    double T = 100.0;

    double times[MAX_ITER] = {0};  // Initialize arrays
    int prey_count[MAX_ITER] = {0};
    int predator_count[MAX_ITER] = {0};

    gillespie_lotka_volterra(k1, k2, k3, prey, predators, T, times, prey_count, predator_count);

    FILE *lv_out = fopen("lotka_volterra_output.csv", "w");
    if (lv_out != NULL) {
        fprintf(lv_out, "Time,Prey,Predators\n");
        for (int i = 0; i < MAX_ITER && times[i] > 0; i++) {
            fprintf(lv_out, "%f,%d,%d\n", times[i], prey_count[i], predator_count[i]);
        }
        fclose(lv_out);
    } else {
        printf("Error: Could not open lotka_volterra_output.csv for writing.\n");
    }

    // Parameters for Brusselator model
    double a = 2.0, b = 5.0;
    int X = 30, Y = 30;
    double volume = 1000;

    gillespie_brusselator(a, b, X, Y, volume, T, times, prey_count, predator_count);

    FILE *br_out = fopen("brusselator_output.csv", "w");
    if (br_out != NULL) {
        fprintf(br_out, "Time,X,Y\n");
        for (int i = 0; i < MAX_ITER && times[i] > 0; i++) {
            fprintf(br_out, "%f,%d,%d\n", times[i], prey_count[i], predator_count[i]);
        }
        fclose(br_out);
    } else {
        printf("Error: Could not open brusselator_output.csv for writing.\n");
    }

    return 0;
}
