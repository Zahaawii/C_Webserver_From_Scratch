#include <stdio.h>

void logging(char* text);

int main (void)
{
logging("Starting application");
}



void logging(char* text) 
{
printf("==========================================\n");
printf("%s\n", text);
printf("==========================================\n");
}
