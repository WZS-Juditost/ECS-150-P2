#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.c"

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

/* Destory */
void test_destory_not_empty(void)
{
	queue_t q;
	int data = 1;
	q = queue_create();
	queue_enqueue(q, &data);
	fprintf(stderr, "*** TEST destory not empty ***\n");

	TEST_ASSERT(queue_destroy(q) == -1);
}
void test_destory_null(void)
{
	queue_t q = NULL;
	fprintf(stderr, "*** TEST destory null ***\n");

	TEST_ASSERT(queue_destroy(q) == -1);
}

void test_destory_empty(void)
{
	queue_t q;
	q = queue_create();
	fprintf(stderr, "*** TEST destory empty ***\n");

	TEST_ASSERT(queue_destroy(q) == 0);
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

void test_enqueue_null(void)
{
	int *data = NULL;
	queue_t q;

	fprintf(stderr, "*** TEST enqueue_null ***\n");

	TEST_ASSERT(queue_enqueue(q, data) == -1);
}

void test_dequeue_null(void)
{
	int *data;
	queue_t q;

	fprintf(stderr, "*** TEST dequeue_null ***\n");

	TEST_ASSERT(queue_dequeue(q, (void**)&data) == -1);
}

void test_dequeue_empty(void)
{
	int *data;
	queue_t q;

	q = queue_create();

	fprintf(stderr, "*** TEST dequeue_empty ***\n");

	TEST_ASSERT(queue_dequeue(q, (void**)&data) == -1);
}


void test_dequeue(void)
{
	int *data;
	int a = 1;
	queue_t q;

	q = queue_create();
	queue_enqueue(q, &a);

	fprintf(stderr, "*** TEST dequeue ***\n");

	TEST_ASSERT(queue_dequeue(q, (void**)&data) == 0);
}

/* Delete */
// void test_delete(void) {
// 	int a = 1;
// 	void* data = &a;
// 	queue_t q;

// 	q = queue_create();
// 	queue_enqueue(q, &a);

// 	fprintf(stderr, "*** TEST delete ***\n");

// 	TEST_ASSERT(queue_delete(q, data) == 0);
// }

/* Callback function that increments items */
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

	fprintf(stderr, "*** TEST iterator ***\n");

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    assert(data[0] == 2);
    assert(queue_length(q) == 9);
}

int main(void)
{
	test_create();
	test_destory_not_empty();
	test_destory_null();
	test_destory_empty();
	test_queue_simple();
	test_enqueue_null();
	test_dequeue_null();
	test_dequeue_empty();
	test_dequeue();
	// test_delete();
	test_iterator();
	return 0;
}
