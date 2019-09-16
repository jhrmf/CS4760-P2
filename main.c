#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>

struct subset{
    int size, sum;
    int *subset;
};

bool isSubsetSum(int set[], int n, int sum)
{
    // Base Cases
    if (sum == 0){
        return true;
    }
    if (n == 0 && sum != 0) {
        return false;
    }
    // If last element is greater than sum, then ignore it
    if (set[n-1] > sum) {
        return isSubsetSum(set, n - 1, sum);
    }
    /* else, check if sum can be obtained by any of the following
       (a) including the last element
       (b) excluding the last element   */
    return isSubsetSum(set, n-1, sum) ||
           isSubsetSum(set, n-1, sum-set[n-1]);
}

void getSubset(int set[], int n, int sum, char finalSet[]){
    if(sum == 0){
        printf("%s", finalSet);
    }
    if(sum != 0 && n == 0){
        //
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

    //kill all processes here
    exit(0);
}

struct subset seperateString(char str[]){
    static int arr[100];
    char tempStr[5];
    int i;
    int n = 0;
    int first, spaceCount = 0;
    int digitLen = 0;
    for(i = 0; str[i] != '\n'; i++){

        if(str[i] == ' '){
            if(spaceCount == 0){
                digitLen = 0;
                first = atoi(tempStr);
                int x;
                for(x = sizeof(tempStr); x > -1; x--){
                    tempStr[x] = 0;
                }
                n++;
                spaceCount++;
            }
            else{
                digitLen = 0;
                arr[n] = atoi(tempStr);
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
        }
    }
    struct subset sub;
    sub.size = sizeof(arr);
    sub.subset = arr;
    sub.sum = first;
    return sub;
}

int main(int argc, char *argv[]){

    int opt = 0;
    char inputFile[100] = "input.dat";
    char outputFile[100] = "output.dat";
    char buffer[100];
    int time = 1;

    while ((opt = getopt(argc, argv,"hi:o:t:")) != -1) {   //GET OPT WOOOOO
        switch (opt) {  //start a switch statement for the commandline arguments
            case 'h' :      //print help message
                printf("Haha, you want a help message you are cool.");
                break;
            case 'i' :      //set input file name
                break;
            case 'o' :      //set output file name
                break;
            case 't' :
                break;
            default:        //if nothing is listed print an error
                perror("Command line argument not recognized.");
                exit(EXIT_FAILURE);
        }

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
            struct subset tempSub = seperateString(buffer);
            int x;
            for(x = 0; x < tempSub.size; x++){
                printf("%d ", tempSub.subset[x]);
            }
            if(isSubsetSum(tempSub.subset, tempSub.size, tempSub.sum) == true){
                printf("Yes\n");
            }
            else{
                printf("nope\n");
            }
            printf("we would fork %d processes?\n", n);
        }
        line++;
    }
    fclose(inptr);
    fclose(outptr);

    timer(1);




}
