// Banker's Algorithm
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int m = 3; // Number of resources
int n = 5; // Number of processes
int* total_instances; // total instances of Resources
int* avail_instances; // available instances of Resources
int** max_matrix; // maximum resources required by each process
int** allocation; // resources allocated to each process
int** need; // resources needed by each process
int* result; // To store a safe sequence of processes
int* all_results; // To store all safe sequences
int res_counter = 0; // Index for result array
int total_solutions = 0; // To store the total number of safe sequences found
int x = 0; // Index for array storing all solutions


// Function to initialize the environment
void init(char* input){

    FILE *fp;
    int i, j;
    
    // Opening file and reading input
    fp = fopen(input, "r+");
    fscanf(fp, "%d", &m);
    fscanf(fp, "%d", &n);

    // ALLOCATING MEMORY AND INITIALIZING EACH DATA STRUCTURE

    // An array to store the instances of each resource
    total_instances = (int*)malloc(sizeof(int)*m);
    for(i = 0; i < m; ++i) {
        fscanf(fp, "%d", &total_instances[i]);
    }

    // A 2D array that stores the maximum resources needed by each process
    max_matrix = (int**)malloc(sizeof(int*)*n);
    for(i = 0; i < n; ++i) {
        max_matrix[i] = (int*)malloc(sizeof(int)*m);
    }
    for(i = 0; i < n; ++i) {
        for(j = 0; j < m; ++j) {
            fscanf(fp, "%d", &max_matrix[i][j]);
        }
    }

    // A 2D array that stores the given allocation of resources to each process
    allocation = (int**)malloc(sizeof(int*)*n);
    for(i = 0; i < n; ++i) {
        allocation[i] = (int*)malloc(sizeof(int)*m);
    }
    for(i = 0; i < n; ++i) {
        for(j = 0; j < m; ++j) {
            fscanf(fp, "%d", &allocation[i][j]);
        }
    }

    // Array to store sum of instances of each resource allocated so far
    int* allocated_sum = (int*)malloc(sizeof(int)*m);
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            allocated_sum[j] += allocation[i][j];
        }
    }

    // Array to store the currently available instances of each resource
    avail_instances = (int*)malloc(sizeof(int)*m);
    for(i = 0; i < m; ++i) {
        avail_instances[i] = total_instances[i] - allocated_sum[i];
    }

    free(allocated_sum);

    // 2D Array to store the need of resources for each process
    need = (int**)malloc(sizeof(int*)*n);
    for(i = 0; i < n; ++i) {
        need[i] = (int*)malloc(sizeof(int)*m);
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            need[i][j] = max_matrix[i][j] - allocation[i][j];
    }

    // Array to store the result safe sequences temporarily
    result = (int*)malloc(sizeof(int)*n);
    for(i = 0; i < n; ++i) {
        result[i] = -1;
    }

    all_results = (int*)malloc(sizeof(int));
}

// Function to check if a process is ready to be run
bool check(int process, int* procs){

    // If process has already been completed, return false
    if (procs[process] == 1){
        return false;
    }

    // If needed resources are more than available resources, return false
    for (int k = 0; k < m; k++){
        if (need[process][k] > avail_instances[k]){
            return false;
        }
    }

    // Else, process can be chosen, return true
    return true;
}

// Function to solve the banker's algorithm problem
bool solve(int* procs, int count){

    // If all n processes have been run, one safe solution is found
    if (count >= n){
        total_solutions ++;
        all_results = (int*)realloc(all_results, sizeof(int) * total_solutions * n);
        for (int i = 0; i < n; i++){
            // printf("%d ", result[i]);
            all_results[x] = result[i];
            x++;
        }
        // printf("\n");
        return true;
    } 

    bool res = false;

    // Iterating over each process
    for (int i = 0; i < n; i++){

        // Check if the process can be chosen
        if (check(i, procs) == true){

            // Set its state to 1 and increment count
            procs[i] = 1;
            count ++;

            // Update the number of available resources
            for (int j = 0; j < m; j++){
                avail_instances[j] += allocation[i][j];
            } 

            // Update the result array with the process number
            result[res_counter] = i;
            res_counter ++;

            // Recursively calling the function for the next process to be chosen
            res = solve(procs, count) || res;
            res_counter --;
            
            // BACKTRACKING
            // Resetting state, count and available resources
            procs[i] = 0;
            count --;
            for (int j = 0; j < m; j++){
                avail_instances[j] -= allocation[i][j];
            } 
        }
    }
    return res;
}

// Function to free all allocated memory
void free_memory(){
    free(total_instances); 
    free(avail_instances); 
    for (int i = 0; i < n; i ++){
        free(max_matrix[i]);
        free(allocation[i]);
        free(need[i]);
    }
    free(max_matrix); 
    free(allocation); 
    free(need); 
    free(result);
}

// Function to print out the safe sequences in lexicographic order
void print_solution(){

    printf("%d\n", total_solutions);
    for (int i = 0; i < x; i++){
        if (i%n == 0 && i!=0){
            printf("\n");
        }
        printf("%d ", all_results[i]);
    }
    printf("\n");
}

int main()
{
    char* filename = "input.txt"; // Input file name
    init(filename);

    int procs[n]; // To store the status of a process (completed or not)
    for (int i = 0; i < n; i++){
        procs[i] = 0;
    } 
    int count = 0; // Number of processes completed so far

    bool solution_exists = solve(procs, count);

    if (!solution_exists)
        printf("No solution exists.\n");

    print_solution();
    free_memory();

	return (0);

}
