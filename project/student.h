//구조체
#pragma once
typedef struct Student
{
    int id;
    char name[32];
    int score;
    struct Student* next;
}Student;

typedef struct{
    int count;
    double average;
    int max;
    int min;
}StatResult;

void add(Student** head, int id, char* name, int score);
void freeList(Student** head);
void printList(Student* head);
int isDuplicate(Student* head, int id);
int updateList(Student* head, int id, int score);
Student* findList(Student* head, int id);
int calculateList(Student* head, StatResult* res);
int deleteList(Student** head, int id);
