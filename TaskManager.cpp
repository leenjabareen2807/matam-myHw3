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
void TaskManager::completeTask(const string& name) {
     int flag=0;
    for(int i=0;i<personsNum &&i<MAX_PERSONS;i++) {

        if( persons[i].getName()==name) {
            flag=1;
            persons[i].completeTask();
        }
    }
    if(flag==0)return;
}

void TaskManager::printAllEmployees() const {
    for (int i = 0; i < personsNum; i++) {
        std::cout << persons[i] << std::endl;
    }
}
void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> filteredTasks;
    for (int i = 0; i < personsNum; i++) {
        const SortedList<Task>& personTasks = persons[i].getTasks();
        for (const Task& task : personTasks) {
            if (task.getType() == type) {
                filteredTasks.insert(task);
            }
        }
    }
    for (const Task& task : filteredTasks) {
        std::cout << task << std::endl;
    }
}

