#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int getMin(int a[], int start, int end);

int main(int argc, char* argv[]){
    //seeds the random number generator based on system time
    srand(time(NULL));
    //initializes array and fills each element with a randomly generated number from 1-250
    int a[20] = {0};
    for(int i = 0; i < 20; i++){
        //modulus 250 added to prevent generated number from going over the integer return limit of exit() (which is 255)
        a[i] = rand() % 250 + 1;
    }

    for(int i = 0; i < 20; i++){
        printf("%d\n",a[i]);
    }

    //calculates index of the midpoint of the array
    int midIndex = (sizeof(a) / sizeof(a[0])) / 2;
    //variable for min of first half
    int minOne;
    //variable for min of second half
    int minTwo;
    //overall min of the array
    int min;

    //creates child process
    pid_t childPID = fork();

    //checks to see if thread creation was successful
    if(childPID < 0){
        perror("Fork Failed!");
        exit(1);
    //condition for child process(loops through second half of array)    
    }else if(childPID == 0){
        //calculates min number of this half of array
        minTwo = getMin(a, midIndex, 20);
        //displays min number of Child process
        printf("Child Process(ID-%d) : Min Number - %d\n", getpid(), minTwo);
        exit(minTwo);

    //condition for parent process(loops through first half of array)    
    }else{
        //calculates min number of this half of array
        minOne = getMin(a, 0, midIndex);
        //displays min number of Parent process
        printf("Parent Process(ID-%d) : Min Number - %d\n", getpid(), minOne);

        //parent process waits for child to finish before continuing
        int status;
        wait(&status);
        //parent process retrieves min number from child
        minTwo = WEXITSTATUS(status);
        //compares two min numbers from each thread and determines the overall min number
        if(minOne < minTwo){
            min = minOne;
        }else{
            min = minTwo;
        }
        //displays final min number
        printf("Overall Minimum Number: %d\n",min);
    }
}

//calculates the minimum value between the two argued indices of the array
int getMin(int a[], int start, int end){
    int currentMin = a[start];
    for(int i = start; i < end; i++){
        if(a[i] < currentMin){
            currentMin = a[i];
        }
    }
    return currentMin;
}