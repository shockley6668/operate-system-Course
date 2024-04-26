#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "Consumer.h"
#include "ProducerConsumerUtilities.h"

void getProduct(ProducerConsumerManager_t *producerConsumerMgr, int threadIdx){
	sem_t *mutex = &(producerConsumerMgr->mutex); // 获取互斥信号量
	sem_t *full = &(producerConsumerMgr->full); // 获取满缓冲区信号量
	sem_t *empty =  &(producerConsumerMgr->empty); // 获取空缓冲区信号量
	int productGetted;

	semWait(full); // 等待缓冲区非空
	semWait(mutex); // 获取互斥信号量

	productGetted = producerConsumerMgr->buffer[producerConsumerMgr->consumerOffset]; // 从缓冲区获取产品
	producerConsumerMgr->consumerOffset = (producerConsumerMgr->consumerOffset + 1) % producerConsumerMgr->bufferSize; // 更新消费者偏移量
	--producerConsumerMgr->remainProducts; // 更新剩余产品数量
	printf("Consumer:%d, get product:%d\n",threadIdx, productGetted); // 打印消费者线程和获取的产品信息

	semSignal(empty); // 释放空缓冲区信号量
	semSignal(mutex); // 释放互斥信号量
}

void *consumerThread(void *arg){
	ThreadStruct_t *curThreadStruct = (ThreadStruct_t *)arg; // 获取线程结构体指针
	ProducerConsumerManager_t *producerConsumerMgr = curThreadStruct->producerConsumerMgr; // 获取生产者消费者管理器指针

	while(curThreadStruct->producerConsumerMgr->exit_flag == 0){ // 当退出标志为0时循环执行
		getProduct(producerConsumerMgr, curThreadStruct->threadIdx); // 获取产品
		sleep(WAIT_TIME_IN_SECOND); // 线程休眠WAIT_TIME_IN_SECOND秒
	}

	return arg; // 返回线程结构体指针
}
