//구조체
#pragma once
typedef struct Student
{
    int id;
    char name[32];
    int score;
    struct Student* next;
}Student;


void add(Student** head, int id, char* name, int score);
void freeList(Student** head);
void printList(Student* head);
int isDuplicate(Student* head, int id);