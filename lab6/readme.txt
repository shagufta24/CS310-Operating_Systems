Operating Systems Lab 6
Implementation of the Banker's Algorithm in C

Name: Shagufta Anjum
Rollno: 19XJ1A0568

To compile: gcc 19XJ1A0568.c -o <insert-name>
To run: ./<insert-name>

Logic Explanation:
I used backtracking to find all possible safe sequences.

main() function:
• The procs array maintains the state of each process. 1 means completed and 0 means not chosen yet.
• The count variable keeps a count of the number of processes run so far.
• The init() function is called to take input and set up variables.
• The solve() function is run, followed by printing the solution and freeing the dynamically allocated memory.

Init() function:
• Input is taken from the file and stored in appropriate dynamically allocated data structures.
• The allocated_sum array stores the sum of allocated resources across all processes.
• The avail_instances array stores the current available resources (total - allocated_sum).
• The need matrix stores the need of each process for each resource (max - allocated).
• The result array stores each safe sequence temporarily. The all_results array stores all the safe sequences in a consequtive fashion.

check() function:
• Checks if a process can be chosen or not.
• If process has already been completed, it returns false.
• If needed resources > available resources, it returns false.
• Otherwise, process can be picked, so it returns true.

solve() function:
• First, if the count >= no. of processes, it means a safe sequence has been found. We append it to the all_results array and return true.
• We iterate over each process and run the check() function.
• If check() returns true, we set its state to 1 and increment count.
• Then we add the allocated resources for this process to the available resources.
• We also update the result array with the process number at the appropriate position.
• Then, we recursively call the solve() function with the new process state array.
• Next, backtracking is implemented. We set the process state to 0, decrement count and reset available resources.
• At any stage, if there is no process that can be chosen, it returns false and we backtrack to the previous step to try other possibilties.
• Due to the nature of the backtracking algorithm, the safe sequences found are already in lexicographic order.

INPUT:
Input is taken from file "input.txt"

Format of "input.txt" file:
3  // The first line indicates the number of resources (m)
5 // The second line indicates the number of processes (n)
10 5 7 //The third line indicates the total number of instances of each resource  (1*m). Here, there are 10, 5, and 7 instances of R1, R2, and R3 respectively
7 5 3 //The next n lines indicate the maximum requirements of each process (max matrix: n*m)
3 2 2
9 0 2
2 2 2
4 3 3
0 1 0 //The next n lines indicate the allocated resources for each process (allocation matrix: n*m)
2 0 0
3 0 2
2 1 1
0 0 2

OUTPUT:
Output is displayed on the command line.
The total number of safe sequences is printed followed by each safe sequence (in lexicographic order).

Format of output for above test case:
16           //The number of safe sequences 
1 3 0 2 4    // Safe sequences are printed in lexicographical (dictionary) order
1 3 0 4 2
1 3 2 0 4
1 3 2 4 0
1 3 4 0 2
1 3 4 2 0
1 4 3 0 2
1 4 3 2 0
3 1 0 2 4
3 1 0 4 2
3 1 2 0 4
3 1 2 4 0
3 1 4 0 2
3 1 4 2 0 
3 4 1 0 2
3 4 1 2 0

