#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "helpers.c"

int num_tasks; // number of tasks to be schduled
int num_procs; // bounded number of heteroenous processors
double** dag; // adjacency matrix form for the input DAG
double** comp_cost; // the cost of processes on each processor table
double* u_rank; // the calculated upper ranks of each process
int* sorted_tasks; // the sorted indexes of each processor based on their upper rank
double* afts; // to store the actual finish times of the task (represented by the index)
int* proc; // the processor that the task is scheduled on (represented by the index)

// to store the Estimated Times (ET): EST and EFT of a task on each processor
typedef struct et{
    int task; //index
    double* ests;
    double* efts;
}et;

et* all_et; // to store est and eft for each task for every processor

typedef struct final_vals
{
    int task;
    int proc;
    double st;
    double et;
} final_vals;

// A function to compute the average computation cost across all the 
// processors for the given task
double avg_comp_cost(int task) {
    int j;
    double avg = 0.0;
    for(j = 0; j < num_procs; ++j) {
        avg += comp_cost[task][j];
    }
    return avg/num_procs;;
}

// A function to calculate the upper rank of the provided task
double upward_rank(int task) {
    if(u_rank[task] != -1) {
        return u_rank[task];
    }
    double max = 0.0;
    int i;
    for(i = 0; i < num_tasks; ++i) {
        if(dag[task][i] != 0) {
            double cost;
            if(u_rank[i] != -1) {
                cost = dag[task][i] + u_rank[i];
            } else {
                cost = dag[task][i] + upward_rank(i);
            }
            if(cost > max) {
                max = cost;
            }
        }
    }
    u_rank[task] = avg_comp_cost(task) + max;
    return u_rank[task];
}

// Structure used to sort indexes
typedef struct Sort {
    int index;
    double val;
} Sort;

// Sorts in decreasing order based on the upper ranks of the tasks.
// this is equivalent to a topological sort of a DAG
void sort_indices() {
    Sort* arr = (Sort*)malloc(sizeof(Sort)*num_tasks);
    int i, j;
    for(i = 0; i < num_tasks; ++i) {
        arr[i].index = i;
        arr[i].val = u_rank[i];
    }
    for(i = 0; i < num_tasks; ++i) {
        for(j = i + 1; j < num_tasks; ++j) {
            if(arr[j].val > arr[i].val) {
                int tempIndex = arr[i].index;
                double tempVal = arr[i].val;
                arr[i].val = arr[j].val;
                arr[i].index = arr[j].index;
                arr[j].val = tempVal;
                arr[j].index = tempIndex;
            }
        }
    }
    for(i = 0; i < num_tasks; ++i) {
        sorted_tasks[i] = arr[i].index;
    }
    free(arr);
}

// function that calculates the upper ranks of the input task,
// sorts them in decreasing order and displays it to the user.
void find_ranks() {
    int i;
    for(i = 0; i < num_tasks; ++i) {
        upward_rank(i);
    }
    sort_indices();
}

// A function to determine whether the provided task
// is an entry task or not
bool check_entry_task(int task) {
    int i;
    for(i = 0; i < num_tasks; ++i) {
        if(dag[i][task] != 0) {
            return false;
        }
    }
    return true;
}

// A function to find an avaialble slot on a processors which has the tasks schedules as provided.
// The number of tasks already scheduled is equal to the size provided.
// This will be the first slot available
void find_slot(TaskProcessor* tasks, int size, double comp_cost, double* earliest_time) {
    if(size == 0) {
        *earliest_time = 0;
        return;
    }
    qsort((void*) tasks, size, sizeof(TaskProcessor), comparator);
    int curr = 0;
    int i;
    for(i = 1; i < size; ++i) {
        if(tasks[i].AST - tasks[curr].AFT >= comp_cost) {
            *earliest_time = tasks[curr].AFT;
            return;
        } else {
            curr++;
            tasks[curr] = tasks[i];
        }
    }
    *earliest_time = tasks[curr].AFT;
    //*earliest_time = tasks[size-1].AFT;
    return;
}

// Function to calcuate the EST
void compute_est(int task, int processor, double* EST) {
    double earliest_time;
    if(processorSchedule[processor]->size == 0) {
        earliest_time = 0.0;
    } else {
        int i, add = 0;
        for(i = 0; i < num_tasks; ++i) {
            if(dag[i][task] != 0 && afts[i] != -1 && proc[i] != processor) {
                add = dag[i][task];
            }
        }
        find_slot(processorSchedule[processor]->tasks, 
              processorSchedule[processor]->size, 
              comp_cost[task][processor] + add, &earliest_time);
    }
    //printf("Earliest Available %g\n", earliest_time);
    double max = DBL_MIN;
    int i;
    for(i = 0; i < num_tasks; ++i) {
        if(dag[i][task] != 0) {
            if(proc[i] == processor) {
                if(max < afts[i]) {
                    max = afts[i];
                }  
            } else {
                if(max < afts[i] + dag[i][task]) {
                    max = afts[i] + dag[i][task];
                }  
            }
            
        }
    }
    *EST = max_double(earliest_time, max);
    //printf("EST on %d is %g\n\n", processor+1, *EST);
    return;
}

// HEFT scheduler function
void heft() {
    
    // all_et = (et*)malloc(sizeof(et)*num_tasks);

    
    for(int i = 0; i < num_tasks; ++i) {
        int task = sorted_tasks[i];

        printf("Task: %d\n", task);

        all_et[i].task = task;



        //printf("Schdeuling %d\n", task+1);
        if(check_entry_task(task)) {
            double min = DBL_MAX;
            int processor;

            
            for(int j = 0; j < num_procs; ++j) {
                all_et[i].ests[j] = 0;
                all_et[i].efts[j] = comp_cost[task][j];

                if(comp_cost[task][j] < min) {
                    min = comp_cost[task][j];
                    processor = j;
                }
            }
            processorSchedule[processor]->tasks = (TaskProcessor*)malloc(sizeof(TaskProcessor));
            processorSchedule[processor]->tasks[processorSchedule[processor]->size].AST = 0.0;
            processorSchedule[processor]->tasks[processorSchedule[processor]->size].AFT = min;
            processorSchedule[processor]->tasks[processorSchedule[processor]->size].process = task;
            processorSchedule[processor]->size++;
            afts[task] = min;
            proc[task] = processor;
            printf("Scheduled on %d with AST %g and AFT %g\n", processor+1, 0.0, min);
        } else {
            double minEFT = DBL_MAX;
            double selectedEST = -1;
            int processor;
            int j;
            for(j = 0; j < num_procs; ++j) {
                double EST;
                compute_est(task, j, &EST);

                all_et[i].ests[j] = EST;
                all_et[i].efts[j] = EST + comp_cost[task][j];

                if(EST + comp_cost[task][j] < minEFT) {
                    minEFT = EST + comp_cost[task][j];
                    selectedEST = EST;
                    processor = j;
                }
            }
            processorSchedule[processor]->tasks 
                            = (TaskProcessor*)realloc(processorSchedule[processor]->tasks, 
                                                      sizeof(TaskProcessor)*((processorSchedule[processor]->size) + 1));
            processorSchedule[processor]->tasks[processorSchedule[processor]->size].AST = selectedEST;
            processorSchedule[processor]->tasks[processorSchedule[processor]->size].AFT = minEFT;
            processorSchedule[processor]->tasks[processorSchedule[processor]->size].process = task;
            processorSchedule[processor]->size++;
            afts[task] = minEFT;
            proc[task] = processor;
            printf("Scheduled on %d with AST %g and AFT %g\n", processor+1, selectedEST, minEFT);
        }
    }
}