#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

// code from https://stackoverflow.com/questions/19461744/how-to-make-parent-wait-for-all-child-processes-to-finish

struct subset{
    int size, sum;
    int subset[100];
};


bool getSubsetSum(int set[], int n, int sum)
{

    if (sum == 0){
        return true;
    }
    if (n == 0 && sum != 0) {
        return false;
    }

    if (set[n-1] > sum) {
        return getSubsetSum(set, n - 1, sum);
    }

    if(getSubsetSum(set, n-1, sum) == true){
        return true;
    }
    else if(getSubsetSum(set, n-1, sum-set[n-1])){
        printf("%d ", set[n-1]);
        return true;
    }
    else{
        return false;
    }

}


void timer(int timeInSec){
    time_t now;

    time(&now);
    struct tm *local = localtime(&now);
    int seconds = local->tm_sec;
    int temp = seconds;
    while(temp - seconds != timeInSec){
        time(&now);
        local = localtime(&now);
        temp = local->tm_sec;
    }

}

struct subset seperateString(char str[]){
    struct subset sub;
    char tempStr[5];
    int i;
    int n = 0;
    int first, spaceCount = 0;
    int digitLen = 0;

    sub.size = 1;

    for(i = 0; i < 100; i++){
        sub.subset[i] = 0;
    }
    for(i = 0; str[i] != '\n'; i++){

        if(str[i] == ' '){
            if(spaceCount == 0){
                digitLen = 0;
                first = atoi(tempStr);
                int x;
                for(x = sizeof(tempStr); x > -1; x--){
                    tempStr[x] = 0;
                }
                spaceCount++;
            }
            else{
                digitLen = 0;
                sub.subset[n] = atoi(tempStr);
                sub.size++;
                int x;
                for (x = sizeof(tempStr); x > -1; x--) {
                    tempStr[x] = 0;
                }
                n++;
            }
        }
        else{
            tempStr[digitLen] = str[i];
            digitLen++;
            if(str[i+1] =='\n'){
                sub.subset[n] = atoi(tempStr);
            }
        }
    }
    sub.sum = first;
    return sub;
}

int fileEmpty(char fileName[]){    //function to check if file is empty

    if( access( fileName, F_OK ) != -1 ) {
        FILE *ptr = fopen(fileName, "r");  //open the level file
        fseek(ptr, 0, SEEK_END);    //seek to the end
        if(ftell(ptr) == 0){    //if the file is empty
            fclose(ptr);    //close the file and return 0
            return 0;
        }
        fclose(ptr);    //otherwise it must not be empty so return 1
        return 1;
    }
    else {
        perror("File does not exist!\n");
        exit(0);
    }
}

int main(int argc, char *argv[]){

    int opt = 0;
    char inputFile[100] = "input.dat";
    char outputFile[100] = "output.dat";
    char buffer[100];
    int timeInSec = 1;

    while ((opt = getopt(argc, argv,"hi:o:t:")) != -1) {   //GET OPT WOOOOO
        switch (opt) {  //start a switch statement for the commandline arguments
            case 'h' :      //print help message
                printf("Haha, you want a help message you are cool.");
                break;
            case 'i' :      //set input file name
                strcpy(inputFile, "");
                strcpy(inputFile, optarg);
                printf("%s\n", inputFile);
                break;
            case 'o' :      //set output file name
                strcpy(outputFile, "");
                strcpy(outputFile, optarg);
                printf("%s\n", outputFile);
                break;
            case 't' :
                timeInSec = atoi(optarg);
                break;
            default:        //if nothing is listed print an error
                perror("Command line argument not recognized.");
                exit(EXIT_FAILURE);
        }

    }
    int inSize = strlen(inputFile);
    int outSize = strlen(outputFile);
    int testing[4] = {4, 3, 2, 1};
    getSubsetSum(testing, sizeof(testing)/sizeof(testing[0]), 9);
    printf("\n");
    exit(0);

    if(inputFile[inSize-1] == 't'){
        if(inputFile[inSize-2] == 'a'){
            if(inputFile[inSize-3] == 'd'){
                if(inputFile[inSize-4] == '.'){
                    printf("Valid File\n");
                }else{perror("Input file not .dat"); exit(0);}
            }else{perror("Input file not .dat"); exit(0);}
        }else{perror("Input file not .dat"); exit(0); }
    }else{perror("Input file not .dat"); exit(0);}

    if(outputFile[outSize-1] == 't'){
        if(outputFile[outSize-2] == 'a'){
            if(outputFile[outSize-3] == 'd'){
                if(outputFile[outSize-4] == '.'){
                    printf("Valid File\n");
                }else{perror("Output file not .dat"); exit(0);}
            }else{perror("Output file not .dat"); exit(0);}
        }else{perror("Output file not .dat"); exit(0); }
    }else{perror("Output file not .dat"); exit(0);}

    if(fileEmpty(inputFile) == 0){
        printf("Input File is empty.\n");
        exit(0);
    }

    int line = 0;
    int n = 0;
    FILE *inptr = fopen(inputFile, "r");
    FILE *outptr = fopen(outputFile, "w");


    while(fgets(buffer, sizeof(buffer), inptr)){
        if(line == 0){
            fputs(buffer, inptr);
            n = atoi(buffer);
        }
        else{
            int x;
            int status;
            pid_t p, wpid;
            printf("PID of Parent is %d\n", getpid());
            for(x = 0; x < n; x++){
                pid_t childPid;  // the child process that the execution will soon run inside of.
                childPid = fork();
                if(x != 0){
                    fgets(buffer, sizeof(buffer), inptr);
                }
                if(childPid == 0)  // fork succeeded
                {
                    line++;
                    fputs(buffer, inptr);
                    printf("ITERATION %d\n", x+1);
                    printf("%s", buffer);
                    printf("PID of Child is %d\n", getpid());
                    struct subset tempSub = seperateString(buffer);
                    if (getSubsetSum(tempSub.subset, tempSub.size, tempSub.sum) == true) {
                        printf("Yes\n");
                    } else {
                        printf("nope\n");
                    }
                    exit(0);
                }

                else if(childPid < 0)  // fork failed
                {
                    perror("Error while attempting fork.\n");
                }

                else  // Main (parent) process after fork succeeds
                {
                    int returnStatus = 0;
                    //waitpid(childPid, &returnStatus, 0);

                    time_t now;
                    time(&now);
                    struct tm *local = localtime(&now);
                    int seconds = local->tm_sec;
                    int temp = seconds;
                    while(temp - seconds != timeInSec){
                        time(&now);
                        local = localtime(&now);
                        temp = local->tm_sec;
                    }
                    if(kill(childPid, 0) != 0){
                        kill(childPid, SIGKILL);
                    }

                }

            }

            break;

        }
        line++;
    }
    fclose(inptr);
    fclose(outptr);



}
