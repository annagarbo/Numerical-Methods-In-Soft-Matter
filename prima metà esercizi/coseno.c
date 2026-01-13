
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

double random_uniform() {
    return (double)rand() / RAND_MAX;
}

// Funzione bersaglio f(x)
double target_function(double x) {
    return cos(x);
}

// Distribuzione di campionamento g(x)
double sampling_distribution(double x) {
    return 1 + 24 / (PI * PI * PI) * (1 - PI / 2) * x * x;
}

// Peso di importance w(x)
double importance_weight(double x) {
    return cos(x) / sampling_distribution(x);
}

// PDF di confronto g(x) con un valore specifico di p
double comparison_pdf(double x, double p) {
    
    if (x < p) {
        return 2*p/(2*p*p+1);
    } else {
        return 2/(2*p*p+1) * x * exp(p * p - x * x);
    }
}

int main() {
    srand(123456789);

    int num_iterations = 0;
    double integral_estimate = 0.0;
    double accuracy_threshold = 0.01;  // Precisione desiderata del 1%
    double exact_value = 1.0;  // Valore esatto dell'integrale
    double previous_estimate = 0.0;
    double p=0.7;
    do {
        double integral_sum = 0.0;

        // Estrarre 1000 punti da g(x) utilizzando accettazione-rigetto
        for (int i = 0; i < 1000; ++i) {
            double x, u, weight, y;
            
            while (1) {
                u = random_uniform();
                y = random_uniform();

                if (u < 2 * p / (2 * p * p + 1)) {
                    x = (2 * p * p + 1) * u / (2 * p);
                } else {
                    x = sqrt(p * p - log(1 - (2 * p * p + 1) * u + 2 * p * p));
                }

                if (y <= target_function(x) / (1.15*comparison_pdf(x, p))) {
                    break;  // Esci dal ciclo while solo se la condizione è soddisfatta
                }
                weight=importance_weight(x);
            }
            integral_sum += weight;
        }

        // Incremento del numero di iterazioni
        num_iterations++;

        // Aggiornamento della stima dell'integrale
        integral_estimate += integral_sum / 1000;

        // Monitoraggio della precisione rispetto al valore esatto
        if (num_iterations > 1) {
            double relative_difference = fabs((integral_estimate - previous_estimate) / previous_estimate);
            if (relative_difference < accuracy_threshold) {
                break;  // La precisione desiderata è stata raggiunta rispetto al valore esatto
            }
        }

        // Aggiornamento della stima precedente
        previous_estimate = integral_estimate;

    } while (1);

    printf("Valore stimato dell'integrale: %f\n", integral_estimate/num_iterations);
    printf("Numero di iterazioni: %d\n", num_iterations);

    // Calcolo e visualizzazione della differenza relativa
    double relative_difference = fabs((integral_estimate/num_iterations - exact_value) / exact_value);
    printf("Differenza Relativa rispetto al valore esatto: %f\n", relative_difference);

    return 0;
}
