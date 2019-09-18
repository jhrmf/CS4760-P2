# CS4760-P2
Assignment 2
CS4760 Fall Semester
Joseph Hill
Repository - https://github.com/jhrmf/CS4760-P2

Invocation of project:
- logParse [-h] [-i inputFile] [-o outputFile] [-t timeForExecution]

Project Description
Create a program that will find if a given subset has a valid sum based on a given number as the sum, and a random set. This will be accomplished using child processes that will execute n lines of a provided input or output file, with the extension ".dat". In the case that no file is provided, it will default input.dat and output.dat respectively. If subsets are found, they will be printed in a standard equation format, and if not, a corresponding message will be given, all outputs printed into an output file. If a time limit is provided by the user, the program will have n seconds of execution, but will not exceed that time. Each subset has 1 second to be evaluated until termination.

Arguments
h Print a help message to the user
i Use the given input file name
o Use the given output file name
t End the program after n seconds

Note: I cannot report any unknown errors or segmentation faults with the program. To my understanding, the program meets all specifications asked, but does also implement a couple that have been assumed. Examples of these are ensuring the user is using the extension .dat when providing any file names. It will then print whether the file is valid or invalid based on this specification.

Any code that is not my own was taken from Linux Man pages, or from code provided us with the project assignment (i.e. the GeeksForGeeks function for subset sum, and the code from the textbook periodicasterisk.c)

perror() was used for printing an error when the file does not exist, as well as when the command line argument is not recognized. getopt() was used to process the commandline arguments.

Thanks!
