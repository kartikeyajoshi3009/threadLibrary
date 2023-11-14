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
#endif
