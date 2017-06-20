#include<mosquitto_broker.h>
#include<memory_mosq.h>
#include<errno.h>
#include<time.h>

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
	fp = fopen("/home/travis/mosquitto/expi/123.txt", "w");
	if (fp == NULL) {
		_mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "Write file failed:%s ", strerror(errno));
		exit(1);
	}
	
	for (int i=0; i<=count; i++) {
		fprintf(fp, "%d %lu\n", (int)*(time_info+i), *(mem_info+i));	
	}
	fclose(fp);
}

void travislu_get_waiting_queue_time(struct mosquitto_msg_store *store)
{
    struct timeval end;
    float waiting_time;

    gettimeofday(&end, NULL);

    waiting_time = (1000000 * (end.tv_sec - store->store_time.tv_sec) + (end.tv_usec - store->store_time.tv_usec)) / (float)1000;
    //_mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "Topic : %s , The waiting time of db_queue: %fms", store->topic, waiting_time);
    _mosquitto_log_printf(NULL, MOSQ_LOG_NOTICE, "DB_QUEUE , ID : %s, Topic : %s , Waiting Time: %fms", store->id, store->topic, waiting_time);

}
void travislu_get_sub_queue_time(struct mosquitto_client_msg *msg)
{
    struct timeval end;
    float waiting_time;

    gettimeofday(&end, NULL);

    waiting_time = (1000000 * (end.tv_sec - msg->store_time.tv_sec) + (end.tv_usec - msg->store_time.tv_usec)) / (float)1000;
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "SUB_QUEUE , ID : %s, Topic : %s , Waiting Time: %fms", msg->id, (msg->store)->topic, waiting_time);
}

