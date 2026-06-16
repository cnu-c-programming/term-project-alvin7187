#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"student.h"
#include"command.h"
#include"file_io.h"

char csv_name[256] = {0};

int main(int argc, char* argv[]){
    if (argc < 2 || (argc == 3) || (argc == 4 && strcmp(argv[2], "-f") != 0) || argc > 4) {
#ifdef ADMIN_MODE
        fprintf(stderr, "Usage: ./admin_shell <csv_file> [-f command_file]\n");
#else
        fprintf(stderr, "Usage: ./client_shell <csv_file> [-f command_file]\n");
#endif
        return -1;
    }

    strncpy(csv_name, argv[1], sizeof(csv_name) - 1);

    FILE* commandFile = stdin;
    int iscommandFile = 0;

    if (argc == 4 && strcmp(argv[2], "-f") == 0) {
        commandFile = fopen(argv[3], "r");
        if (commandFile == NULL) {
            printf("Error: Cannot open command file %s\n", argv[3]);
            return -1;
        }
        iscommandFile = 1;
    }

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

    printf("Loaded %d students from %s\n\n",cnt,csv_name);


    char input[256];
    int cnt1 = 0;

    while(1){
        if(!iscommandFile){
#ifdef ADMIN_MODE
        printf("admin> ");
#else
        printf("client> ");
#endif

        fflush(stdout);
        }

        if(fgets(input, sizeof(input), commandFile) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        int blank = 1;
        for(int i = 0; input[i] != '\0'; i++){
            if(input[i] != ' ' && input[i] != '\t'){ blank = 0; break; }
        }
        if(blank || input[0] == '#') continue;

        cnt1++;

        if(iscommandFile){
            printf("[command file:%d] %s\n", cnt1, input);
        }

        ShellResult result = cut_command(input, &head);

        printf("\n");

        if(iscommandFile && result != SHELL_OK && result != SHELL_EXIT)
            printf("Skipped line %d\n\n", cnt1);

        if(result == SHELL_EXIT)
            break;
    }

    if (iscommandFile && commandFile != NULL) {
        fclose(commandFile);
    }

    freeList(&head);
    return 0;
}
