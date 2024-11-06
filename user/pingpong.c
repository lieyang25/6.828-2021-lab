#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
    if(argc != 1){
        fprintf(2,"Error: pingpong failed, check something...\n");
        exit(1);
    }
    int p_to_c[2],c_to_p[2];
    pipe(p_to_c);
    pipe(c_to_p);
    char x = 'x';
    int pid = fork();
    if(pid == -1){
        fprintf(2,"Error: fork...\n");
        exit(1);
    }
    if(pid == 0){
        close(p_to_c[1]);
        close(c_to_p[0]);
        if(read(p_to_c[0],&x,sizeof(x))>0){
            printf("%d: received ping\n",pid);
        }
        if(write(c_to_p[1],&x,sizeof(x) >= 0)){

        }
    }else{
        close(p_to_c[0]);
        close(c_to_p[1]);
        if(write(p_to_c[1],&x,sizeof(x)) >= 0){

        }
        if(read(c_to_p[0],&x,sizeof(x)) > 0){
            printf("%d: received pong\n",pid);
        }
    }
    close(p_to_c[1]);
    close(c_to_p[0]);
    exit(0);
}