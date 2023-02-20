#include"Type.h"
#include<pthread.h>
#include<sys/syscall.h>
#include<functional>
#include<unistd.h>
//Thread类不是线程，而是用来控制线程和保存线程状态的类
class Thread
{
private:
    pid_t tid_;//线程id,使用系统调用SYS_gettid，全局唯一
    const string name_;//该线程在线程池中的名字——线程池的名字+初始化当前线程时的序号
    pthread_t  pthreadId_;
    bool started_;
    bool join_;
private:
    static void *startThread(void*);
public:
    typedef std::function<void ()> ThreadFunc;
    ThreadFunc func_;//进程函数
    Thread(ThreadFunc func_,const string &name);
    ~Thread(){};
    void start();
    int join();
};

