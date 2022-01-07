#include <stdio.h>
#include <stdlib.h>
#include "heft.c"

void initialize(char* input) {
    FILE *fp;
    fp = fopen(input, "r+");
    fscanf(fp, "%d\n%d", &num_tasks, &num_procs);
    int i, j;

    // A 2D array that stores the computation costs of every task on each processor
    comp_cost = (double**)malloc(sizeof(double*)*num_tasks);
    for(i = 0; i < num_tasks; ++i) {
        comp_cost[i] = (double*)malloc(sizeof(double)*num_procs);
    }
     // getting the input fot the computation costs from the input file
    for(i = 0; i < num_procs; ++i) {
        for(j = 0; j < num_tasks; ++j) {
            fscanf(fp, "%lf", &comp_cost[j][i]);
        }
    }

    // A 2D array to store the communication costs of the switching from one task to another
    // This only applies if the processor is changed between the scheduling of the two tasks
    dag = (double**)malloc(sizeof(double*)*num_tasks);
    for(i = 0; i < num_tasks; ++i) {
        dag[i] = (double*)malloc(sizeof(double)*num_tasks);
    }
    // getting the input fot the communication costs from the input file
    for(i = 0; i < num_tasks; ++i) {
        for(j = 0; j < num_tasks; ++j) {
            fscanf(fp, "%lf", &dag[i][j]);
        }
    }

    // A data structure to store the computed upper ranks of each task (represented by the index)
    u_rank = (double*)malloc(sizeof(double)*num_tasks);
    for(i = 0; i < num_tasks; ++i) {
        u_rank[i] = -1;
    }

    // An array that stores the sorted tasks in decreasing order as per the upper ranks
    // this is equivalent to a topological sort of a DAG
    sorted_tasks = (int*)malloc(sizeof(int)*num_tasks);

    // Initializing the process schduler data structure used to store the scheduling information
    processorSchedule = (ProcessorSchedule**)malloc(sizeof(ProcessorSchedule*)*num_procs);
    for(i = 0; i < num_procs; ++i) {
        processorSchedule[i] = (ProcessorSchedule*)malloc(sizeof(ProcessorSchedule)*num_procs);
        processorSchedule[i]->size = 0;
        processorSchedule[i]->tasks = NULL;
    }

    // initializing the AFT array
    afts = (double*)malloc(sizeof(double)*num_tasks);
    for(i = 0; i < num_tasks; ++i) {
        afts[i] = -1;
    }

    // Initializing the processor array
    proc = (int*)malloc(sizeof(int)*num_tasks);
    for(i = 0; i <num_tasks; ++i) {
        proc[i] = -1;
    }

    // to store est and eft of all tasks for every processor
    all_et = (et*)malloc(sizeof(et)*num_tasks); 
    for(i = 0; i <num_tasks; ++i) {
        all_et[i].task = -1;
        all_et[i].ests = (double*)malloc(sizeof(double)*num_procs);
        all_et[i].efts = (double*)malloc(sizeof(double)*num_procs);
        for (int j = 0; j<num_procs; ++j) {
            all_et[i].ests[j] = -1;
            all_et[i].efts[j] = -1;
        }
    }
}

void display_output() {
    int i;
    printf("No. of tasks: %d\n", num_tasks);
    printf("No. of processors: %d\n", num_procs);
    printf("\n");
    printf("The upward rank values:\n");
    for(i = 0; i < num_tasks; ++i) {
        printf("Task %d: %.6lf\n", i+1, u_rank[i]);
    }
    printf("\n");
    
    printf("The order of tasks to be scheduled: \n");
    for(i = 0; i < num_tasks; ++i) {
        printf("%d ", sorted_tasks[i] + 1);
    }
    printf("\n\n");

    printf("EST and EFT on different processors\n");
    for(int i = 0; i < num_tasks; i++)
    {
        printf("Task: %d\n", i + 1);

        int x = sorted_tasks[i];
        for(int j = 0; j < num_procs; j++)
        {
            printf("processor %d||est: %lf eft: %lf ||\n", j + 1, all_et[x].ests[j], all_et[x].efts[j]);
            // printf("processor %d||est: eft:  ||\n", j);
        }
        
        printf("\n");
    }
    
    printf("Final Schedule:\n");

    final_vals *fv = (final_vals*)malloc(sizeof(final_vals)*num_tasks);

    int cnt = 0;


    for(i = 0; i < num_procs; ++i) {
        TaskProcessor* tasks = processorSchedule[i]->tasks;
        int j;
        for(j = 0; j < processorSchedule[i]->size; ++j) {
            int index = tasks[j].process;
            fv[index].proc = i;
            fv[index].st = tasks[j].AST;
            fv[index].et = tasks[j].AFT;
            // printf("Task %d is executed on processor %d from time %g to %g\n",
            //     tasks[j].process+1, 
            //     i+1,
            //     tasks[j].AST, 
            //     tasks[j].AFT);
        }
    }

    for(int i = 0; i < num_tasks; i++)
    {
        // for(int j = 0; j < num_tasks; j++)
        // {
        //     if(fv[j].task == i)
        //     {
                printf("Task %d is executed on processor %d from time %g to %g\n",
                i + 1, 
                fv[i].proc + 1,
                fv[i].st, 
                fv[i].et);

        //         break;
        //     }
        // }
    }
    
    double makespan = DBL_MIN;
    for(i = 0; i < num_tasks; ++i) {
        if(makespan < afts[i]) {
            makespan = afts[i];
        }
    }
    printf("\nHence, the makespan length from the schedule: %g\n", makespan);
}

void free_space() {
    int i;

    for(i = 0; i < num_tasks; ++i) {
        free(comp_cost[i]);
    }

    for(i = 0; i < num_tasks; ++i) {
        free(dag[i]);
    }

    for(i = 0; i < num_procs; ++i) {
        free(processorSchedule[i]->tasks);
        free(processorSchedule[i]);
    }

    free(comp_cost);
    free(dag);
    free(u_rank);
    free(processorSchedule);
    free(afts);
    free(proc);
    free(sorted_tasks);
}

int main(int argc, char** argv) {
    if(argc != 2) {
        puts("Please provide just one file in the specified format.");
        return -1;
    }
    initialize(argv[1]);
    find_ranks();
    heft();
    display_output();
    free_space();
    return 0;
}