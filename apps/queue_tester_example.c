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
  queue_dequeue(q, (void*)&ptr);
  TEST_ASSERT(ptr == &data1);
  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &data2);
  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &data3);
}

int main(void)
{
	test_create();
  test_enqueue_null();
	test_queue_simple();
  test_queue_multiple();

	return 0;
}
