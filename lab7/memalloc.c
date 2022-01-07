#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int memory_size = 1024;
int warmup_steps = 1000;
int experiment_steps = 1000;

int block_count = 0; //Total no. of blocks created
int nchecks = 0; //No. of holes checked to find the right one
int consparts = 0; //Max partitions created consequtively (without evictions)
int evics = 0; //Evictions needed for one partition
int maxevics = 0; // Max evictions needed for one partition

// Memblock can be either a hole or a partition in memory
typedef struct memblock{
    int size;
    bool hole;
    struct memblock* next;
} memblock;

memblock* head = NULL; // Head of linked list

void initList(){
    head = (memblock*)malloc(sizeof(memblock));
    head->size = memory_size;
    head->hole = true;
    head->next = NULL;
    block_count += 1;
    // printf("Initialized List\n");
}

void printBlocks() {
    printf("Printing all blocks:\n");
    memblock *temp = head;

    for(int i=0; i < block_count; i++) {
        if (temp->hole == true)
            printf("    Hole at loc %d of size %d\n", i, temp->size);
        else
            printf("    Partition at loc %d of size %d\n", i, temp->size);
        if(temp->next != NULL) {
            temp = temp->next;
        }
    }
} 

// Returns node at given index
memblock* getNode(int loc){
    memblock* current = head;
    int count = 0;
    while (current != NULL) {
        if (count == loc)
            return current;
        count++;
        current = current->next;
    }
    return NULL;
}

memblock* createPartition(int size, int location){

    // Creating a new partition
    memblock* new_block = malloc(sizeof(memblock));
    new_block->size = size;
    new_block->hole = false;
    new_block->next = NULL;

    // Inserting it at given location
    memblock *temp = head;
    for(int i=0; i < location-1; i++) {
        if(temp->next != NULL) {
            temp = temp->next;
        }
    }

    if(location == 0){
        new_block->next = temp;
        // printf("Hole: %d, size: %d\n", new_block->next->hole, new_block->next->size);
        head = new_block;
    }
    else{
        new_block->next = temp->next;
        temp->next = new_block;
    }
    
    block_count += 1;

    // Newly created block
    printf("Created new partition of size %d at location %d\n", size, location);
    consparts++;
    return new_block;
}

void removeBlock(int location){

    // Deleting node from given location
    memblock *temp = head;
    for(int i=0; i < location-1; i++) {
        if(temp->next != NULL) {
            temp = temp->next;
        }
    }
    if (location == 0){
        head = head->next;
    }
    else{
        temp->next = temp->next->next;
    }
    block_count -= 1;
}

// To insert a partition in a hole at given location
bool insertPartition(int size, int location){
    memblock* block = getNode(location);

    // Hole is too small or is a partition
    if (block->hole == false){
        printf("It is a partition, not a hole.\n");
        return false;
    }
    else if (block->size < size){
        printf("Hole is too small\n");
        return false;
    }
    // Fits perfectly
    else if (block->size == size){
        printf("Inserted job at location %d\n", location);
        block->hole = false;
        return true;
    }
    // Hole is too big
    else{ 
        block->size -= size; // Reduce hole size
        // Create a new partition at given location
        memblock* new_partition = createPartition(size, location); 
        printf("Inserted job at location %d\n", location);
        // printBlocks();
        return true;
    }
}

// To remove a partition at given location and make it a hole
int removePartition(int location){
    
    memblock* block = getNode(location);
    block->hole = true;

    // If entire memory is a hole
    if (block->size == memory_size)
        return location;

    //Consolidating holes if any on either side
    if (location > 0){ //left side
        memblock* prev_block = getNode(location-1);
        if (prev_block->hole == true){
            block->size += prev_block->size;
            removeBlock(location - 1); // Removing hole on the left side
            // printf("Left consolidation done\n");
            location -= 1; // since it is moved to the left
        }
    }
    // printBlocks();
    if (location < block_count && block->next != NULL){ //right side
        memblock* next_block = getNode(location+1);
        if (next_block->hole == true){
            block->size += next_block->size;
            removeBlock(location + 1);
            // printf("Right consolidation done\n");
        }
    }
    // printBlocks();
    return location; // Location of consolidated hole
}

// Randomly chooses a partition to evixt
int chooseEviction() {
        int victim; 
        int loc = 0;
        int npart = 0;
        int count = 0;

        // count partitions
        memblock* temp = head;
        for (int i = 0; i < block_count; i++) {
            if (temp->hole == false)
                npart++;
            if (temp->next != NULL)
                temp = temp->next;
        }

        // make sure memory is not empty
        if (npart == 0) {
            printf("Error: attempted to free partition in empty memory.\n");
            return -1;
        }

        // choose a victim to evict
        // Formula: (rand() % (upper - lower + 1)) + lower
        // Choosing between 0 to (npart-1)
        victim = (rand() % (npart-1 - 0 + 1)) + 0; 

        // find the block that the victim is in
        temp = head;
        for (int i = 0; i < block_count; i++) {
            if (temp->hole == false) {
                if (count == victim) {
                    loc = i;
                }
                count++;
            }
            if (temp->next != NULL)
                temp = temp->next;
        }

        // printf("Npart: %d, Victim: %d, Loc of victim: %d\n", npart, victim, loc);
        return loc;
    } 

