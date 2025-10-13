#include<stdio.h>
#include<unistd.h>

int main(){
    int a = 10;
    int pid = fork();
    printf("Process ID: %d, a = %d\n", getpid(), a);
    printf("Enter new value of a: ");
    scanf("%d", &a);
    printf("New value of a is: %d\n", a);
    return 0;
}