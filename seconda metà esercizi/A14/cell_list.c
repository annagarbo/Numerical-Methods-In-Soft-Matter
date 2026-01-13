#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_DUMBBELLS 50
#define NUM_STEPS 1000
#define TIME_STEP 0.01
#define K_SPRING 1.0
#define LAMBDA_SPRING 0.5
#define NUM_FORCES 3

typedef struct {
    double x, y;
} Vector;

void initialize_system(Vector* probe_position, Vector* probe_velocity, Vector* dumbbell_positions, Vector* dumbbell_velocities) {
    *probe_position = (Vector){0.0, 0.0};
    *probe_velocity = (Vector){0.0, 0.0};
    
    for (int i = 0; i < NUM_DUMBBELLS; i++) {
        dumbbell_positions[i] = (Vector){(double)rand() / RAND_MAX, (double)rand() / RAND_MAX};
        dumbbell_velocities[i] = (Vector){(double)rand() / RAND_MAX - 0.5, (double)rand() / RAND_MAX - 0.5};
    }
}

void update_system(Vector* probe_position, Vector* probe_velocity, Vector* dumbbell_positions, Vector* dumbbell_velocities, double f) {
    // Update probe position and velocity
    probe_position->x += probe_velocity->x * TIME_STEP;
    probe_position->y += probe_velocity->y * TIME_STEP;
    
    // Calculate total force on the probe
    Vector total_force_on_probe = {0.0, 0.0};
    
    for (int i = 0; i < NUM_DUMBBELLS; i++) {
        Vector pos1 = dumbbell_positions[i];
        Vector pos2 = {pos1.x + 1, pos1.y}; // Assuming alignment in x-direction
        
        // Calculate force from pos1
        Vector displacement1 = {probe_position->x - pos1.x, probe_position->y - pos1.y};
        double distance1 = sqrt(displacement1.x * displacement1.x + displacement1.y * displacement1.y);
        
        if (distance1 > 0) {
            Vector unit_vector1 = {displacement1.x / distance1, displacement1.y / distance1};
            double spring_force_magnitude1 = K_SPRING * (distance1 - LAMBDA_SPRING);
            Vector spring_force1 = {spring_force_magnitude1 * unit_vector1.x, spring_force_magnitude1 * unit_vector1.y};
            Vector force1 = {-spring_force1.x - f * unit_vector1.x, -spring_force1.y - f * unit_vector1.y};
            total_force_on_probe.x += force1.x;
            total_force_on_probe.y += force1.y;
        }
        
        // Calculate force from pos2
        Vector displacement2 = {probe_position->x - pos2.x, probe_position->y - pos2.y};
        double distance2 = sqrt(displacement2.x * displacement2.x + displacement2.y * displacement2.y);
        
        if (distance2 > 0) {
            Vector unit_vector2 = {displacement2.x / distance2, displacement2.y / distance2};
            double spring_force_magnitude2 = K_SPRING * (distance2 - LAMBDA_SPRING);
            Vector spring_force2 = {spring_force_magnitude2 * unit_vector2.x, spring_force_magnitude2 * unit_vector2.y};
            Vector force2 = {-spring_force2.x - f * unit_vector2.x, -spring_force2.y - f * unit_vector2.y};
            total_force_on_probe.x += force2.x;
            total_force_on_probe.y += force2.y;
        }
    }
    
    // Update probe velocity
    probe_velocity->x += total_force_on_probe.x * TIME_STEP;
    probe_velocity->y += total_force_on_probe.y * TIME_STEP;
    
    // Update dumbbell positions and velocities
    for (int i = 0; i < NUM_DUMBBELLS; i++) {
        Vector pos1 = dumbbell_positions[i];
        Vector pos2 = {pos1.x + 1, pos1.y}; // Assuming alignment in x-direction
        
        Vector displacement = {pos2.x - pos1.x, pos2.y - pos1.y};
        double distance = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
        Vector unit_vector = {displacement.x / distance, displacement.y / distance};
        
        double spring_force_magnitude = K_SPRING * (distance - LAMBDA_SPRING);
        Vector spring_force = {spring_force_magnitude * unit_vector.x, spring_force_magnitude * unit_vector.y};
        Vector propulsive_force = {f * unit_vector.x, f * unit_vector.y};
        
        Vector force1 = {-spring_force.x - propulsive_force.x, -spring_force.y - propulsive_force.y};
        Vector force2 = {spring_force.x + propulsive_force.x, spring_force.y + propulsive_force.y};
        
        dumbbell_velocities[i].x += force1.x * TIME_STEP;
        dumbbell_velocities[i].y += force1.y * TIME_STEP;
        dumbbell_velocities[i].x += force2.x * TIME_STEP;
        dumbbell_velocities[i].y += force2.y * TIME_STEP;
        
        dumbbell_positions[i].x += dumbbell_velocities[i].x * TIME_STEP;
        dumbbell_positions[i].y += dumbbell_velocities[i].y * TIME_STEP;
        
        pos2 = (Vector){dumbbell_positions[i].x + 1, dumbbell_positions[i].y};
        dumbbell_positions[i].x = (pos1.x + pos2.x) / 2;
        dumbbell_positions[i].y = (pos1.y + pos2.y) / 2;
    }
}

double run_simulation(double f) {
    Vector probe_position, probe_velocity;
    Vector dumbbell_positions[NUM_DUMBBELLS], dumbbell_velocities[NUM_DUMBBELLS];
    
    initialize_system(&probe_position, &probe_velocity, dumbbell_positions, dumbbell_velocities);
    
    Vector probe_initial_position = probe_position;
    double msd = 0.0;
    
    for (int step = 0; step < NUM_STEPS; step++) {
        update_system(&probe_position, &probe_velocity, dumbbell_positions, dumbbell_velocities, f);
        double dx = probe_position.x - probe_initial_position.x;
        double dy = probe_position.y - probe_initial_position.y;
        msd += dx * dx + dy * dy;
    }
    
    msd /= NUM_STEPS;
    return msd;
}

int main() {
    double forces[16] = {0.0, 1.0,2.0,3.0,4.0, 5.0,6.0,7.0,8.0,9.0,10,100,200,300,500,1000};
    FILE *file = fopen("msd_results.txt", "w");
    
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    
    fprintf(file, "Force\tMean Squared Displacement\n");
    
    for (int i = 0; i < 16; i++) {
        double f = forces[i];
        double msd = 0.0;
        int num_realizations = 10;  // Number of realizations
        
        for (int j = 0; j < num_realizations; j++) {
            msd += run_simulation(f);
        }
        
        msd /= num_realizations;
        fprintf(file, "%f\t%f\n", f, msd);
        printf("Force: %f, MSD: %f\n", f, msd);
    }
    
    fclose(file);
    return EXIT_SUCCESS;
}

