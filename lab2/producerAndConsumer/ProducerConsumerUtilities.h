#ifndef PRODUCERCONSUMER_UTILITIES_HEADER
#define PRODUCERCONSUMER_UTILITIES_HEADER

#include <semaphore.h>
#include <pthread.h>

#include <stdio.h>

#define BUFFER_SIZE 10 // 缓冲区大小

#define WAIT_TIME_IN_SECOND 1 // 等待时间（秒）

typedef struct {
    sem_t mutex; // 用于以独占方式访问缓冲区，初始值为1
    sem_t full; // 用于指示缓冲区中已填充产品的盒子数量
    sem_t empty; // 用于指示缓冲区中空盒子的数量

    int producerOffset; // 生产者偏移量
    int consumerOffset; // 消费者偏移量

    int remainProducts; // 剩余产品数量
    int buffer[BUFFER_SIZE]; // 缓冲区
    int bufferSize; // 缓冲区大小

    int exit_flag; // 退出标志
} ProducerConsumerManager_t;

typedef struct 
{
    ProducerConsumerManager_t *producerConsumerMgr; // 生产者消费者管理器指针
    pthread_t tid; // 线程ID
    int threadIdx; // 线程索引
} ThreadStruct_t;

ProducerConsumerManager_t *createProducerConsumerManager(); // 创建生产者消费者管理器

void initThreadStruct(ThreadStruct_t *threadStruct, ProducerConsumerManager_t *producerConsumerMgr, int threadIdx); // 初始化线程结构体
ThreadStruct_t *createThreadStruct(ProducerConsumerManager_t *producerConsumerMgr, int threadIdx); // 创建线程结构体

void semWait(sem_t *sem); // 等待信号量
void semSignal(sem_t *sem); // 发送信号量
void semDestroy(sem_t *sem); // 销毁信号量
#endif