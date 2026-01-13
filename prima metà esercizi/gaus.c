//
//  gaus.c
//  
//
//  Created by Anna Garbo.1 on 08/11/23.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Funzione di densità della distribuzione gaussiana standard
double gaussianDensity(double x) {
    return exp(-x * x / 2.0) / sqrt(2.0 * M_PI);
}
// Funzione per generare numeri casuali da una distribuzione gaussiana
double generateGaussianRandom(double x) {
    

    double Z0=sqrt(M_PI/2)*((2*gaussianDensity(x)-1)+M_PI/12*pow(2*gaussianDensity(x)-1,3)+7*M_PI/480*pow(2*gaussianDensity(x)-1,5));

    return Z0;
}

// Funzione da campionare
double targetFunction(double x) {
    return sqrt(x) * exp(-x * x);
}



int main() {
    // Imposta il seme del generatore di numeri casuali
    srand(12345);

    int numSamples = 100; // Numero di campioni
    double integral = 0.0;
    double U1, Z0;
    
    for (int i = 0; i < numSamples; i++) {
        // Genera un campione dalla distribuzione gaussiana standard
        U1 = rand() / (double)RAND_MAX;
        double sample = generateGaussianRandom(U1);
        printf("sample: %lf\n", sample);
        // Calcola il valore della funzione di importanza
        double importanceWeight = targetFunction(fabs(sample));

        // Aggiorna il valore dell'integrale
        integral += importanceWeight;
    }

    // Calcola la stima dell'integrale
    integral /= numSamples*sqrt(2/M_PI);

    printf("Stima dell'integrale usando l'importance sampling: %f\n", integral);


    return 0;
}
