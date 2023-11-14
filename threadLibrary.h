#ifndef THREADLIBRARY_H
#define THREADLIBRARY_H
#include<pthread.h>

typedef enum threadStatus{READY, RUNNING, SUSPENDED, TERMINATED}threadStatus;

typedef struct threadControlBlock{
    pthread_t thread;
    int id;
    threadStatus status;
    void(*function)(void *);
    void *arg;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
}threadControlBlock;

typedef struct threadLib
{
    threadControlBlock* threads;
    int threadCount;
    int nextThreadId;
}threadLib;

int createThread(threadLib* tL, void(*function)(void*), void* arg);
void deleteThread(threadLib* tL, int threadId);
void suspendThread(threadLib* tL, int threadId);
void resumeThread(threadLib *tL, int threadId);
void runThread(threadLib *tL, int threadId);
void sleepThread(threadLib *tL, int threadId, int t);
void showThreadStatus(threadLib* tL, int threadId);
#endif
