#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void logging(char* text);

int main (void)
{

logging("Web server started!");
}


/* 

This method is implemented to create a simple logging system that logs actions and append it to a log file.
It also takes the string and post it in a structured design in the console for better maintainability

*/

void logging(char* text) {
time_t currentTime;
time(&currentTime);

FILE *logFile = fopen("Logfile.txt", "a");
if (logFile == NULL)
{
    printf("Logfile doesn't exist\n");
    exit(0);
}
fprintf(logFile, "%s: %s\n", ctime(&currentTime), text);
printf("==========================================\n");
printf("%s: %s\n", ctime(&currentTime), text);
}
