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

//functions myhandler(), setupinterrupt(), and setuptimer(), were provided to us from the textbook
//I made slight modifications to adjust for a user provided time the program would have until termination

int limitReached = 0;     //integer that will hold 1 or 0, 1 if the time limit is reached, 0 if not, when finding subset

static void myhandler(int s) {
    int errsave;
    errsave = errno;
    exit(0);                                                           //close program in its tracks after timer expires
    errno = errsave;
}
static int setupinterrupt(void) {                                                     /* set up myhandler for SIGPROF */
    struct sigaction act;
    act.sa_handler = myhandler;
    act.sa_flags = 0;
    return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}
static int setupitimer(int n) {                                             /* set ITIMER_PROF for n-second intervals */
    struct itimerval value;
    value.it_interval.tv_sec = n;                //set the timer to however many seconds user enters or default 1 second
    value.it_interval.tv_usec = 0;
    value.it_value = value.it_interval;
    return (setitimer(ITIMER_PROF, &value, NULL));
}


struct subset{   //a structure for the subset equation, holding the size, the sum, the subset, and the equation to print
    int size, sum;
    int subset[200];
    char equation[1000];
};

/*Below code is the code provided us by GeeksForGeeks, with modifications, such as a time limit and returning equation*/
bool getSubsetSum(int set[], int n, int sum, char equation[], int currentSeconds)  //function to evaluate the subset sum
{
    if(getSeconds() - currentSeconds == 1){       //if the current time is one more than one second from our start point
        limitReached = 1;                                                                  //mark time limitReached as 1
        return false;                                                                                     //return false
    }
    if (sum == 0){
        return true;
    }
    if (n == 0 && sum != 0) {
        return false;
    }

    if (set[n-1] > sum) {
        return getSubsetSum(set, n - 1, sum, equation, currentSeconds);
    }

    if(getSubsetSum(set, n-1, sum, equation, currentSeconds) == true){
        return true;
    }
    else if(getSubsetSum(set, n-1, sum-set[n-1], equation, currentSeconds)){
        char temp[1000];                                                                     //create a temporary string
        sprintf(temp, "%d", set[n-1]);               //store the current value in the set that is part of the subset sum
        strcat(equation, temp);                                    //store that value in the overall equation of the sum
        strcat(equation, " + ");                                                 //add an addition sign after the number
        return true;
    }
    else{
        return false;
    }

}

int getSeconds(){                                    //function to retrieve the current second in time from the computer
    time_t now;
    time(&now);                                                                       //get the current time from system
    struct tm *local = localtime(&now);                                                       //specifically the seconds
    int seconds = local->tm_sec;                                                                     //store the seconds
    return seconds;                                                                                          //return it
}

struct subset seperateString(char str[]){      //function to seperate the passed string into the subset structure values
    struct subset sub;                                                                       //create a subset structure
    char tempStr[5];                  //temp string for the first line of the file which has the number of lines to read
    int i;                                                                              //integer for the for loop below
    int n = 0;                                                                 //holds the current element of the subset
    int first, spaceCount = 0;                           //first will hold the sum, and spaceCount is for checking later
    int digitLen = 0;                                            //holds the current element of the digit in the tempStr

    sub.size = 1;                                                                           //set the starting size to 1

    for(i = 0; str[i] != '\n'; i++){                                    //traverse passed string until new line is found

        if(str[i] == ' '){                                                                 //if the character is a space
            if(spaceCount == 0){                                                             //and it is the first space
                digitLen = 0;                                                             //set the digit length to zero
                first = atoi(tempStr);                                                           //store the first value
                int x;                                                                              //for the loop below
                for(x = sizeof(tempStr); x > -1; x--){                                  //initialize the tempStr to null
                    tempStr[x] = 0;                                       //this ensures it is reset for all after cases
                }
                spaceCount++;   //increase the spaceCount so we know we've moved past the first number, which is the sum
            }
            else{                                              //all other cases are numbers in the set, so follow below
                digitLen = 0;                                                                    //set the digitLen to 0
                sub.subset[n] = atoi(tempStr);            //save the tempStr that holds the number in the set to the set
                sub.size++;                                                                      //increase the set size
                int x;                                                                              //for the loop below
                for (x = sizeof(tempStr); x > -1; x--) {                                 //reset the temp string to null
                    tempStr[x] = 0;
                }
                n++;                          //increase n (n is the element in the subset where the numbers are stored)
            }
        }
        else{                                    //otherwise, the character is not a space so we should do the following
            tempStr[digitLen] = str[i];                                                //save the value into the tempStr
            digitLen++;                                                //increase the tempStr element for the next digit
            if(str[i+1] =='\n'){                                        //if the next element in the string is a newline
                sub.subset[n] = atoi(tempStr);                      //save it in the subset as the loop will have exited
            }
        }
    }
    sub.sum = first;                                                        //set the sum to the value we saved in first
    strcpy(sub.equation, "\0");                                              //initialize the structure equation to null
    return sub;                                                                                   //return the structure
}


