#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

void add(Student** head, int nid, char* nname, int nscore){
    Student* newNode = (Student*)malloc(sizeof(Student));
    if(!newNode) return;

    newNode->id = nid;
    newNode->score = nscore;
    newNode->next = NULL;
    strncpy(newNode->name,nname,sizeof(newNode->name)-1);
    newNode->name[sizeof(newNode->name) -1 ] = '\0';

    if(*head == NULL){
        *head = newNode;
        return;
    }

    Student* p = *head;
    while(p->next != NULL){
        p = p->next;
    }
    p->next = newNode;
}

void freeList(Student** head){
    Student* p = *head;
    while(p!=NULL){
        Student* n = p->next;
        free(p);
        p = n;
    }
    *head = NULL;
}
