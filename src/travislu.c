#include<mosquitto_broker.h>
#include<memory_mosq.h>


extern unsigned long g_msgs_dropped;

void travislu_print_all_info(struct mosquitto_db *db)
{
    travislu_get_cur_memory();
    travislu_get_max_memory();
    travislu_get_msg_store_count(db);
    travislu_get_msgs_dropped();
    travislu_get_client(db);
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "\n");
}

void travislu_whereis(char *str)
{
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "Now at %s", str);
}

void travislu_printf(char *str)
{
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "The msg is %s", str);
}

void travislu_print_mos(struct mosquitto *mosq)
{

}

void travislu_get_cur_memory()
{
    unsigned value;
    value = _mosquitto_memory_used();
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "The curr memory : %d", value);
}

void travislu_get_max_memory()
{
    unsigned value;
    value = _mosquitto_max_memory_used();
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "The curr memory : %d", value);
}

void travislu_get_msg_store_count(struct mosquitto_db *db)
{
    int msg_store_count;
    msg_store_count = db->msg_store_count;
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "The msg count : %d", msg_store_count);
}

void travislu_get_msgs_dropped()
{
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "The msg dropped count : %ld", g_msgs_dropped);
}

void travislu_get_client(struct mosquitto_db *db)
{
    unsigned int count_by_sock;

    if (db->contexts_by_sock)
        count_by_sock = db->contexts_by_sock->hh_sock.tbl->num_items;
    else
        count_by_sock = 0;
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "The client : %d", count_by_sock);
}

void travislu_get_waiting_queue_time(struct mosquitto_msg_store *store)
{
    struct timeval end;
    float waiting_time;

    gettimeofday(&end, NULL);

    waiting_time = (1000000 * (end.tv_sec - store->store_time.tv_sec) + (end.tv_usec - store->store_time.tv_usec)) / (float)1000;
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "Topic : %s , The waiting time of db_queue: %fms", store->topic, waiting_time);
}
void travislu_get_sub_queue_time(struct mosquitto_client_msg *msg)
{
    struct timeval end;
    float waiting_time;

    gettimeofday(&end, NULL);

    waiting_time = (1000000 * (end.tv_sec - msg->store_time.tv_sec) + (end.tv_usec - msg->store_time.tv_usec)) / (float)1000;
    _mosquitto_log_printf(NULL, MOSQ_LOG_INFO, "Topic : %s , The waiting time of sub: %fms", (msg->store)->topic, waiting_time);

}