int fileEmpty(char fileName[], char executable[]){                                  //function to check if file is empty

    if( access( fileName, F_OK ) != -1 ) {
        FILE *ptr = fopen(fileName, "r");                                                          //open the level file
        fseek(ptr, 0, SEEK_END);                                                                       //seek to the end
        if(ftell(ptr) == 0){                                                                      //if the file is empty
            fclose(ptr);                                                                   //close the file and return 0
            return 0;
        }
        fclose(ptr);                                                        //otherwise it must not be empty so return 1
        return 1;
    }
    else {                                                                           //otherwise the file must not exist
        char temp[100];
        strcpy(temp, executable);                                      //store our executable name in a temporary string
        strcat(temp, ": Error");                                               //concatenate the required text for error
        perror(temp);                                        //print the detailed error message using perror as required
        exit(0);                                                                                     //close the program
    }
}

int main(int argc, char *argv[]){

    int opt = 0;
    char inputFile[100] = "input.dat";                                         //set the default input file to input.dat
    char outputFile[100] = "output.dat";                                     //set the default output file to output.dat

     // READER - The buffer size can be adjusted as you need, but I assume quite a large buffer for the line in the file
    char buffer[5000];

    int timeInSec = 1;         //make the default timeInSec (time entire program can run in seconds) to zero as required

    while ((opt = getopt(argc, argv,"hi:o:t:")) != -1) {                                                //GET OPT WOOOOO
        switch (opt) {                                          //start a switch statement for the commandline arguments
            case 'h' :                                                                              //print help message
                printf("Haha, you want a help message you are cool.");
                break;
            case 'i' :                                                                             //set input file name
                strcpy(inputFile, "");
                strcpy(inputFile, optarg);
                break;
            case 'o' :                                                                            //set output file name
                strcpy(outputFile, "");
                strcpy(outputFile, optarg);
                break;
            case 't' :                                                     //set the time entire program can execute for
                timeInSec = atoi(optarg);
                break;
            default:                                                              //user inputted something unrecognized
                perror("Command line argument not recognized.");
                exit(EXIT_FAILURE);
        }

    }

    setupinterrupt();                                                    //start the interrupt and timer for the program
    setupitimer(timeInSec);

    int inSize = strlen(inputFile);                                                  //get string size of the input file
    int outSize = strlen(outputFile);                                               //get string size of the output file

/*-----------------The Below Code checks for the .dat extension. Not required explicitly but I assumed----------------*/
    if(inputFile[inSize-1] == 't'){
        if(inputFile[inSize-2] == 'a'){
            if(inputFile[inSize-3] == 'd'){
                if(inputFile[inSize-4] == '.'){
                    printf("Valid Input File\n");                     //print that the file is valid (is .dat extension)
                }else{perror("Input file not .dat"); exit(0);}
            }else{perror("Input file not .dat"); exit(0);}
        }else{perror("Input file not .dat"); exit(0); }
    }else{perror("Input file not .dat"); exit(0);}

    if(outputFile[outSize-1] == 't'){
        if(outputFile[outSize-2] == 'a'){
            if(outputFile[outSize-3] == 'd'){
                if(outputFile[outSize-4] == '.'){
                    printf("Valid Output File\n");                    //print that the file is valid (is .dat extension)
                }else{perror("Output file not .dat"); exit(0);}
            }else{perror("Output file not .dat"); exit(0);}
        }else{perror("Output file not .dat"); exit(0); }
    }else{perror("Output file not .dat"); exit(0);}
/*-----------------The Above Code checks for the .dat extension. Not required explicitly but I assumed----------------*/

    if(fileEmpty(inputFile, argv[0]) == 0){                                                 //check if the file is empty
        printf("Input File is empty.\n");                                                      //if it is, tell the user
        exit(0);                                                                                      //exit the program
    }

    int n = 0;                                                    //hold the number of lines in the file to be processed
    FILE *inptr = fopen(inputFile, "r");                                               //open the input file for reading
    FILE *outptr = fopen(outputFile, "w");                                            //open the output file for writing


    fgets(buffer, sizeof(buffer), inptr);                                               //get the first line of the file

    n = atoi(buffer);                                                              //get the number of lines to evaluate
    int x;                                                                                              //for loop below
    int wstatus;                                                                                //hold status of process
    printf("PID of Parent is %d\n", getpid());                                                    //print the parent PID
    pid_t childPid, w;

    for(x = 0; x < n; x++){                                                       //enter the loop that will run n times
        childPid = fork();                                                                             //fork upon start
        fgets(buffer, sizeof(buffer), inptr);                                                //get the next line of file
        if(childPid == 0)                              //fork was successful, so the child process will do the following
            {
            struct subset tempSub = seperateString(buffer); //create new subset structure and fill it with file contents
            int currentSecond = getSeconds();                                           //get the current second of time

            /*---------------- This statement evaluates if the subset retrieved is a valid subset sum ----------------*/
            if (getSubsetSum(tempSub.subset, tempSub.size, tempSub.sum, tempSub.equation, currentSecond) == true) {
                tempSub.equation[strlen(tempSub.equation)-2] = '=';    //change the trailing addition symbol to an equal
                char temp[10] = {0}, storedPid[1000] = {0};                                 //initialize strings to null
                sprintf(storedPid, "%d", getpid());                           //store the process id for start of string
                strcat(storedPid, ": ");                                            //add semicolon for formatting after
                sprintf(temp, "%d", tempSub.sum);                                 //save the sum in the temporary string
                strcat(tempSub.equation, temp);               //add that to the end of the equation string of the subset
                strcat(tempSub.equation, "\n");                                       //make sure the end has a new line
                strcat(storedPid, tempSub.equation);               //add the equation after the corresponding process id
                fputs(storedPid, outptr);                           //*sigh* store this LONG string into the output file
                exit(0);                                                                             //close the process
            }
            else {                                    //if the subset function returns false (no sum or not enough time)
                char error[100];                                            //create a string to store the error message
                if(limitReached == 1){                                //if the time limit was reached (limitReached = 1)
                    char temp[20];
                    sprintf(temp, "%d", getpid());                                                //store the process id
                    strcpy(error, temp);
                    strcat(error, ": Process ended after 1 second - No subset found\n");   //concatenate the err message
                    fputs(error, outptr);                                       //write the error message to output file
                    limitReached = 0;                                                      //reset the limitReached to 0
                }
                else{                                       //otherwise, that means there simply was no subset sum found
                    char temp[20];
                    sprintf(temp, "%d", getpid());                                                //store the process id
                    strcpy(error, temp);
                    strcat(error, ": No subset found\n");                                //concatenate the error message
                    fputs(error, outptr);                                       //write the error message to output file
                }
                exit(0);                                                                               //end the process
            }
            }
        else                                                                 // the parent process will do the following
        {
            do {             //a loop found on http://man7.org/linux/man-pages/man2/wait.2.html to wait for child to end
                w = waitpid(childPid, &wstatus, WUNTRACED | WCONTINUED);               //check the status of the process
                if (w == -1) {
                    break;                                                    //if the process is exited, break the loop
                }

                /*----------------------------Uncomment the below code for exit statuses------------------------------*/
                /*if (WIFEXITED(wstatus)) {
                    printf("exited, status=%d\n", WEXITSTATUS(wstatus));
                } else if (WIFSIGNALED(wstatus)) {
                    printf("killed by signal %d\n", WTERMSIG(wstatus));
                } else if (WIFSTOPPED(wstatus)) {
                    printf("stopped by signal %d\n", WSTOPSIG(wstatus));
                } else if (WIFCONTINUED(wstatus)) {
                    printf("continued\n");
                }*/
                /*----------------------------Uncomment the above code for exit statuses------------------------------*/

            } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));        //while the process is not exited or signaled
        }

    }

    fclose(inptr);                                                                  //close the input file when finished
    fclose(outptr);                                                                //close the output file when finished

    exit(0);                                                                                            //end of program


}
