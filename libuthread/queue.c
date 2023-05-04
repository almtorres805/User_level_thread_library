#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node {
  void *data;
  struct node *next;
};

struct queue {
  /* TODO Phase 1 */
  struct node *front;
  struct node *rear;
};

queue_t queue_create(void)
{
  /* TODO Phase 1 */
  queue_t q = malloc(sizeof(*q));
  if (q == NULL){
    return NULL;
  }
  q->front = NULL;
  q->rear = NULL;
  return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
  if (queue == NULL | data == NULL) {
    return -1;
  }
  
  struct node *n = malloc(sizeof(*n));
  // if failure to allocate a new node
  if (n == NULL) {
    return -1;
  }

  n->data = data;
  n->next = NULL;
  if (queue->rear == NULL) {
    queue->rear = n;
    queue->front = n;
  } else {
    queue->rear->next = n;
    queue->rear = n;
  }
  return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
  if (queue == NULL | data == NULL | queue->front == NULL) {
    return -1;
  }
  
  *data = queue->front->data;

  struct node *tmp = queue->front;
  queue->front = queue->front->next;
  free(tmp);

  if (queue->front == NULL) {
    queue->rear = NULL;
  }
  return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}
