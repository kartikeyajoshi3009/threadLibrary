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
void deleteThread(threadLib* tL, int threadId){
    int index=-1;
    for(int i=0; i<tL->threadCount; i++){
        if(tL->threads[i].id==threadId){
            index=i;
            break;
        }
    }
    if(index!=-1){
        threadControlBlock* tcb=&(tL->threads[index]);
        pthread_join(tcb->thread, NULL);
        tcb->status=TERMINATED;
        pthread_mutex_destroy(&(tcb->mutex));
        pthread_cond_destroy(&(tcb->condition));
        for(int i=index; i<tL->threadCount-1; i++){
            tL->threads[i]=tL->threads[i+1];
        }
        tL->threadCount--;
        tL->threads=(threadControlBlock*)realloc(tL->threads, sizeof(threadControlBlock) * tL->threadCount);
    }
}
void suspendThread(threadLib* tL, int threadId){
    for(int i=0; i<tL->threadCount; i++){
        if(tL->threads[i].id==threadId){
            pthread_mutex_lock(&(tL->threads[i].mutex));
            tL->threads[i].status=SUSPENDED;
            pthread_mutex_unlock(&(tL->threads[i].mutex));
            break;
        }
    }
}
void resumeThread(threadLib* tL, int threadId){
    for(int i=0; i<tL->threadCount; i++){
        if(tL->threads[i].id==threadId){
            pthread_mutex_lock(&(tL->threads[i].mutex));
            tL->threads[i].status=READY;
            pthread_cond_signal(&(tL->threads[i].condition));
            pthread_mutex_unlock(&(tL->threads[i].mutex));
            break;
        }
    }
}
void runThread(threadLib* tL, int threadId){
    for(int i=0; i<tL->threadCount; i++){
        if(tL->threads[i].id==threadId){
            pthread_mutex_lock(&(tL->threads[i].mutex));
            tL->threads[i].status=RUNNING;
            pthread_create(&(tL->threads[i].thread), NULL, tL->threads[i].function, tL->threads[i].arg);
            pthread_mutex_unlock(&(tL->threads[i].mutex));
            break;
        }
    }
}
void sleepThread(threadLib* tL, int threadId, int t){
    for(int i=0; i<tL->threadCount; i++){
        if(tL->threads[i].id==threadId){
            pthread_mutex_lock(&(tL->threads[i].mutex));
            tL->threads[i].status=SUSPENDED;
            pthread_mutex_unlock(&(tL->threads[i].mutex));
            usleep(t*1000);
            pthread_mutex_lock(&(tL->threads[i].mutex));
            tL->threads[i].status=READY;
            pthread_mutex_unlock(&(tL->threads[i].mutex));
            break;
        }
    }
}
void showThreadStatus(threadLib* tL, int threadId){
    for(int i=0; i<tL->threadCount; i++){
        if(tL->threads[i].id==threadId){
            printf("Thread %d Status: ", tL->threads[i].id);
            switch (tL->threads[i].status)
            {
            case READY:
                printf("READY\n");
                break;
            case RUNNING:
                printf("RUNNING\n");
            case SUSPENDED:
                printf("SUSPENDED\n");
            default:
                printf("TERMINATED\n");
                break;
            }
            break;
        }
    }
}
