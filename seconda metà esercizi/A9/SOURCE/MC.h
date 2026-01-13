void copycoordinates(long int idp)
{
     parts[idp].ox = parts[idp].x;
     parts[idp].oy = parts[idp].y;
     parts[idp].oz = parts[idp].z;

}

void rollback(long int idp)
{
     parts[idp].x = parts[idp].ox;
     parts[idp].y = parts[idp].oy;
     parts[idp].z = parts[idp].oz;

}


void translation()
{
    for (long int idp = 0; idp < mySys.num_particles; idp++) {
            // Genera uno spostamento casuale
            double dx = ran3(&mySys.seed) * 2.0 - 1.0; // Genera un valore tra -1 e 1
            double dy = ran3(&mySys.seed) * 2.0 - 1.0;
            double dz = ran3(&mySys.seed) * 2.0 - 1.0;

            // Applica lo spostamento alla posizione
            parts[idp].x += dx;
            parts[idp].y += dy;
            parts[idp].z += dz;

            // Assicurati che le particelle rimangano all'interno dei limiti della scatola, se necessario
            // Periodic boundary conditions can be applied here
        }
}



void do_MC_sweep()
{
    for (long int idp = 0; idp < mySys.num_particles; idp++) {
            copycoordinates(idp);  // Salva la configurazione corrente
            
            translation();  // Genera una nuova configurazione

            // Calcola l'energia della nuova configurazione
            double new_energy = compute_energy();

            // Decidi se accettare o meno la nuova configurazione
            double delta_energy = new_energy - previous_energy;
            if (delta_energy < 0 || ran3(&mySys.seed) < exp(-delta_energy / mySys.temperature)) {
                // Accetta la nuova configurazione
                previous_energy = new_energy;
            } else {
                // Ripristina la configurazione precedente
                rollback(idp);
            }
        }
}

void do_MC(){

    char dumpname[100];
    char restartname[100];

    FILE* f = fopen("energy.dat", "a");
    FILE* g = fopen("acceptance.dat", "a"); 

    sprintf(restartname,"restartpoint.dat");
    
    for(mySys.step=0; mySys.step < mySys.NSteps; mySys.step++){
         
        do_MC_sweep();
        //if(mySys.step % 1000 == 0)  WriteConf(restartname);
     
        if(mySys.step % mySys.NPrint == 0){ 
            fprintf(f, "%d %f\n", mySys.step, current_energy); // Sostituisci con l'energia corrente
            fprintf(g, "%d %f\n", mySys.step, acceptance_ratio); // Sostituisci con il rapporto di accettazione
        }
    }
   
    fclose(f); fclose(g);
   
}