// Returns location of the largest hole
int worstFit(int size){
    int location = -1;
    int max_size = 0;

    // search for biggest hole
    memblock* temp = head;
    for (int i = 0; i < block_count; i++) {
        if (temp->hole == true) {
            nchecks++;
            if (temp->size > max_size) {
                max_size = temp->size;
                location = i;
            }
        }
        if (temp->next != NULL) 
            temp = temp->next;
    }

    // If largest hole is not big enough, it evicts processes
    evics = 0;
    while (max_size < size) {
            location = chooseEviction();
            //printBlocks();
            printf("Evicting partition of size %d from location %d\n", getNode(location)->size, location);
            location = removePartition(location);
            consparts = 0;
            evics++;
            // printf("Removed partition\n");
            max_size = getNode(location)->size;
        }
    // printBlocks();
    if (evics > maxevics)
        maxevics = evics;
    return location;
}

// Returns location of the first hole that is large enough
int firstFit(int size){
    int location = -1;
    int cur_size = 0;

    // search for first hole big enough
    memblock* temp = head;
    for (int i = 0; i < block_count; i++) {
        if (temp->hole == true) {
            nchecks++;
            if (temp->size >= size) {
                cur_size = temp->size;
                location = i;
                break;
            }
        }
        if (temp->next != NULL) 
            temp = temp->next;
    }
    // If no hole of required size is found
    evics = 0;
    while (cur_size < size) {
            location = chooseEviction();
            //printBlocks();
            printf("Evicting partition of size %d from location %d\n", getNode(location)->size, location);
            location = removePartition(location);
            consparts = 0;
            evics++;
            // printf("Removed partition\n");
            cur_size = getNode(location)->size;
        }
    // printBlocks();
    if (evics > maxevics)
        maxevics = evics;
    return location;
}

int bestFit(int size){
    int location = -1;
    int best_size = memory_size+1;
    int found = 0;

    // search for the smallest hole that is big enough
    memblock* temp = head;
    for (int i = 0; i < block_count; i++) {
        if (temp->hole == true) {
            nchecks++;
            if (temp->size >= size && temp->size < best_size) {
                found = 1;
                best_size = temp->size;
                location = i;
            }
        }
        if (temp->next != NULL) 
            temp = temp->next;
    }
    // If no hole of required size is found
    evics = 0;
    while (found == 0) {
            location = chooseEviction();
            //printBlocks();
            printf("Evicting partition of size %d from location %d\n", getNode(location)->size, location);
            location = removePartition(location);
            evics++;
            consparts = 0;
            // printf("Removed partition\n");
            best_size = getNode(location)->size;
            if (best_size >= size)
                found = 1;
        }
    // printBlocks();
    if (evics > maxevics)
        maxevics = evics;
    return location;
}

int generateJob(int job_policy){
    int random;
    int upper;
    int lower;
    // srand(time(0));

    if (job_policy == 1){       // Any job
            upper = 1024;
            lower = 1;
    }
    else if (job_policy == 2){ // Small job
            upper = 100;
            lower = 1;
    }
    else {                     // Large job
            upper = 1000;
            lower = 500;
    }
    return (rand() % (upper - lower + 1)) + lower;
}

// Global stats variables
int max_job_count = 0; 
int min_job_count = 1025; 
double max_frag = 0;
double min_frag = 1025;
int total_job_count = 0;
int total_hole_count = 0;
int max_hole_count = 0;
int min_hole_count = 1025;
double total_frag = 0; 

