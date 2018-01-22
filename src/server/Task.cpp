//
// Created by omerz on 17/01/18.
//

#include "Task.h"



Task::Task(void * (*func)(void *arg), void* arg) : func(func), arg(arg) {}

    void Task::execute() {
        func(arg);
    }


