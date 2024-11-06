#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[])
{
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
        read(pip[0],&x,sizeof(x));
        helper(x);
    }
    else
    {
        if(write(pip[1],&x,sizeof(x)) >= 0){}
        wait(0);

    }

    exit(0);
}
void helper(int x)
{

}