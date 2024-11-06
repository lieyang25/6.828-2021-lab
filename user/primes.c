#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 在 main 函数之前声明 helper 函数原型
void helper(int input);

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Error: primes failed...\n");
        exit(1);
    }

    int pip[2];
    pipe(pip);

    int pid = fork();
    if (pid == -1) {
        fprintf(2, "Error: fork failed...\n");
        exit(1);
    }

    if (pid == 0) {
        // 子进程调用 helper 函数，传入文件描述符 pip[0]
        close(pip[1]); // 关闭子进程中的写端
        helper(pip[0]);
    } else {
        close(pip[0]); // 父进程关闭读端
        for (int i = 2; i <= 35; i++) {
            write(pip[1], &i, sizeof(i));
        }
        close(pip[1]); // 关闭写端以结束输入
        wait(0);       // 等待子进程完成
    }

    exit(0);
}

// 筛选素数的递归函数
void helper(int input) {
    int prime;

    // 读取第一个素数，如果没有数据则退出
    if (read(input, &prime, sizeof(prime)) == 0) {
        close(input);
        exit(0);
    }

    printf("prime %d\n", prime);

    int pip[2];
    pipe(pip);

    int pid = fork();
    if (pid == 0) {
        // 在新进程中递归调用 helper 处理下一个素数
        close(pip[1]); // 关闭写端
        helper(pip[0]);
    } else {
        close(pip[0]); // 父进程关闭读端

        int x;
        // 将不能被 prime 整除的数传递到下一个管道
        while (read(input, &x, sizeof(x)) > 0) {
            if (x % prime != 0) {
                write(pip[1], &x, sizeof(x));
            }
        }

        close(input);   // 关闭输入端
        close(pip[1]);  // 关闭写端
        wait(0);        // 等待子进程完成
    }

    exit(0);
}
