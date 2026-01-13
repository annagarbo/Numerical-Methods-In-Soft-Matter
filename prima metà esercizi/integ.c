//
//  integ.c
//  
//
//  Created by Anna Garbo.1 on 05/11/23.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Funzione roh(x)
double roh(double x) {
    return exp(-x);
}

// Funzione g(x, a)
double g(double x, double a) {
    return a * exp(-a * x);
}

// Funzione da integrare f(x, T)
double f(double x, double T) {
    return (x >= T) ? 1.0 : 0.0;
}

int main() {
    srand(1234);  // Inizializza il generatore di numeri casuali con il tempo corrente

    double T = 3.0;
    double a_opt = ((T + 1) - sqrt(1 + T * T)) / T;

    int num_iterations = 0;
    double desired_accuracy = 0.010;  // 1% accuracy
    double estimated_integral = 0.0;
    double estimated_variance = 0.0;
    double sigma = 0.0;

    while (1) {
        num_iterations++;
        int num_samples = 1000;
        double samples[num_samples];

        for (int i = 0; i < num_samples; i++) {
            double U = (double)rand() / RAND_MAX;  // Genera un numero casuale tra 0 e 1
            samples[i] = -log(1 - U) / a_opt;
        }

        double weights[num_samples];

        for (int i = 0; i < num_samples; i++) {
            weights[i] = f(samples[i], T) * roh(samples[i]) / g(samples[i], a_opt);
        }

        estimated_integral = 0.0;
        
        for (int i = 0; i < num_samples; i++) {
            estimated_integral += weights[i];
        }
        
        estimated_integral /= num_samples;

        estimated_variance = 0.0;
        
        for (int i = 0; i < num_samples; i++) {
            estimated_variance += (weights[i] - estimated_integral) * (weights[i] - estimated_integral);
        }
        
        estimated_variance /= num_samples - 1;

        sigma = sqrt(estimated_variance);

        if (sigma < desired_accuracy * fabs(estimated_integral)) {
            break;
        }
    }

    printf("Numerical estimation of the integral: %f\n", estimated_integral);
    printf("Number of iterations: %d\n", num_iterations);
    printf("Estimated Variance: %f\n", sigma);
    printf("Analytical value of the integral: %f\n", exp(-T));

    return 0;
}
