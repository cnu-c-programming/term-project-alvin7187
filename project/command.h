#pragma once
#include"student.h"

typedef enum {
    SHELL_OK = 0,
    SHELL_EXIT,
    SHELL_ERR_UNKNOWN_COMMAND,
    SHELL_ERR_INVALID_ARGUMENT,
    SHELL_ERR_MISSING_ARGUMENT,
    SHELL_ERR_FILE_OPEN,
    SHELL_ERR_FILE_WRITE,
    SHELL_ERR_STUDENT_NOT_FOUND,
    SHELL_ERR_DUPLICATE_STUDENT,
    SHELL_ERR_INVALID_SCORE
} ShellResult;

typedef ShellResult (*CommandHandler)(char* args, Student** head);

typedef struct {
    const char* name;
    CommandHandler handler;
    const char* usage;
    const char* description;
} Command;

#ifdef ADMIN_MODE
ShellResult handle_save(char* s, Student** head);
ShellResult handle_add(char* s, Student** head);
ShellResult handle_delete(char* s, Student** head);
ShellResult handle_update(char* s, Student** head);
#endif

ShellResult handle_reload(char* s, Student** head);
ShellResult handle_find(char* s, Student** head);
ShellResult handle_list(char* s, Student** head);
ShellResult handle_stats(char* s, Student** head);
ShellResult handle_help(char* s, Student** head);
ShellResult handle_clear(char* s, Student** head);
ShellResult handle_exit(char* s, Student** head);
ShellResult cut_command(char* s, Student** head);