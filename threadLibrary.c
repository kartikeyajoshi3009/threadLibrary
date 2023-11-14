#include"threadLibrary.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int createThread(threadLib* tL, void (*function)(void*), void* arg){
    int id=tL->nextThreadId++;
    tL->threads=(threadControlBlock*)realloc(tL->threads, sizeof(threadControlBlock) * (tL->threadCount+1));
    threadControlBlock* tcb=&(tL->threads[tL->threadCount]);
    tcb->id=id;
    tcb->status=READY;
    tcb->function=function;
    tcb->arg=arg;
    pthread_mutex_init(&(tcb->mutex), NULL);
    pthread_cond_init(&(tcb->condition), NULL);
    int result=pthread_create(&(tcb->thread), NULL, function, arg);
    if(result==0){
        tL->threadCount++;
        return id;
    } 
    else{
        free(tcb);
        return -1;
    }
}
void deleteThread(threadLib* tL, int threadId){}
void suspendThread(threadLib* tL, int threadId){}
void resumeThread(threadLib* tL, int threadId){}
void runThread(threadLib* tL, int threadId){}
void sleepThread(threadLib* tL, int threadId, int t){}
void showThreadStatus(threadLib* tL, int threadId){}