void stats(int time_step, FILE *fptr){

    int total_hole_size = 0;
    int hole_count = 0; 
    double avg_hole_size = 0; 
    double avg_holes_examined = 0;
    double avg_hole_count = 0;

    int total_job_size = 0; 
    int job_count = 0; 
    double avg_job_size = 0;
    double avg_job_count = 0;

    double frag = 0;
    double avg_frag = 0;

    memblock* temp = head;
    for (int i = 0; i < block_count; i++) {
        if (temp->hole == true) {
            hole_count ++;
            total_hole_size += temp->size;
        }
        else {
            job_count ++;
            total_job_size += temp->size;
        }
        if (temp->next != NULL) 
            temp = temp->next;
    }
    total_job_count += job_count;
    total_hole_count += hole_count;

    // Average fragmentation (for this time step only)
    frag = (double)total_hole_size/memory_size;
    total_frag += frag;
    avg_frag = (double)total_frag / (time_step+1);
    // printf("Average fragmentation: %.4lf\n", avg_frag);
    fprintf(fptr, "%.4lf,", avg_frag);
    // Average hole size (for this time step only)
    avg_hole_size = (double)total_hole_size/hole_count;
    // printf("Average Hole Size: %.4lf\n", avg_hole_size);
    fprintf(fptr, "%9.4lf,", avg_hole_size);
    // Average no. of holes examined for partition
    avg_holes_examined = (double)nchecks/(time_step+1);
    // printf("Average Holes examined for partition request: %.4lf\n", avg_holes_examined);
    fprintf(fptr, "%10.4lf,", avg_holes_examined);
    // Highest fragmentation
    if (frag >= max_frag)
        max_frag = frag;
    // printf("Highest fragmentation: %.4lf\n",max_frag);
    fprintf(fptr, "%.4lf,",max_frag);
    // Lowest fragmentation
    if (frag < min_frag)
        min_frag = frag;
    // printf("Lowest fragmentation: %.4lf\n", min_frag);
    fprintf(fptr, "%.4lf,", min_frag);
    // Average no of jobs
    avg_job_count = (double)total_job_count/(time_step+1);
    // printf("Average job count: %.4lf\n", avg_job_count);
    fprintf(fptr, "%.4lf,", avg_job_count);
    // Highest Job Count
    if (job_count >= max_job_count)
        max_job_count = job_count;
    // printf("Highest job count: %d\n", max_job_count);
    fprintf(fptr, "%d,", max_job_count);
    // Lowest Job count
    if (job_count < min_job_count)
        min_job_count = job_count;
    // printf("Lowest job count: %d\n", min_job_count);
    fprintf(fptr, "%5d,", min_job_count);
    // Average job size (for this time step only)
    avg_job_size = (double)total_job_size/job_count;
    // printf("Total jobs size: %d\n", total_job_size);
    // printf("Job count: %d\n", job_count);
    // printf("Average job size: %.4lf\n", avg_job_size);
    fprintf(fptr, "%10.4lf,", avg_job_size);
    // Average no. of holes
    avg_hole_count = (double)total_hole_count/(time_step+1);
    // printf("Average hole count: %.4lf\n", avg_hole_count);
    fprintf(fptr, "%8.4lf,", avg_hole_count);
    // Highest no. of holes
    if (hole_count >= max_hole_count)
        max_hole_count = job_count;
    // printf("Highest hole count: %d\n", max_hole_count);
    fprintf(fptr, "%d,", max_hole_count);
    // Lowest no. of holes
    if (hole_count < min_hole_count)
        min_hole_count = hole_count;
    // printf("Lowest hole count: %d\n", min_hole_count);
    fprintf(fptr, "%d,", min_hole_count);
    // Largest no. of partitions created without evictions
    // printf("Highest number of partitions created without evictions: %d\n", consparts);
    fprintf(fptr, "%d,", consparts);
    // Largest number of evictions to satisfy one partition request
    // printf("Highest number of evictions to satisfy one partition request: %d\n", maxevics);
    fprintf(fptr, "%d\n", maxevics);
}

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Wrong input parameters.\n");
        return -1;
    }
    
    int job_profile = atoi(argv[1]);
    int policy = atoi(argv[2]);
    
    if (job_profile < 1 || job_profile > 3 || policy < 1 || policy > 3){
        printf("Wrong input parameters.\n");
        return -1;
    }

    FILE *fptr;
    fptr = fopen("output.csv", "w");
    if(fptr == NULL){
        printf("ERROR OPENING FILE.\n");   
        return -1;            
    }

    fprintf(fptr, "Average fragmentation,");
    fprintf(fptr, "Average hole size,");
    fprintf(fptr, "Average holes examined,");
    fprintf(fptr, "Highest fragmentation,");
    fprintf(fptr, "Lowest fragmentation,");
    fprintf(fptr, "Average job count,");
    fprintf(fptr, "Highest job count,");
    fprintf(fptr, "Lowest job count,");
    fprintf(fptr, "Average job size,");
    fprintf(fptr, "Average hole count,");
    fprintf(fptr, "Highest hole count,");
    fprintf(fptr, "Lowest hole count,");
    fprintf(fptr, "Highest partitions created without evictions,");
    fprintf(fptr, "Highest evictions for partition request\n");

    initList();
    // printBlocks();

    int time;
    int hole_loc;
    int job_size;

    //Warmup Steps
    printf("WARMUP STEPS:\n");
    for (time = 0; time < warmup_steps; time++) {
            job_size = generateJob(job_profile);
            printf("\n%d. Generated job of size %d\n", time+1, job_size);
            if (policy == 1)
                hole_loc = bestFit(job_size);
            else if (policy == 2)
                hole_loc = worstFit(job_size);
            else
                hole_loc = firstFit(job_size);
            printf("Hole found at location %d\n", hole_loc);
            insertPartition(job_size,hole_loc);
        }

    // Experiment Steps
    printf("EXPERIMENT STEPS:\n");
    for (time = 0; time < experiment_steps; time++) {
            job_size = generateJob(job_profile);
            printf("\n%d. Generated job of size %d\n", time+1+warmup_steps, job_size);
            if (policy == 1)
                hole_loc = bestFit(job_size);
            else if (policy == 2)
                hole_loc = worstFit(job_size);
            else
                hole_loc = firstFit(job_size);
            printf("Hole found at location %d\n", hole_loc);
            insertPartition(job_size, hole_loc);

            // Calculating statistics
            printf("\n");
            stats(time, fptr);
        }

    // Freeing allocated memory
    memblock* temp;
    while (head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }

    printf("\nEnd of simulation.\n");
    fclose(fptr);
    return 0;
}
