#include "target.h"
#include "../cqueue.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define CQ_MAX_ITEM 10
#define PRINT_CQ(format, ...) printf(format, ##__VA_ARGS__); printf("\n"); fflush(stdout);

void print_all_items(cqueue* tqueue)
{
    PRINT_CQ("Items in queue: ");

    int i = 0;
    for(i = 0; i < tqueue->size(tqueue); i++)
    {
        int res = 0;
        tqueue->at(tqueue, &res,i);
        PRINT_CQ ("[%d] = %d ",i, res);
    }
}

void pop_queue(cqueue* tqueue, int size)
{
    while(size > 0)
    {
        int item = 0;
        cq_byte res_pop = tqueue->pop(tqueue, &item);
        if(!res_pop)
        {
            PRINT_CQ ("<-- pop %d | size = %zu empty = %d",
                     item,tqueue->size(tqueue), tqueue->empty(tqueue));
        }
        size--;
    }
}

void* queue_task1(void* arg)
{
    cqueue* queue = (cqueue*)arg;

    if(!queue)
        return NULL;

    int i = 10;

    while(i >= 0)
    {
        queue->push(queue, &i);
        PRINT_CQ ("--> Thread push: %d | size = %zu empty = %d ",
                 i, queue->size(queue), queue->empty(queue));
        i--;

        sleep(1);
    }

    return NULL;
}

void* queue_task2(void* arg)
{
    cqueue* queue = (cqueue*)arg;

    if(!queue)
        return NULL;

    int i = 10;

    while(i >= 0)
    {
        int item = 0;
        cq_byte res_pop = queue->pop(queue, &item);
        if(!res_pop)
        {
            PRINT_CQ ("<-- Thread pop %d | size = %zu empty = %d",
                     item,queue->size(queue), queue->empty(queue));
        }

        i--;

        sleep(1);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i = 0, a = 0;
    cqueue* tqueue = cqueue_new (sizeof(int), CQ_MAX_ITEM);

    if(tqueue)
    {
        PRINT_CQ ("Address queue: %p | size = %zu capacity=%zu",
                 tqueue, tqueue->size(tqueue), tqueue->capacity(tqueue));
    }
    else
    {
        PRINT_CQ ("Don't create queue: %p", tqueue);
        return 1;
    }

    //------------------------------------------------------------------------------------------

    PRINT_CQ("\nTEST #1");
    for(i = 0; i < CQ_MAX_ITEM; i++)
    {
        a = i+1;
        tqueue->push(tqueue, &a);
        PRINT_CQ ("--> push: %d | size = %zu empty = %d ",
                 a, tqueue->size(tqueue), tqueue->empty(tqueue));
    }

    print_all_items(tqueue);

    pop_queue(tqueue, tqueue->size(tqueue)-3);

    a += 11;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu empty = %d ",
             a, tqueue->size(tqueue), tqueue->empty(tqueue));
    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu empty = %d  ",
             a, tqueue->size(tqueue), tqueue->empty(tqueue));
    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu empty = %d  ",
             a, tqueue->size(tqueue), tqueue->empty(tqueue));

    print_all_items(tqueue);

    pop_queue(tqueue, tqueue->size(tqueue));

    //------------------------------------------------------------------------------------------

    PRINT_CQ("\nTEST #2");
    a = 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu ", a, tqueue->size(tqueue));

    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu ", a, tqueue->size(tqueue));

    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu ", a, tqueue->size(tqueue));

    print_all_items(tqueue);

    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu ", a, tqueue->size(tqueue));

    print_all_items(tqueue);

    pop_queue(tqueue, 1);

    print_all_items(tqueue);

    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu ", a, tqueue->size(tqueue));

    print_all_items(tqueue);

    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu ", a, tqueue->size(tqueue));

    print_all_items(tqueue);

    pop_queue(tqueue, 1);

    print_all_items(tqueue);

    a += 1;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu ", a, tqueue->size(tqueue));

    print_all_items(tqueue);

    //------------------------------------------------------------------------------------------

    PRINT_CQ("\nTEST #3");
    tqueue->clear(tqueue);
    PRINT_CQ("Clear queue: size = %zu empty = %d ",
             tqueue->size(tqueue), tqueue->empty(tqueue));
    print_all_items(tqueue);
    PRINT_CQ(" ");

    a = 100;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu empty = %d",
             a, tqueue->size(tqueue), tqueue->empty(tqueue));
    print_all_items(tqueue);
    PRINT_CQ(" ");

    a = 200;
    tqueue->push(tqueue, &a);
    PRINT_CQ ("--> push: %d | size = %zu empty = %d",
             a, tqueue->size(tqueue), tqueue->empty(tqueue));
    print_all_items(tqueue);
    PRINT_CQ(" ");

    tqueue->free(tqueue);
    //tqueue->empty(tqueue); // crashed
    tqueue = NULL;
    PRINT_CQ("Delete queue: ptr = %p",tqueue);

    //------------------------------------------------------------------------------------------

    PRINT_CQ("\nTEST #4 - Struct");

    typedef struct
    {
        unsigned int x;
        unsigned int y;
        unsigned int rgb;
    } pixel;

    cqueue* squeue = cqueue_new (sizeof(pixel), CQ_MAX_ITEM);

    if(squeue)
    {
        PRINT_CQ ("Address queue: %p | size = %zu capacity=%zu",
                 squeue, squeue->size(squeue), squeue->capacity(squeue));
    }
    else
    {
        PRINT_CQ ("Don't create queue: %p", squeue);
        return 1;
    }

    for(i = 0; i < CQ_MAX_ITEM; i++)
    {
        pixel a = {i+1, i*9+2, 0xfaddaa+i};
        squeue->push(squeue, &a);
        PRINT_CQ ("--> push: 0x%x | size = %zu empty = %d ",
                 a.rgb, squeue->size(squeue), squeue->empty(squeue));
    }

    PRINT_CQ("Items in queue: ");
    for(i = 0; i < squeue->size(squeue); i++)
    {
        pixel ret;
        squeue->at(squeue, &ret,i);
        PRINT_CQ ("[%d] = 0x%x ",i, ret.rgb);
    }

    while(squeue->size(squeue) > 0)
    {
        pixel ret;
        cq_byte res = squeue->pop(squeue, &ret);
        if(!res)
        {
            PRINT_CQ ("<-- pop 0x%x | size = %zu empty = %d",
                     ret.rgb,squeue->size(squeue), squeue->empty(squeue));
        }
    }

    squeue->free(squeue);
    squeue = NULL;

    //------------------------------------------------------------------------------------------

    PRINT_CQ("\nTEST #5 - Threads");
    pthread_t t1;
    pthread_t t2;

    cqueue* queue_t = cqueue_new (sizeof(int), CQ_MAX_ITEM);

    int result = pthread_create (&t1, NULL, queue_task1, queue_t);
    if(result)
    {
        PRINT_CQ ("The thread1 could not be created. Error number: %d ", result);
        exit(1);
    }

    result = pthread_create (&t2, NULL, queue_task2, queue_t);
    if(result)
    {
        PRINT_CQ ("The thread1 could not be created. Error number: %d ", result);
        exit(2);
    }

    result = pthread_join (t1, NULL);
    if(result)
    {
        PRINT_CQ ("The thread1 could not be joined. Error number: %d ", result);
        exit(3);
    }
    result = pthread_join (t2, NULL);
    if(result)
    {
        PRINT_CQ ("The thread2 could not be joined. Error number: %d ", result);
        exit(4);
    }

    return 0;
}
