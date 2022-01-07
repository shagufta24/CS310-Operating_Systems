// A structure to store the scheduled tasks.
typedef struct TaskProcessor {
    int process;
    double AST;
    double AFT;
} TaskProcessor;

// A structure that keeps track of the tasks scheduled processor wise.
typedef struct ProcessorSchedule {
    int size;
    TaskProcessor* tasks;
} ProcessorSchedule;

ProcessorSchedule** processorSchedule;

// Comparator for the qsort function to sort the tasks
int comparator(const void* v1, const void* v2) {
    TaskProcessor* p1 = (TaskProcessor*) v1;
    TaskProcessor* p2 = (TaskProcessor*) v2;
    return (p1 -> AST) - (p2 -> AST);
}

// Max of two integers
int max_int(int n, int m) {
    return n > m ? n : m;
}

// Max of two doubles
double max_double(double n, double m) {
    return n > m ? n : m;
}