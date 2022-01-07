#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Page Replacement Algorithms
// Author: Shagufta Anjum
// Roll no: 19XJ1A0568

// Returns the position of given element in the array
int search(int x, int* frames, int N){
    for (int i=0; i<N; i++){
        if (frames[i] == x)
            return i;
    }
    return -1;
}

void printFrames(int* frames, int N){
    for (int i=0; i<N; i++){
        if (frames[i] == -1){
            printf(" \t");
            continue;
        }
        printf("%d\t", frames[i]);
    }
    printf("\n");
}

int fifo(int N, int S, int* R, int* frames){
    printf("FIFO Algorithm\n");
    int misses = 0;
    int frame_pos = 0;
    for (int i=0; i<S; i++){
        int page = R[i]; // Page to be inserted

        if (search(page, frames, N) < 0){ // Page miss
            misses++;

            // If there is space available, insert page
            if (frame_pos < N){  
                frames[frame_pos] = page;
                frame_pos++;
            }
            // If frames are full, replace oldest page
            else{  
                frame_pos = 0;
                frames[frame_pos] = page;
                frame_pos++;
            }
        }
        printFrames(frames, N);
    }
    return misses;
}

int optimal(int N, int S, int* R, int* frames){
    printf("Optimal Algorithm\n");
    int misses = 0;
    int filled_pos = 0;

    for (int i=0; i<S; i++){
        int page = R[i]; // Page to be inserted

        // Page hit, continue
        if (search(R[i], frames, N) >= 0){
            printFrames(frames, N);
            continue;
        }

        // Page miss
        misses++;
        // If there is space available, insert page
        if (filled_pos < N){ 
                frames[filled_pos] = page;
                filled_pos++;
        }
        // If frames are full, replace the page farthest away in the future
        else{
            int res = -1, farthest = i+1, flag = 0;

            // Compare each frame with remaining string and update the farthest element
            for (int k = 0; k < N; k++) {
                int j;
                for (j = i+1; j < S; j++) {
                    if (frames[k] == R[j]) {
                        if (j > farthest) {
                            farthest = j;
                            res = k;
                        }
                        break;
                    }
                }

                // If a frame is never referenced in future, replace it
                if (j == S){
                    frames[k] = page;
                    flag = 1;
                    break;
                }
            }
           
            if (flag == 0){
                // If none of the frames were referenced in the future, replace the first one
                if (res == -1)
                    frames[0] = page;
                else
                    frames[res] = page;
            }
        }
        printFrames(frames, N);
    }
    return misses;
}

int lru(int N, int S, int* R, int* frames){
    printf("LRU Algorithm\n");
    int misses = 0;
    int filled_pos = 0;

    for (int i=0; i<S; i++){
        int page = R[i]; // Page to be inserted

        // Page hit, continue
        if (search(R[i], frames, N) >= 0){
            printFrames(frames, N);
            continue;
        }

        // Page miss
        misses++;
        // If there is space available, insert page
        if (filled_pos < N){ 
                frames[filled_pos] = page;
                filled_pos++;
        }
        // If frames are full, replace the page farthest away in the future
        else{
            int res = -1, farthest = i-1, flag = 0;

            // Compare each frame with preceding string and update the farthest element
            for (int k = 0; k < N; k++) {
                int j;
                for (j = i-1; j >= 0; j--) {
                    if (frames[k] == R[j]) {
                        if (j < farthest) {
                            farthest = j;
                            res = k;
                        }
                        break;
                    }
                }
                // If a frame is never referenced in the past, replace it
                if (j < 0){
                    frames[k] = page;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){  
                 // If none of the frames were referenced in the past, replace the first one
                if (res == -1)
                    frames[0] = page;
                else
                    frames[res] = page;
            }
        }
        printFrames(frames, N);
    }
    return misses;
}

int main(int argc, char* argv[]){

    if (argc < 3){
        printf("Insufficient input arguments.\n");
        return 1;
    }

    int N = atoi(argv[1]); // No. of frames
    int S = atoi(argv[2]); // Length of reference string

    if (argc < 4+S){
        printf("Insufficient input arguments.\n");
        return 1;
    }
 
    int* R = (int*)malloc(S*sizeof(int)); // Reference string
    for(int i=0; i<S; i++){
        R[i] = atoi(argv[i+3]);
    }

    int* frames = (int*)malloc(N*sizeof(int)); // Array to store frames in memory
    for(int i=0; i<N;i++){
        frames[i] = -1;
    }

    char* A = argv[S+3]; // Algorithm

    int page_faults = 0;

    if (strcmp(A, "FIFO") == 0)
        page_faults = fifo(N, S, R, frames);
    else if (strcmp(A, "OPTIMAL") == 0)
        page_faults = optimal(N, S, R, frames);
    else if (strcmp(A, "LRU") == 0)
        page_faults = lru(N, S, R, frames);
    else{
        printf("Wrong algorithm.\n");
        return 1;
    }

    printf("Page faults: %d\n", page_faults);

    free(R);
    free(frames);
    return 0;
}
