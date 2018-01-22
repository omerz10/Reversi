//
// Created by omerz on 17/01/18.
//

#ifndef REVERSI_TASK_H
#define REVERSI_TASK_H


class Task {

private:
    void *(*func)(void *arg);
    void *arg;

public:
    Task(void * (*func)(void *arg), void* arg);
    virtual ~Task() {}
    void execute();

};


#endif //REVERSI_TASK_H
