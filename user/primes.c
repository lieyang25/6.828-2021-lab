#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[])
{
    if(argc != 1){
        fprintf(2,"Error: primes failed...\n");
        exit(1);
    }
    int pip[2];
    pipe(pip);
    int x = 35;
    int pid = fork();
    if(pid == -1)
    {
        fprintf(2,"Error: fork failed...\n");
        exit(1);
    }
    if(pid == 0)
    {
        close(0);
        close(pip[1]);
        helper(pip[0]);
    }
    else
    {
        close(pip[0]);
        for(int i=2;i<=35;i++)
            write(pip[1],&i,sizeof(x));
        close(pip[1]);
        wait(0);
    }

    exit(0);
}
void helper(int input)
{
    int prime;
    if(read(input,&prime,sizeof(prime)) == 0)
        return;
    printf("prime %d\n",prime);
    int pip[2];
    pipe(pip);
    int pid = fork();
    if(pid == 0)
    {
        close(0);
        close(pip[1]);
        helper(pip[0]);
    }
    else
    {
        close(pip[0]);
        int x;
        while(read(input,&x,sizeof(x)) != 0){
            if(x % prime == 0)
                write(pip[1],&x,sizeof(x));
        }
        close(input);
        close(pip[1]);
        wait(0);
    }
}