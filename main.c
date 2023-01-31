#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#define STRING_SIZE 500

char parent_directory[STRING_SIZE];

enum commands {
    Invalid, Create_File, Insert,
    Cat,Remove,Copy,
    Cut,Paste,Find,
    Replace,Grep,Undo,
    Auto_indent,Compare,Directory_tree,
    Exit
};

//function prototypes:
int Command_code(char *command);
int terminal();
long long find_word(char *string , char *word);
char *get_file_address(char *string);
char *get_file_path(char *address);
int file_exist(char *address);
void create_path(char *address );
void create_file(char *string);


int main() {
    char *string;
    getcwd(parent_directory,STRING_SIZE);


    return 0;
}

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

int terminal(){
    char entry[STRING_SIZE];
    char command[STRING_SIZE];
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
    return 1;
}

long long find_word(char *string , char *word){
    char temp[strlen(word)];
    long long i ,j , size ;
    size = strlen(string) - strlen(word);
    for(i=0;i<=size;i++){
        for(j=0;j< strlen(word);j++)
            temp[j]=string[i+j];
        if(!strcmp(word,temp))
            return i;
    }
    return -1;
}

char *get_file_address(char *string){
    char c;
    char *address;
    long long index ,i;
    address = (char*) malloc(STRING_SIZE * sizeof(char));
    memset(address,'\0', sizeof(address));
    index = find_word(string , "-file ");
    index += strlen("-file ");

    if(string[index] == '"'){
        c= '"';
        index++;
    }
    else
        c = ' ';

    for(i=0; string[index+i] != c && string[index+i] != '\0'; i++)
        address[i]=string[index+i];

    return address;
}

char *get_file_path(char *address){
    char *path;
    long long i ;
    path = (char*) malloc(STRING_SIZE * sizeof(char));
    memset(path , '\0', sizeof (path));
    strcpy(path , address);
    for( i = strlen(address) ; path [i] != '\\'; i--)
        path[i] = '\0';
    return path;
}

int file_exist(char *address){
    FILE *file;
    file = fopen(address,"r");
    if(file) {
        return 1;
        fclose(file);
    }
    return 0;
}

void create_path(char *address){
    char *path , *token;
    int cd_stat;
    path = get_file_path(address);
    token = strtok(path,"\\");
    while(token != NULL){
        cd_stat = chdir(token);
        if(cd_stat == -1){
            mkdir(token);
            chdir(token);
        }
        token = strtok(NULL,"\\");
    }
    chdir(parent_directory);
    free(path);
    free(token);
}

void create_file(char *string){
    char *address;
    address = get_file_address(string);
    if(file_exist(address)){
        printf("Error:the file already exist!\n");
    }
    else {
        create_path(address);
        FILE *file;
        file = fopen(address, "w");
        fclose(file);
    }
}