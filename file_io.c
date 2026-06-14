#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "student.h"

int loadCSV(char* filename, Student** head){
    FILE* fp = fopen(filename, "r");
    if(!fp) return -1;

    char line[256];
    int cnt = 0;

    if(fgets(line, sizeof(line), fp)==NULL){
        fclose(fp);
        return -1;
    }

    while(fgets(line, sizeof(line), fp)!=NULL){
        line[strcspn(line,"\n")] = '\0';

        char* sid = strtok(line,",");
        char* name = strtok(NULL, ",");
        char* sscore = strtok(NULL, ",");

        if(sid && name && sscore){
            int id = atoi(sid);
            int score = atoi(sscore);

            add(head, id, name, score);
            cnt++;
        }
    }

    fclose(fp);
    return cnt;
}

int saveCSV(char* filename, Student* head){
    FILE* fp = fopen(filename, "w");
    if(!fp) return -1;

    fprintf(fp, "id,name,score\n");

    Student* p = head;
    while(p != NULL){
        fprintf(fp,"%d,%s,%d\n",p->id,p->name,p->score);
        p = p->next;
    }

    fclose(fp);
    return 0;
}
