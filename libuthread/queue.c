#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct q_node {
	void* data;
	struct q_node* next;		
}q_node;

struct q_node* newNode (void* d){
	q_node* newNode = (q_node*)malloc(sizeof(q_node));
	newNode->data = d;
	newNode->next = NULL;
	return newNode;
}

struct queue {
	q_node *front, *rear;
	int size;
};

queue_t queue_create(void)
{
	queue_t q = (struct queue*)malloc(sizeof(struct queue));
	if (q == NULL){
		return NULL;
	}
	q->front = q->rear = NULL;
	q->size = 0;
	return q;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->size >0){
		return -1;
	}
	else {
		queue = NULL;
		free(queue);
		return 0;
	}
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL||data == NULL) {
		return -1;
	}
	q_node* temp = newNode(data);
	if (queue->rear == NULL) {
		queue->front = temp;
	}
	else {
		queue->rear->next = temp;
	}
	queue->rear = temp;
	queue->size++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL||data == NULL||queue->size == 0) {
		return -1;
	}
	*data = queue->front->data;
	q_node* remove = queue->front;
	if (queue->size == 1){
		queue->front = queue->rear = NULL;
	}
	else {
		queue->front = queue->front->next;
	}
	free(remove);
	queue->size--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (queue == NULL||data == NULL) {
		return -1;
	}
	q_node *curr = queue->front;
	if(curr->data == data) {
		q_node *remove = curr;
		queue->front = queue->front->next;
		free(remove);
		queue->size--;
		return 0;
	}

	while(curr->next!=NULL){
		if(curr->next->data == data){
			q_node *remove = curr->next;
			curr->next = curr->next->next;
			free(remove);
			queue->size--;
			return 0;
		}
		else {
			curr = curr->next;
		}
	}
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if (queue == NULL||func == NULL) {
		return -1;
	}
	q_node *itr = queue->front;
	q_node *itr_copy = NULL;
	while(itr!=NULL){
		itr_copy = itr->next;
		func(queue, itr->data);
		itr = NULL;
		itr = itr_copy;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL) {
		return -1;
	}
	else {
		return queue->size;
	}
}