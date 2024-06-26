#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ProducerConsumerUtilities.h"
#include "Consumer.h"
#include "Producer.h"

#define PRODUCER_NUMBER 2
#define CONSUMER_NUMBER 10

int main(int argc, char *argv[]){
    int idx = 0;

    // 分配内存以存储生产者线程结构体
    ThreadStruct_t *producerThreadStructPtr = (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t) *PRODUCER_NUMBER);
    // 分配内存以存储消费者线程结构体
    ThreadStruct_t *consumerThreadStructPtr = (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t) *CONSUMER_NUMBER);
    // 创建生产者消费者管理器
    ProducerConsumerManager_t *producerConsumerMgr = createProducerConsumerManager();
    if(NULL==producerThreadStructPtr||NULL==consumerThreadStructPtr||NULL==producerConsumerMgr)
    {
        perror("malloc fail!\n");
        exit(EXIT_FAILURE);
    }

    // 创建生产者线程
    for(idx = 0; idx < PRODUCER_NUMBER; ++idx){
        ThreadStruct_t *curProducerThreadStruct = producerThreadStructPtr + idx;
        initThreadStruct(curProducerThreadStruct, producerConsumerMgr, idx);
        pthread_create(&curProducerThreadStruct->tid, NULL, producerThread, (void *)curProducerThreadStruct);
    }

    // 创建消费者线程
    for(idx = 0; idx < CONSUMER_NUMBER; ++idx){
        ThreadStruct_t *curConsumerThreadStruct = consumerThreadStructPtr + idx;
        initThreadStruct(curConsumerThreadStruct, producerConsumerMgr, idx);
        pthread_create(&curConsumerThreadStruct->tid, NULL, consumerThread, (void *)curConsumerThreadStruct);
    }


    do{
        // 从标准输入获取一个字符
        char aChar = getchar();
        if (aChar == 'q')
        {
            // 设置退出标志为1，通知线程退出
            producerConsumerMgr->exit_flag = 1;
        }
    }while(producerConsumerMgr->exit_flag == 0);


    // 等待生产者线程退出
    for(idx = 0; idx < PRODUCER_NUMBER; ++idx){
        ThreadStruct_t *curProducerThreadStruct = producerThreadStructPtr + idx;
        pthread_join(curProducerThreadStruct->tid, NULL);
    }

    // 等待消费者线程退出
    for(idx = 0; idx < CONSUMER_NUMBER; ++idx){
        ThreadStruct_t *curConsumerThreadStruct = consumerThreadStructPtr + idx;
        pthread_join(curConsumerThreadStruct->tid, NULL);
    }

    // 释放内存
    free(producerThreadStructPtr);
    free(consumerThreadStructPtr);
    semDestroy(&(producerConsumerMgr->mutex));
    semDestroy(&(producerConsumerMgr->full));
    semDestroy(&(producerConsumerMgr->empty));
    free(producerConsumerMgr);
    return 1;
}