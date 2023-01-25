#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

enum commands {
    Invalid, Create_File, Insert,
    Cat,Remove,Copy,
    Cut,Paste,Find,
    Replace,Grep,Undo,
    Auto_indent,Compare,Directory_tree,
    Exit
};

int Command_code(char *command){
    int code;
    code = Invalid;
    code += Create_File * !strcmp(command,"createfile");
    code += Insert * !strcmp(command,"insertstr");
    code += Cat * !strcmp(command,"cat");
    code += Remove * !strcmp(command,"removestr");
    code += Copy * !strcmp(command,"copystr");
    code += Cut * !strcmp(command,"cutstr");
    code += Paste * !strcmp(command,"pastestr");
    code += Find * !strcmp(command,"find");
    code += Replace * !strcmp(command,"replace");
    code += Grep * !strcmp(command,"grep");
    code += Undo * !strcmp(command,"undo");
    code += Auto_indent * !strcmp(command,"auto-indent");
    code += Compare * !strcmp(command,"compare");
    code += Directory_tree * !strcmp(command,"tree");
    code += Exit * !strcmp(command , "exit");
    return code;
}

char* find_file_address(char *entry){
    char entry_cpy[500];
    char *token;

    strcpy(entry_cpy,entry);
    token = strtok(entry_cpy," ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    printf("%s\n",token);

    if(*token == '"'){
        strcpy(entry_cpy,entry);
        token = strtok(entry_cpy,"\"");
        token = strtok(NULL,"\"");
    }

    return token;
}

void create_file(char *entry){
    char* file_address;
    file_address = (char*) calloc(500,sizeof(char));
    strcpy(file_address,find_file_address(entry));

    printf("%s\n",file_address);

    FILE * file;
    file = fopen(file_address, "r");
    if (file){
        printf("file already exist\n");
        fclose(file);
    }
    else{

    }



}

int main() {
    char entry[500];
    char command[500];
    while(1){
        gets(entry);
        strcpy(command,entry);
        strtok(command , " ");
        switch (Command_code(command)) {
            case Invalid:
                printf("invalid command\n");
                break;

            case Create_File:
                create_file(entry);
                break;

            case Insert:
                printf("insert\n");
                break;

            case Cat:
                printf("cat\n");
                break;

            case Remove:
                printf("remove\n");
                break;

            case Copy:
                printf("copy\n");
                break;

            case Cut:
                printf("cut\n");
                break;

            case Paste:
                printf("paste\n");
                break;

            case Find:
                printf("find\n");
                break;

            case Replace:
                printf("replace\n");
                break;

            case Grep:
                printf("grep\n");
                break;

            case Undo:
                printf("undo\n");
                break;

            case Auto_indent:
                printf("auto indent\n");
                break;

            case Compare:
                printf("compare\n");
                break;

            case Directory_tree:
                printf("tree\n");
                break;

            case Exit:
                printf("program finished\n");
                return 0;

        }
    }

}
