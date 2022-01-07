Operating Systems Lab 7

Aim: To implement contiguous memory allocation schemes

Summary:
- This program runs a simulation of a memory policy on a hypothetical machine.
- Jobs of random sizes are generated and are allocated a partition in the memory according to the given allocation scheme.
- In case there isn't a hole large enough for a partition, existing jobs are terminated until there is sufficient free space.
- Memory statistics are calculated over 1000 steps (after running 1000 warmup steps to ensure equlibrium).
- Memory size: 1024 KB

Jobs:
1. Profile 1 - Any sized jobs (size between 1 and 1024)
2. Profile 2 - Small jobs (size between 1 and 100)
3. Profile 3 - Large jobs (size between 500 and 1000)

Replacement policies:
1. Policy 1 - Best Fit
2. Policy 2 - Worst Fit
3. Policy 3 - First Fit

To run:
./a.out <job_profile> <replacement_policy>
Examples:
./a.out 1 3 (Any size job, First Fit)
./a.out 2 1 (Small jobs, Best Fit)

Output: 
A summary of job allocation at each step is printed out.
The statistics over 1000 steps are written to output.csv file.

Example:
1. Generated job of size 84
Hole found at location 0
Created new partition of size 84 at location 0
Inserted job at location 0
2. Generated job of size 87
Hole found at location 1
Created new partition of size 87 at location 1
Inserted job at location 1
3. Generated job of size 78
Hole found at location 2
Created new partition of size 78 at location 2
Inserted job at location 2
etc.

summary.pdf - Contains the statistics of running the program over all 9 combinations of input over 1000 time steps each.

