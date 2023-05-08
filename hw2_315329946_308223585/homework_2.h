#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> //to use usleep
#include <time.h>
#include <sys/time.h>

#define MAX_THREADS 4096
#define MAX_COUNTERS 100
#define MAX_COUNTER_NAME_LENGTH 15
#define LOG_ENABLED 0
#define START_TIME get_start_time()

struct timeval start_time;

struct timeval get_start_time() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return current_time;
}

/* Define the job struct */
struct job {
    char command[1024];
    struct job *next;
};

/* Define the work queue struct - this is a linked list*/
struct work_queue {
    struct job *head;
    struct job *tail;
    pthread_mutex_t mutex;
};

// Structure to hold thread ID and work queue pointer
struct ThreadData {
    int thread_id;
    struct work_queue* work_queue;
};

//functions decleration:
int is_empty(struct work_queue *queue);
void add_job(struct work_queue *queue, char *command);
struct job *pop_job(struct work_queue *queue);
int create_counter_files(int num_counters);
void* worker_thread(void *arg);
void create_worker_threads(pthread_t* thread_ids, int num_threads, struct work_queue *work_queue);
void dispatcher(const char* cmdfile, int num_threads, struct work_queue *work_queue);
void cleanup(struct work_queue *queue, pthread_t *threads, int num_threads);
void create_log_file(int thread_num);
void log_start_job(int thread_num, long long start_time, char* job_line);
void log_end_job(int thread_num, long long end_time, char* job_line);
void log_dispatcher(long long time, char* cmd_line);
void remove_job(struct work_queue *queue);