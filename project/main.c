#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"student.h"
#include"command.h"
#include"file_io.h"

char csv_name[256] = {0};

int main(int argc, char* argv[]){
    if (argc < 2) {
#ifdef ADMIN_MODE
        fprintf(stderr, "Usage: ./admin_shell <csv_file> [-f command_file]\n");
#else
        fprintf(stderr, "Usage: ./client_shell <csv_file> [-f command_file]\n");
#endif
        return 1;
    }

    strncpy(csv_name, argv[1], sizeof(csv_name) - 1);
    Student* head = NULL;

    int cnt = loadCSV(csv_name, &head);

    if (cnt < 0) {
        printf("Error: csv open fail.\n");
    }

    printf("[%s]\n", 
#ifdef ADMIN_MODE
           "Admin Program"
#else
           "Client Program"
#endif
    );

    

    printf("Loaded %d students from %s\n",cnt,csv_name);


    char input[256];
    while(1){
#ifdef ADMIN_MODE
        printf("admin> ");
#else
        printf("client> ");
#endif

        fflush(stdout);

        if(fgets(input, sizeof(input), stdin) == NULL){
            printf("\n");
            break;
        }

        ShellResult result = cut_command(input, &head);
        
        if(result == SHELL_EXIT)
            break;
    }
}