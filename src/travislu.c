#include<mosquitto_broker.h>
#include<memory_mosq.h>
#include<errno.h>
#include<time.h>

#define MAX_QUEUE_SIZE 1500000

struct QueueInfo {
	char id[32];
	char topic[16];
	float timestamp;
};

struct QueueInfo sub_queue_info[MAX_QUEUE_SIZE];
struct QueueInfo db_queue_info[MAX_QUEUE_SIZE];
int sub_queue_count = 0;
int db_queue_count = 0;

void travislu_whereis(char *msg)
{
	time_t t = mosquitto_time();
	_mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "%f In %s function", (float)t, msg);
}

void travislu_get_curr_memory()
{
	unsigned long mem_count;
	mem_count = _mosquitto_memory_used();
	_mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "current memory : %lu ", mem_count);
}

void travislu_save_curr_memory(unsigned long *time_info, unsigned long *mem_info, int count)
{
	time_t now;
	unsigned long mem_count;
	mem_count = _mosquitto_memory_used();
	_mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "current memory : %lu ", mem_count);
	*(mem_info+count) = mem_count;
	now = time(NULL);
	*(time_info+count) = (int)now;
}

void travislu_terminate(unsigned long *time_info, unsigned long *mem_info, int count)
{
	errno = 0;
	_mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "MQTT terminate");

	FILE *fp;
	FILE *fp_sub;
	FILE *fp_db;

	fp = fopen("/home/travis/mosquitto/expi/123.txt", "w");
	fp_sub = fopen("/home/travis/mosquitto/expi/sub_queue.txt", "w");
	fp_db = fopen("/home/travis/mosquitto/expi/db_queue.txt", "w");
	if (fp == NULL || fp_sub == NULL || fp_db == NULL) {
		_mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "Write file failed:%s ", strerror(errno));
		exit(1);
	}
	
	for (int i=0; i<=count; i++) {
		fprintf(fp, "%d %lu\n", (int)*(time_info+i), *(mem_info+i));	
	}

	for (int i=0; i<db_queue_count; i++) {
		fprintf(fp_db, "%s %s %f\n", db_queue_info[i].id, db_queue_info[i].topic, db_queue_info[i].timestamp);
	}

	for (int i=0; i<sub_queue_count; i++) {
		fprintf(fp_sub, "%s %s %f\n", sub_queue_info[i].id, sub_queue_info[i].topic, sub_queue_info[i].timestamp);
	}

	fclose(fp);
	fclose(fp_sub);
	fclose(fp_db);
}

void travislu_get_waiting_queue_time(struct mosquitto_msg_store *store)
{
    struct timeval end;
    float waiting_time;

    gettimeofday(&end, NULL);

    waiting_time = (1000000 * (end.tv_sec - store->store_time.tv_sec) + (end.tv_usec - store->store_time.tv_usec)) / (float)1000;
    //_mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "Topic : %s , The waiting time of db_queue: %fms", store->topic, waiting_time);
    _mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "DB_QUEUE , ID : %s, Topic : %s , Waiting Time: %fms", store->id, store->topic, waiting_time);

	strncpy(db_queue_info[db_queue_count].id, store->id, 31);
	strncpy(db_queue_info[db_queue_count].topic, store->topic, 15);
	db_queue_info[db_queue_count].timestamp = waiting_time;
	db_queue_count++;
	if (db_queue_count == MAX_QUEUE_SIZE)
		db_queue_count = 0;
}
void travislu_get_sub_queue_time(struct mosquitto_client_msg *msg)
{
    struct timeval end;
    float waiting_time;

    gettimeofday(&end, NULL);

    waiting_time = (1000000 * (end.tv_sec - msg->store_time.tv_sec) + (end.tv_usec - msg->store_time.tv_usec)) / (float)1000;
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "SUB_QUEUE , ID : %s, Topic : %s , Waiting Time: %fms", msg->id, (msg->store)->topic, waiting_time);

	strncpy(sub_queue_info[sub_queue_count].id, msg->id, 31);
	strncpy(sub_queue_info[sub_queue_count].topic, (msg->store)->topic, 15);
	sub_queue_info[sub_queue_count].timestamp = waiting_time;
	sub_queue_count++;
	if (sub_queue_count == MAX_QUEUE_SIZE)
		sub_queue_count = 0;
}

