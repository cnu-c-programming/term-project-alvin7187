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

int updateList(Student* head, int id, int score){
    Student* p = head;

    while(p != NULL){
        if(p->id == id){
            p->id = id;
            p->score = score;
            return 1;
        }
        p = p -> next;
    }
    return 0;
}

Student* findList(Student* head, int id){
    Student* p = head;
    while(p != NULL){
        if(p->id == id)
            return p;
        p = p->next;
    }

    return NULL;
}



int calculateList(Student* head, StatResult* res){
    if(head == NULL)
        return 0;

    res->count = 0;
    int sum = 0;
    res->max = -1;
    res->min = 101;

    Student* curr = head;
    while (curr != NULL) {
        res->count++;
        sum += curr->score;

        if (curr->score > res->max) {
            res->max = curr->score;
        }
        if (curr->score < res->min) {
            res->min = curr->score;
        }

        curr = curr->next;
    }

    res->average = (double)sum / res->count;
    return 1;
}

int deleteList(Student** head, int id){
    if(*head == NULL){
        return 0;
    }

    Student* now = *head;
    Student* prev = NULL;

    if (now != NULL && now->id == id) {
        *head = now->next;
        free(now);
        return 1;
    }

    while (now != NULL && now->id != id) {
        prev = now;
        now = now->next;
    }

    if (now == NULL) {
        return 0;
    }

    
    prev->next = now->next;
    free(now);
    return 1;
}
