#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

// code from https://stackoverflow.com/questions/19461744/how-to-make-parent-wait-for-all-child-processes-to-finish

pid_t mainChild;

static void myhandler(int s) {

    int errsave;
    errsave = errno;
    kill(mainChild, SIGKILL);
    errno = errsave;
}
static int setupinterrupt(void) { /* set up myhandler for SIGPROF */
    struct sigaction act;
    act.sa_handler = myhandler;
    act.sa_flags = 0;
    return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}
static int setupitimer(int n) { /* set ITIMER_PROF for 2-second intervals */
    struct itimerval value;
    value.it_interval.tv_sec = n;
    value.it_interval.tv_usec = 0;
    value.it_value = value.it_interval;
    return (setitimer(ITIMER_PROF, &value, NULL));
}

char finalEquation[] = "";
struct subset{
    int size, sum;
    int subset[200];
    char equation[1000];
};


bool getSubsetSum(int set[], int n, int sum, char equation[])
{

    if (sum == 0){
        return true;
    }
    if (n == 0 && sum != 0) {
        return false;
    }

    if (set[n-1] > sum) {
        return getSubsetSum(set, n - 1, sum, equation);
    }

    if(getSubsetSum(set, n-1, sum, equation) == true){
        return true;
    }
    else if(getSubsetSum(set, n-1, sum-set[n-1], equation)){
        char temp[1000];
        sprintf(temp, "%d", set[n-1]);
        strcat(equation, temp);
        strcat(equation, " + ");
        printf("%s\n", equation);
        strcpy(finalEquation, equation);
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

    //setupinterrupt();
    //setupitimer();


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
    strcpy(sub.equation, "\0");
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
    char buffer[5000];
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


    fgets(buffer, sizeof(buffer), inptr);  //go through the file

    n = atoi(buffer);
    int x;
    int wstatus;
    printf("PID of Parent is %d\n", getpid());
    pid_t childPid, w;
    for(x = 0; x < n; x++){
        childPid = fork();
        fgets(buffer, sizeof(buffer), inptr);
        if(childPid == 0)  // fork succeeded
            {
            line++;
            struct subset tempSub = seperateString(buffer);
            if (getSubsetSum(tempSub.subset, tempSub.size, tempSub.sum, tempSub.equation) == true) {
                tempSub.equation[strlen(tempSub.equation)-2] = '=';
                char temp[10] = {0}, storedPid[1000] = {0};
                sprintf(storedPid, "%d", getpid());
                strcat(storedPid, ": ");
                sprintf(temp, "%d", tempSub.sum);
                strcat(tempSub.equation, temp);
                strcat(tempSub.equation, "\n");
                strcat(storedPid, tempSub.equation);
                fputs(storedPid, outptr);
                exit(0);
            }
            else {
                printf("nope\n");
                exit(0);
            }
            }
        else  // Main (parent) process after fork succeeds
        {
        }
        do {
            w = waitpid(childPid, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                break;
            }
            if (WIFEXITED(wstatus)) {
                printf("exited, status=%d\n", WEXITSTATUS(wstatus));
            } else if (WIFSIGNALED(wstatus)) {
                printf("killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
                printf("continued\n");
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
        timer(1);
    }
    



    fclose(inptr);
    fclose(outptr);
    exit(0);


}
