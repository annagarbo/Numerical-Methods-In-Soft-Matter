#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100           // Number of particles
#define sigma 1.0       // Diameter of hard sphere
#define K_B 1.0         // Boltzmann constant

typedef struct {
    double x, y, z;
} Particle;

Particle particles[N];
double L;  // Simulation box length
double temperature = 1.0;  // Temperature (irrelevant for hard spheres)

// Initialize random number generator
double rand_uniform() {
    return (double)rand() / RAND_MAX;
}

// Apply periodic boundary conditions
double apply_PBC(double coord) {
    if (coord >= L) coord -= L;
    if (coord < 0.0) coord += L;
    return coord;
}

// Compute squared distance between two particles with periodic boundary conditions
double distance_squared(Particle p1, Particle p2) {
    double dx = fabs(p1.x - p2.x);
    double dy = fabs(p1.y - p2.y);
    double dz = fabs(p1.z - p2.z);
    
    if (dx > L / 2) dx = L - dx;
    if (dy > L / 2) dy = L - dy;
    if (dz > L / 2) dz = L - dz;

    return dx * dx + dy * dy + dz * dz;
}

// Check for overlap
int has_overlap(int i) {
    for (int j = 0; j < N; j++) {
        if (i != j) {
            double dist_sq = distance_squared(particles[i], particles[j]);
            if (dist_sq < sigma * sigma) {
                return 1;  // Overlap detected
            }
        }
    }
    return 0;  // No overlap
}

// Compute total energy
double total_energy() {
    for (int i = 0; i < N; i++) {
        if (has_overlap(i)) {
            return 1e6;  // Very large energy for overlap
        }
    }
    return 0.0;  // Zero energy if no overlaps
}

// Perform a single Monte Carlo trial move
int monte_carlo_move(double dmax) {
    int i = rand() % N;
    Particle old_pos = particles[i];

    // Propose random displacement in x, y, z directions
    double dx = dmax * (2.0 * rand_uniform() - 1.0);
    double dy = dmax * (2.0 * rand_uniform() - 1.0);
    double dz = dmax * (2.0 * rand_uniform() - 1.0);

    // Displace particle
    particles[i].x = apply_PBC(particles[i].x + dx);
    particles[i].y = apply_PBC(particles[i].y + dy);
    particles[i].z = apply_PBC(particles[i].z + dz);

    // Check for overlaps after the move
    if (has_overlap(i)) {
        // Reject the move (restore the old position)
        particles[i] = old_pos;
        return 0;  // Move rejected
    }
    return 1;  // Move accepted
}

// Perform a Monte Carlo sweep (N trial moves)
void monte_carlo_sweep(double dmax, int* accepted_moves, int* total_moves) {
    for (int i = 0; i < N; i++) {
        (*total_moves)++;
        *accepted_moves += monte_carlo_move(dmax);
    }
}

// Initialize the particle positions randomly within the 3D box
void initialize_particles() {
    for (int i = 0; i < N; i++) {
        particles[i].x = rand_uniform() * L;
        particles[i].y = rand_uniform() * L;
        particles[i].z = rand_uniform() * L;
    }
}

// Initialize particles in a simple cubic lattice
void initialize_cubic_lattice() {
    double spacing = sigma;  // Set the spacing to the diameter
    for (int i = 0; i < N; i++) {
        particles[i].x = (i % 10) * spacing;  // Assume a 10x10x1 lattice
        particles[i].y = ((i / 10) % 10) * spacing;
        particles[i].z = (i / 100) * spacing;
    }
}

int main() {
    // Seed random number generator
    srand(time(NULL));

    // Array of dmax values and densities
    double dmax_values[] = {0.01, 0.1, 0.5, 0.75, 1.0};
    double densities[] = {0.05, 0.3, 0.5, 1.0};
    int num_dmax = sizeof(dmax_values) / sizeof(dmax_values[0]);
    int num_densities = sizeof(densities) / sizeof(densities[0]);
    int num_realizations = 10; // Number of realizations

    for (int d = 0; d < num_densities; d++) {
        for (int dm = 0; dm < num_dmax; dm++) {
            double rho = densities[d];
            L = cbrt(N / rho);  // Set simulation box size based on density

            double dmax = dmax_values[dm];
            double total_energy_sum = 0.0;
            int total_accepted_moves = 0;
            int total_moves = 0;

            for (int realization = 0; realization < num_realizations; realization++) {
                initialize_particles(); // For random initialization
                // initialize_cubic_lattice(); // Uncomment for cubic lattice initialization

                int num_sweeps = 1000; // Number of Monte Carlo sweeps
                for (int sweep = 0; sweep < num_sweeps; sweep++) {
                    int accepted_moves = 0;
                    monte_carlo_sweep(dmax, &accepted_moves, &total_moves);
                    total_accepted_moves += accepted_moves;
                    total_energy_sum += total_energy();

                    // Print energy every 100 sweeps
                    if (sweep % 100 == 0) {
                        //printf("Density: %.2f, dmax: %.2f, Sweep %d, Total Energy: %f\n",
                               //rho, dmax, sweep, total_energy());
                    }
                }
            }

            double acceptance_ratio = (double)total_accepted_moves / total_moves;
            printf("Density: %.2f, dmax: %.2f, Acceptance Ratio: %.4f, Average Energy: %f\n",
                   rho, dmax, acceptance_ratio, total_energy_sum / num_realizations);
        }
    }

    return 0;
}


