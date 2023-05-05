#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)


/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Creat NULL */
void test_null_create(void)
{
  // Not sure how to make malloc fail
}

/* Test enqueue with NULL input */
void test_enqueue_null(void)
{
  int data = 3;
  queue_t q;

  fprintf(stderr, "*** TEST enqueue_null ***\n");

  q = queue_create();

  /* Enqueuing with NULL queue */
  TEST_ASSERT(-1 == queue_enqueue(NULL, &data));

  /* Enqueuing with NULL data */
  TEST_ASSERT(-1 == queue_enqueue(q, NULL));
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* Test enqueue/dequeue multiple */
void test_queue_multiple(void)
{
  int data1 = 5, data2 = 10, data3 = 15, *ptr;
  queue_t q;

  fprintf(stderr, "*** TEST queue_multiple ***\n");

  q = queue_create();

  /* Enqueue multiple items */
  queue_enqueue(q, &data1);
  queue_enqueue(q, &data2);
  queue_enqueue(q, &data3);

  /* Dequeue and check items */
  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &data1);
  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &data2);
  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &data3);
}

/* Test delete */
void test_queue_delete(void)
{
  int data1 = 5, data2 = 6;
  queue_t q;

  fprintf(stderr, "*** TEST queue_delete ***\n");

  q = queue_create();

  /* Enqueue items */
  queue_enqueue(q, &data1);
  queue_enqueue(q, &data2);
  
  /* Check if data passed in NULL */
  TEST_ASSERT(-1 == queue_delete(q, NULL));

  /* delete items */
  TEST_ASSERT(0 == queue_delete(q, &data1));
  TEST_ASSERT(0 == queue_delete(q, &data2));
  
  /* Check if queue is now empty */
  TEST_ASSERT(-1 == queue_delete(q, &data2));
}

/* Test queue size */
void test_queue_size(void)
{
  int data1 = 5, data2 = 6;
  queue_t q;

  fprintf(stderr, "*** TEST queue_size ***\n");

  q = queue_create();

  /* Enqueue items */
  queue_enqueue(q, &data1);
  queue_enqueue(q, &data2);

  TEST_ASSERT(2 == queue_length(q));

  queue_delete(q, &data1);
  TEST_ASSERT(1 == queue_length(q));

  /* Queue is empty */
  queue_delete(q, &data2);
  TEST_ASSERT(-1 == queue_length(q));
}

/* Callback function that increments items */
static void iterator_inc(queue_t q, void *data)
{
  int *a = (int*)data;
   
  if (*a == 42){
    //printf("%d\n", *a);
    //printf("%d\n", queue_length(q));
    queue_delete(q, data);
  }
  else{
    *a += 1;
  }
}

/* Test queue iterate */
void test_queue_iterate(void)
{
  queue_t q;
  int data[] = {1,2,3,4,5,42,6,7,8,8};
  size_t i;

  fprintf(stderr, "*** TEST queue_iterate ***\n");

  /* Initialize the queue and enqueue items */
  q = queue_create();
  for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
    queue_enqueue(q, &data[i]);

  /* Increment every item of the queue, delete item '42' */
  TEST_ASSERT(queue_length(q) == 10);
  queue_iterate(q, iterator_inc);
  TEST_ASSERT(queue_length(q) == 9);
}


int main(void)
{
	test_create();
  test_enqueue_null();
	test_queue_simple();
  test_queue_multiple();
  test_queue_delete();
  test_queue_size();
  test_queue_iterate();

	return 0;
}
