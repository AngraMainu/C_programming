#include "queue.h"

void delete_queue(queue_t *queue) {
    free(queue->values);
    free(queue);
}

bool is_empty(queue_t *queue) {
    return (queue->num_entries == 0) ? 1 : 0;
}

bool is_full(queue_t *queue) {
    return (queue->num_entries == queue->size) ? 1 : 0;
}

queue_t* create_queue(int capacity) {
   queue_t *queue = (queue_t *)malloc(sizeof(queue_t) * 1);
   if (!queue)
   {
      fprintf(stderr, "Error while allocation!\n");
      free(queue);
      exit(100);
   }

   queue->head = 0;
   queue->tail = 0;
   queue->size = capacity;
   queue->values = malloc(sizeof(int *) * queue->size);
   queue->num_entries = 0;
   return queue;
}

bool push_to_queue(queue_t *queue, void *data){
    if(is_full(queue)) {
        return false;
    }
    if(queue->num_entries == 0) {
       queue->values[queue->tail] = (int *)data;
    } else {
      queue->tail = (queue->tail + 1) % queue->size;
      queue->values[queue->tail] = (int *)data;
    }
   queue->num_entries++;
    return true;
}

void* pop_from_queue(queue_t *queue) {

    if(is_empty(queue)) {
        return NULL;
    }
    int *result;
    result = queue->values[queue->head];

    if (queue->num_entries == 1) {
        queue->num_entries--;
        return result;
    } else {
        queue->head = (queue->head + 1) % queue->size;
        queue->num_entries--;
        return result;
    }
    
    return result; 
}

void* get_from_queue(queue_t *queue, int idx) {

    if(queue->num_entries == 0) return NULL;

    if (idx < 0){
       idx = idx % queue->size;
      idx = queue->size + idx;
    }

    if((idx >= queue->head) && (idx <= queue->tail)) {
        return queue->values[ (queue->head + idx) ];
    } else {
        return NULL; }
}

int get_queue_size(queue_t *queue) {
    return queue->num_entries;
}
