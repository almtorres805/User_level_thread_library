#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node
{
  void *data;
  struct node *next;
};

struct queue
{
  struct node *front;
  struct node *rear;
  size_t size;
};

queue_t queue_create(void)
{
  queue_t q = malloc(sizeof(*q));
  if (q == NULL){
    return NULL;
  }
  q->front = NULL;
  q->rear = NULL;
  q->size = 0;
  return q;
}

int queue_destroy(queue_t queue)
{
  if (queue == NULL || queue->size > 0) {
    return -1;
  }
  free(queue);
  return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
  if (queue == NULL || data == NULL) {
    return -1;
  }
  
  struct node *n = malloc(sizeof(*n));
  // if failure to allocate a new node
  if (n == NULL) {
    return -1;
  }
  
  queue->size++;
  n->data = data;
  n->next = NULL;
  // Queue is empty
  if (queue->rear == NULL) {
    queue->rear = n;
    queue->front = n;
  } else {
    // Append new node to the rear
    queue->rear->next = n;
    queue->rear = n;
  }
  return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
  if (queue == NULL || data == NULL || queue->front == NULL) {
    return -1;
  }
  
  // Set value of data pointer to address of node to be dequeue'd
  *data = queue->front->data;
  
  // Set front to point to second node in the queue
  queue->front = queue->front->next;
  
  // Decrement size of queue
  queue->size--;

  // Set rear to NULL if queue is empty
  if (queue->front == NULL) {
    queue->rear = NULL;
  }
  return 0;
}

int queue_delete(queue_t queue, void *data)
{
  if (queue == NULL || data == NULL){
    return -1;
  }
  
  struct node *curr_node = malloc(sizeof(*curr_node));
  struct node *prev_node = malloc(sizeof(*prev_node));
  
  curr_node = queue->front;
  prev_node = NULL;

  while(curr_node != NULL){
    // Node was found
    if (curr_node->data == data){
      // Deleting the first node in the queue
      if (prev_node == NULL){
        queue->front = curr_node->next;
        // Update rear if delete removed queue of size 1
        if (queue->front == NULL) {
          queue->rear = NULL;
        }
      } else {
        // Deleting in the middle/end of the queue
        prev_node->next = curr_node->next;
        // Update rear if last item is deleted
        if (prev_node->next == NULL){
          queue->rear = prev_node;
        }
      }
      queue->size--;
      free(curr_node);
      return 0;
    }
    prev_node = curr_node;
    curr_node = curr_node->next;
  }
  // Data was not found
  return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
  if (queue == NULL || func == NULL){
    return -1;
  }

  struct node *n = malloc(sizeof(*n));
  n = queue->front;
  while(n != NULL){
    void *data = n->data;
    n = n->next;
    func(queue, data);
  }
  return 0;
}

int queue_length(queue_t queue)
{
  if (queue == NULL || queue->size == 0){
    return -1;
  }
  return queue->size;
}
