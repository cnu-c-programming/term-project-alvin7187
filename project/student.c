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

void printList(Student* head){
    Student* p = head;
    while(p != NULL){
        printf("%-10d %-20s %-10d\n",p->id, p->name, p->score);
        p = p->next;
    }
}

int isDuplicate(Student* head, int nid){
    Student* p = head;
    while(p!=NULL){
        if(p->id == nid){
            return 1;
        }
        p = p->next;
    }
    return 0;
}