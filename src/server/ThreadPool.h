//
// Created by omerz on 18/01/18.
//

#ifndef REVERSI_THREADPOOL_H
#define REVERSI_THREADPOOL_H

#include <iostream>
#include <zconf.h>
#include <queue>
#include "Task.h"
#include <pthread.h>
using namespace std;

class ThreadPool {

private:
    queue<Task *> tasksQueue;
    pthread_t* threads;
    void executeTasks();
    bool stopped;
    pthread_mutex_t lock;
    static void *execute(void *arg);

public:
    /**
     * Constructor.
     * @param threadsNum number of threads to generate.
     */
    ThreadPool(int threadsNum);

    /**
    Adds a given task to the threadpool.

     @param task task by ref
     */
    void addTask(Task *task);

    /**
     * Terminates the threadpool.
     */
    void terminate();

    /**
     * Deconstructor.
     */
    virtual ~ThreadPool();


};


#endif //REVERSI_THREADPOOL_H
