#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#define STRING_SIZE 500


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

void find_file_address(char* _entry , char* _file_address){
    auto char entry_cpy[STRING_SIZE];
    auto char *token;

    strcpy(entry_cpy, _entry);
    token = (entry_cpy," "); //this token is the command.
    token = strtok(NULL, " "); //this token is "-file" header.
    token = strtok(NULL, " "); // this token is the file address.

    if(*token == '"'){
        strcpy(entry_cpy, _entry);
        token = strtok(entry_cpy,"\"");
        token = strtok(NULL,"\"");
    }

    strcpy(_file_address, token) ;
}

void find_file_path(char* _file_address , char* _file_path){
    auto int i;
    for(i = strlen(_file_address) - 1; _file_address[i] != '\\'; i--);
    strncpy(_file_path, _file_address, i);
}

void find_file_name(char* _file_address , char* _file_name){
    auto char address[STRING_SIZE];
    strcpy(address , strrev(_file_address));
    strtok(address , "\\");
    strcpy(_file_name , strrev(address));
}

int check_file_existence(char* _file_address){
    FILE * file;
    file = fopen(_file_address, "r");
    if (file){
        fclose(file);
        return 1;
    }
    else
        return 0;

}

void create_path(char* _path,char* _name){
    char path_copy[STRING_SIZE];
    char* token;
    int cd_stat;
    strcpy(_path, _path);
    token = strtok(path_copy,"\\");
    while(token != NULL){
        cd_stat = chdir(token);
        if(cd_stat == -1){
            mkdir(token);
            chdir(token);
        }
        token = strtok(NULL , "\\");
    }
    FILE * file;
    file = fopen(_name, "w");
    fclose(file);
    chdir("C:");
    getcwd(path_copy,STRING_SIZE);
    printf("%s\n",path_copy);

}

void create_file(char *_entry){
    char file_address[STRING_SIZE];
    char file_path[STRING_SIZE];
    char file_name[STRING_SIZE];
    find_file_address(_entry, file_address);
    find_file_path(file_address , file_path);
    find_file_name(file_address , file_name);

    if(check_file_existence(file_address))
        printf("file already exist\n");
    else{
        create_path(file_path,file_name);

    }

}

void insert(char *_entry){
    char file_address[STRING_SIZE];
    char string[STRING_SIZE];
    find_file_address(_entry , file_address);
    if(check_file_existence(file_address)){

    }
    else
        printf("ERROR:file do not exist!\n");
}

int main() {
    char entry[STRING_SIZE];
    char command[STRING_SIZE];
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
