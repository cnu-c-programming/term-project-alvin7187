#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"student.h"
#include"command.h"
#include"file_io.h"

int main(int argc, char* argv[]){
    if (argc < 2) {
#ifdef ADMIN_MODE
        fprintf(stderr, "Usage: ./admin_shell <csv_file> [-f command_file]\n");
#else
        fprintf(stderr, "Usage: ./client_shell <csv_file> [-f command_file]\n");
#endif
        return 1;
    }

    char* csv_name = argv[1];
    Student* head = NULL;

    printf("[%s]\n", 
#ifdef ADMIN_MODE
           "Admin Program"
#else
           "Client Program"
#endif
    );

    int cnt = loadCSV(csv_name, &head);

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
