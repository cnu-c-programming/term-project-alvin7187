#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"command.h"
#include"student.h"

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
ShellResult handle_save(char* s, Student** head){return SHELL_OK;}

ShellResult handle_add(char* s, Student** head){
    if(s == NULL || strlen(s) == 0){
        printf("Error: Missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    char* tid = strtok(s, " ");
    char* name = strtok(NULL, " ");
    char* tscore = strtok(NULL, " ");    

    char* nam = strtok(NULL, " ");

    if(tid == NULL || name == NULL || tscore == NULL || nam != NULL){
        printf("Error: Invalid arguments.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    int id = atoi(tid);
    int score = atoi(tscore);

    if(id <= 0){
        printf("Error: Invalid arguments.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if(isDuplicate(*head, id)){
        printf("Error: Duplicate id.\n");
        return SHELL_ERR_DUPLICATE_STUDENT;
    }

    if(score < 0 || score > 100){
        printf("Error: Invalid score.\n");
        return SHELL_ERR_INVALID_SCORE;
    }   
    
    add(head, id, name, score);
    printf("Student added.");
    return SHELL_OK;
}

ShellResult handle_delete(char* s, Student** head){return SHELL_OK;}

ShellResult handle_update(char* s, Student** head){return SHELL_OK;}

ShellResult handle_reload(char* s, Student** head){return SHELL_OK;}

ShellResult handle_find(char* s, Student** head){return SHELL_OK;}

ShellResult handle_list(char* s, Student** head){
    if(*head == NULL){
        printf("No students registered.\n");
        return SHELL_OK;
    }

    printf("%-10s %-20s %-10s\n", "ID", "Name", "Score");
    printList(*head);
    return SHELL_OK;
}

ShellResult handle_stats(char* s, Student** head){return SHELL_OK;}

ShellResult handle_help(char* s, Student** head){return SHELL_OK;}

ShellResult handle_clear(char* s, Student** head){return SHELL_OK;}
