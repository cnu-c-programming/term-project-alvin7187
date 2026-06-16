#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"command.h"
#include"student.h"
#include"file_io.h"

#ifdef ADMIN_MODE
Command commands[] = {
    {"save", handle_save, "save", "Save students to CSV"},
    {"reload", handle_reload, "reload", "Reload students from CSV"},
    {"add", handle_add, "add <id> <name> <score>", "Add a student"},
    {"delete", handle_delete, "delete <id>", "Delete a student"},
    {"update", handle_update, "update <id> <score>", "Update student score"},
    {"find", handle_find, "find <id>", "Find student"},
    {"list", handle_list, "list", "List students"},
    {"stats", handle_stats, "stats", "Show statistics"},
    {"help", handle_help, "help", "Show help"},
    {"clear", handle_clear, "clear", "Clear screen"},
    {"exit", handle_exit, "exit", "Exit shell"}
};
#else
Command commands[] = {
    {"reload", handle_reload, "reload", "Reload students from CSV"},
    {"find", handle_find, "find <id>", "Find student"},
    {"list", handle_list, "list", "List students"},
    {"stats", handle_stats, "stats", "Show statistics"},
    {"help", handle_help, "help", "Show help"},
    {"clear", handle_clear, "clear", "Clear screen"},
    {"exit", handle_exit, "exit", "Exit shell"}
};
#endif

extern char csv_name[256];
int n = sizeof(commands) / sizeof(Command);

ShellResult cut_command(char* input, Student** head){
    input[strcspn(input, "\n")] = '\0';
    char* cmd = strtok(input," ");

    char* para = strtok(NULL,"");

    for(int i = 0; i<n; i++){
        if(strcmp(cmd, commands[i].name) == 0){
            return commands[i].handler(para, head);
        }
    }

    printf("Unknown command or permission denied.\n");
    return SHELL_ERR_UNKNOWN_COMMAND;
}

ShellResult handle_exit(char* para, Student** head){
    printf("Goodbye.\n");
    return SHELL_EXIT;
}

ShellResult handle_save(char* s, Student** head){
    if(s != NULL && strlen(s) > 0){
        char* nam = strtok(s," ");
        if(nam != NULL){
            printf("Error: invalid arguments.");
            return SHELL_ERR_INVALID_ARGUMENT;
        }
    }

    int cnt = saveCSV(csv_name, *head);
    if(cnt >= 0){
        printf("Saved %d students to %s.\n",cnt,csv_name);
        return SHELL_OK;
    }else{
        printf("Error: csv save fail.");
        return SHELL_ERR_FILE_WRITE;
    }
}

