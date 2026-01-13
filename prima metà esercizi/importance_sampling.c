#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double random_uniform() {
    return (double)rand() / RAND_MAX;
}

// Funzione f(x) = e^(-x^2) * g(x)
double f(double x, double T) {
    if (x > T) {
        return 1.0;
    } else {
        return 0.0;
    }
}

double g(double x, double a) {
    return a * exp(-a * x);
}

double cumulative_inv(double x, double a) {
    return -1 / a * log(1 - x);
}

int main() {
    int num_samples = 100000;

    double integral_importance = 0.0;
    double T = 3.0;
    double a = 1 + 1 / T - sqrt(1 + 1 / (T * T));

    srand(42321); // Inizializza il generatore di numeri casuali

    for (int i = 0; i < num_samples; i++) {
        double u = random_uniform();

        double x = cumulative_inv(u, a); // Campiona con distribuzione gaussiana
        printf("x %f\n",x)
        // Calcola il contributo all'integrale con il metodo di campionamento di importanza
        integral_importance += f(x, T) / g(x, a);
    }

    // Calcola la media dell'integrale importanza
    integral_importance = integral_importance / num_samples;

    printf("Importance sampling result: %f\n", integral_importance);
    printf("Expected Mean Value: %f\n", exp(-T));
    printf("Relative Error %f\n", fabs(integral_importance - exp(-T)) / exp(-T));

    return 0;
}
