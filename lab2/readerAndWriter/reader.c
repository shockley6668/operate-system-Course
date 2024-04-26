#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 50

char buffer[BUFFER_SIZE]; // 缓冲区
int read_count = 0; // 读者数量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // count互斥锁

pthread_mutex_t rw=PTHREAD_MUTEX_INITIALIZER; //读写锁
pthread_mutex_t w=PTHREAD_MUTEX_INITIALIZER;    //实现写者优先
void* writer(void* arg) {
    while (1)
    {
        char* message = (char*)arg;
        pthread_mutex_lock(&w);
        pthread_mutex_lock(&rw);
        
        // 写入缓冲区
        printf("Writer: Writing '%s' to buffer\n", message);
        snprintf(buffer, BUFFER_SIZE, "%s", message);
        pthread_mutex_unlock(&rw);
        pthread_mutex_unlock(&w);

        
    }
    
    
    
    return NULL;
}

void* reader(void* arg) {
    while (1)
    {
        pthread_mutex_lock(&w);
        pthread_mutex_lock(&mutex);  //各读进程互斥访问count
        
        if (read_count== 0) { 
            pthread_mutex_lock(&rw); // 第一个读者加锁
        }
        
        read_count++; // 增加读者数量
        pthread_mutex_unlock(&mutex); // count解锁
        
        // 从缓冲区读取
        printf("Reader: Reading '%s' from buffer\n", buffer);
        pthread_mutex_unlock(&w);
        pthread_mutex_lock(&mutex); // 加锁
        read_count--; // 减少读者数量
        if (read_count == 0) { // 如果没有读者了，则唤醒等待的写者
            pthread_mutex_unlock(&rw); // 最后一个读者解锁
        }
        pthread_mutex_unlock(&mutex); // 解锁
        
    }
    return NULL;
    
}

int main() {
    pthread_t writer_thread, reader_thread1, reader_thread2;
    
    // 创建写者线程
    pthread_create(&writer_thread, NULL, writer, "Hello, Writer!");
    
    // 创建读者线程
    pthread_create(&reader_thread1, NULL, reader, NULL);
    pthread_create(&reader_thread2, NULL, reader, NULL);
    
    // 等待线程结束
    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread1, NULL);
    pthread_join(reader_thread2, NULL);
    
    return 0;
}
