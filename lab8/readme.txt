Operating Systems Lab 8

Name: Shagufta Anjum
Roll no: 19XJ1A0568

Aim: To implement the following Page Replacement Algorithms:
     - First In First Out (FIFO)
     - Optimal
     - Least Recentlly Used (LRU)

Input: Command Line arguments
1. N, the number of frames for the given process
2. S, the size of the reference string
3. R, the reference string where references are separated with space
4. A, the replacement algorithm, FIFO, OPTIMAL or LRU.

Output: The frame contents at each stage and number of page faults.

Sample run:
Input: ./a.out 3 10 4 7 6 1 7 6 1 2 7 2 FIFO
Output: 
FIFO Algorithm
4
4       7
4       7       6
1       7       6
1       7       6
1       7       6
1       7       6
1       2       6
1       2       7
1       2       7
Page faults: 6

Logic:
- Sanitization checks are implemented to check the input.
- To insert a new page, the frames array is searched for a page hit.
- If there is a miss, replacement is needed. The appropriate algorithm is used to choose the page to replace.
- In FIFO, we choose the oldest page to replace (first frame in the array).
- In Optimal, we check the remaining reference string to find the frame that is not used for the longest time in the future.
- In LRU, we check the preceding reference string to find the frame that is least recently used.
- The frame contents are printed out in each step.
- The page faults are counted and printed out.