#include"../head/ThreadPool.h"
#include<cstdio>
void task1(){
    printf("task1\n");
}
void task2(int a,int b){
    printf("tansk2%d\n",a+b);
}
void task3(){
    printf("task3\n");
}
int main(){
    ThreadPool pool("test_threadpool");
    pool.start(5);
    pool.run(task1);
    pool.run(std::bind(task2,1,2));
    pool.run(task3);
    while(true){};//这里要用一个loop循环保持，要不然还未执行完任务程序就退出了
    return 0;
}