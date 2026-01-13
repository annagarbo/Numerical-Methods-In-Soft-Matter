#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Function to generate a pair of independent, standard, normally distributed random numbers
void boxMullerTransform(double *z0, double *z1) {
    // Generate two random numbers uniformly distributed between 0 and 1
    double u1 = rand() / (RAND_MAX + 1.0);
    double u2 = rand() / (RAND_MAX + 1.0);

    // Apply the Box-Muller transformation to get standard normal variables
    *z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    *z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
}

int main() {
    // Seed the random number generator
    srand((unsigned)time(NULL));

    // Number of pairs to generate
    int numPairs = 1000;

    // Open the file for writing
    FILE *file = fopen("gauss_dist.csv", "w");

    // Check if the file was opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }

    // Write the header to the file
    fprintf(file, "Pair,Value1,Value2\n");

    // Generate and write pairs of normally distributed random numbers to the file
    for (int i = 0; i < numPairs; ++i) {
        double z0, z1;
        boxMullerTransform(&z0, &z1);
        fprintf(file, "%d,%f,%f\n", i + 1, z0, z1);
    }

    // Close the file
    fclose(file);

    printf("Pairs of normally distributed random numbers written to gauss_dist.csv\n");

    return 0;
}

