//
// Created by leenn on 6/11/2025.
//
#include "TaskManager.h"

TaskManager::TaskManager() {
    TaskId=0;
    personsNum=0;
}
void TaskManager::assignTask(const string& name, const Task& task) {
    int personIndex = -1;
    for (int i = 0; i < personsNum && i < MAX_PERSONS; i++) {
        if (persons[i].getName() == name) {
            personIndex = i;
            break;
        }
    }
    if (personIndex == -1) {
        if (personsNum >= MAX_PERSONS) {
            throw std::runtime_error("TaskManager is FULL");
        }
        Person newPerson(name);
        persons[personsNum] = newPerson;
        personIndex = personsNum;
        personsNum++;
    }
    Task newTask(task);
    newTask.setId(TaskId++);
    persons[personIndex].assignTask(newTask);
}


