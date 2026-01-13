//
//  main.c
//  Numerical Methods in Soft Matter
//
//  Created by Anna Garbo.1 on 02/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int numThrows = 10000000; // You can change the number of "throws" as needed.
    int numHits = 0;

    double a = 1.0;
    double b = 3.0;
    double c = 2.0;
    double d = 5.0;

    // Seed the random number generator.
    srand(12345);

    for (int i = 0; i < numThrows; i++) {
        // Generate random points within the rectangle and circle radious
        double x = (double)rand() / RAND_MAX * (b - a) + a;
        double y = (double)rand() / RAND_MAX * (d - c) + c;
        double r = (double)rand() / RAND_MAX * (a - 0) + 0;
        
        // Check if the point is within the rectangle.
        if (x >= a && x <= b && y >= c && y <= d && r<=a) {
            numHits++;
        }
    }
    printf("%d\n", numHits);
    double estimatedArea_rec = (double)numHits / numThrows * (b - a) * (d - c);
    double estimatedArea_circ = (double)numHits / numThrows * a * a * 3.14159;
    
    printf("Estimated Area Rectangle: %lf\n", estimatedArea_rec);
    printf("Analytic Area Rectangle: %lf\n", (b - a) * (d - c));
    printf("Estimated Area Circle: %lf\n", estimatedArea_circ);
    printf("Analytic Area Circle: %lf\n", a*a*3.14159);
    return 0;
}




