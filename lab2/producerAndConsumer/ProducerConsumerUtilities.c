#include <stdio.h>
#include <stdlib.h>

#include "ProducerConsumerUtilities.h"

// 初始化生产者消费者管理器
void initProducerConsumerManager(ProducerConsumerManager_t *producerConsumerMgr){
    sem_init(&producerConsumerMgr->mutex,0, 1); // 初始化互斥信号量
    sem_init(&producerConsumerMgr->full,0, 0); // 初始化满缓冲区信号量
    sem_init(&producerConsumerMgr->empty,0, BUFFER_SIZE); // 初始化空缓冲区信号量

    producerConsumerMgr->producerOffset = 0; // 初始化生产者偏移量
    producerConsumerMgr->consumerOffset = 0; // 初始化消费者偏移量
    producerConsumerMgr->bufferSize = BUFFER_SIZE; // 初始化缓冲区大小

    producerConsumerMgr->exit_flag = 0; // 初始化退出标志
}

// 创建生产者消费者管理器
ProducerConsumerManager_t *createProducerConsumerManager(){
    ProducerConsumerManager_t *producerConsumerMgr = (ProducerConsumerManager_t *)malloc(sizeof(ProducerConsumerManager_t));

    if(producerConsumerMgr == NULL){
        printf("Error: unable to create producerConsumerMgr\n");
        return NULL;
    }

    initProducerConsumerManager(producerConsumerMgr);

    return producerConsumerMgr;
}

// 初始化线程结构体
void initThreadStruct(ThreadStruct_t *threadStruct, ProducerConsumerManager_t *producerConsumerMgr, int threadIdx){
    threadStruct->producerConsumerMgr = producerConsumerMgr; // 设置线程结构体的生产者消费者管理器指针
    threadStruct->threadIdx = threadIdx; // 设置线程结构体的线程索引
}

// 创建线程结构体
ThreadStruct_t *createThreadStruct(ProducerConsumerManager_t *producerConsumerMgr, int threadIdx){
    ThreadStruct_t *threadStruct = (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t));

    if(threadStruct == NULL){
        printf("Error: unable to create ThreadStruct_t\n");
        return NULL;
    }

    initThreadStruct(threadStruct, producerConsumerMgr, threadIdx);

    return threadStruct;
}

// 等待信号量
void semWait(sem_t *sem){
    int ret=sem_wait(sem);
    if(ret<0)
    {
        perror("sem_wait fail!"); // 打印错误信息
        exit(EXIT_FAILURE); // 退出程序
    }
}

// 发送信号量
void semSignal(sem_t *sem){
    int ret=sem_post(sem);
    if(ret<0)
    {
        perror("sem_post fail!\n"); // 打印错误信息
        exit(EXIT_FAILURE); // 退出程序
    }
}

// 销毁信号量
void semDestroy(sem_t* sem)
{
    int ret=sem_destroy(sem);
    if(ret<0)
    {
        perror("sem_destroy run fail\n"); // 打印错误信息
        exit(EXIT_FAILURE); // 退出程序
    }
}