ShellResult handle_add(char* s, Student** head){
    if(s == NULL || strlen(s) == 0){
        printf("Error: Missing arguments.");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    char* tid = strtok(s, " ");
    char* name = strtok(NULL, " ");
    char* tscore = strtok(NULL, " ");    

    char* nam = strtok(NULL, " ");

    if(tid == NULL || name == NULL || tscore == NULL || nam != NULL){
        printf("Error: Invalid arguments.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(tid);

    char* ep;
    int score = (int)strtol(tscore, &ep, 10);
    if(*ep != '\0'){
        printf("Error: Invalid score.");
        return SHELL_ERR_INVALID_SCORE;
    }

    if(id <= 0){
        printf("Error: Invalid id.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if(isDuplicate(*head, id)){
        printf("Error: Duplicate id.");
        return SHELL_ERR_DUPLICATE_STUDENT;
    }

    if(score < 0 || score > 100){
        printf("Error: Invalid score.");
        return SHELL_ERR_INVALID_SCORE;
    }   
    
    add(head, id, name, score);
    printf("Student added.\n");
    return SHELL_OK;
}

ShellResult handle_delete(char* s, Student** head){
    if (s == NULL || strlen(s) == 0) {
        printf("Error: Missing arguments.");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    char* tid = strtok(s, " ");
    char* nam = strtok(NULL, " ");

    if (tid == NULL || nam != NULL) {
        printf("Error: Invalid arguments.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(tid);
    if (id <= 0) {
        printf("Error: Invalid id.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (deleteList(head, id)) {
        printf("Student deleted.\n");
        return SHELL_OK;
    } else {
        printf("Error: student not found.");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }
}

ShellResult handle_update(char* s, Student** head){
    if(s == NULL || strlen(s) == 0){
        printf("Error: Missing arguments.");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    char* tid = strtok(s, " ");
    char* tscore = strtok(NULL, " ");    

    char* nam = strtok(NULL, " ");

    if(tid == NULL || tscore == NULL || nam != NULL){
        printf("Error: Invalid arguments.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(tid);

    char* ep;
    int score = (int)strtol(tscore, &ep, 10);
    if(*ep != '\0'){
        printf("Error: Invalid score.");
        return SHELL_ERR_INVALID_SCORE;
    }

    if(id<=0){
        printf("Error: Invalid id.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (score < 0 || score > 100) {
        printf("Error: Invalid score.");
        return SHELL_ERR_INVALID_SCORE;
    }

    if(updateList(*head, id, score)){
        printf("Student updated.\n");
        return SHELL_OK;
    }else{
        printf("Error: student not found.");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }
}

ShellResult handle_reload(char* s, Student** head){
    if (s != NULL && strlen(s) > 0) {
        char* nam = strtok(s, " ");
        if (nam != NULL) {
            printf("Error: Invalid arguments.");
            return SHELL_ERR_INVALID_ARGUMENT;
        }
    }

    freeList(head);
    if (loadCSV(csv_name, head) >= 0) {
        int count = 0;
        Student* curr = *head;
        while (curr != NULL) {
            count++;
            curr = curr->next;
        }
        printf("Reloaded %d students from %s.\n", count, csv_name);
        return SHELL_OK;
    } else {
        printf("Error: csv load fail.");
        return SHELL_ERR_FILE_OPEN;
    }
}

ShellResult handle_find(char* s, Student** head){
    if(s == NULL || strlen(s) == 0){
        printf("Error: Missing arguments.");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    char* tid = strtok(s, " ");
    char* nam = strtok(NULL, " ");

    if (tid == NULL || nam != NULL) {
        printf("Error: Invalid or missing arguments.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(tid);
    if (id <= 0) {
        printf("Error: Invalid id.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    Student* target = findList(*head, id);

    if(target != NULL){
        printf("ID: %d\n", target->id);
        printf("Name: %s\n", target->name);
        printf("Score: %d\n", target->score);
        return SHELL_OK;
    }else{
        printf("Error: student not found.");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }
}

ShellResult handle_list(char* s, Student** head){
    if(*head == NULL){
        printf("No students found");
        return SHELL_OK;
    }

    printf("%-10s %-20s %-10s\n", "ID", "Name", "Score");
    printList(*head);
    return SHELL_OK;
}

ShellResult handle_stats(char* s, Student** head){
    if (s != NULL && strlen(s) > 0) {
        char* nam = strtok(s, " ");
        if (nam != NULL) {
            printf("Error: Invalid arguments.");
            return SHELL_ERR_INVALID_ARGUMENT;
        }
    }

    StatResult res;
    if (calculateList(*head, &res) == 0) {
        printf("No student data available.");
        return SHELL_OK;

    }
    printf("Count: %d\n", res.count);
    printf("Average: %.1f\n", res.average);
    printf("Max: %d\n", res.max);
    printf("Min: %d\n", res.min);

    return SHELL_OK;
}

ShellResult handle_help(char* s, Student** head){
    printf("Commands:\n");
    for(int i = 0; i<sizeof(commands)/sizeof(Command); i++){
        printf("%-30s%s\n",commands[i].usage,commands[i].description);
    }
    return SHELL_OK;
}

ShellResult handle_clear(char* s, Student** head){
    if (s != NULL && strlen(s) > 0) {
        char* nam = strtok(s, " ");
        if (nam != NULL) {
            printf("Error: Invalid arguments.");
            return SHELL_ERR_INVALID_ARGUMENT;
        }
    }

    printf("\033[2J\033[H");
    fflush(stdout); 

    return SHELL_OK;
}
