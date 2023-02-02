#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#define STRING_SIZE 500
#define TEXT_SIZE 100000

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
long long find_pattern(char *string , char *pattern);
char *address_handler(char *string);
char *get_file_path(char *address);
int file_exist(char *address);
void create_path(char *address );
void create_file(char *string);
void tree_recursion(const int Depth , int depth);
int isFile(const char *address);
void tree(char *string);
int isBlank(char c);
char *str_handler(char *string);
long long pos_handler(char *__string);
void copy_file(FILE *dest, FILE *source);
FILE *make_undo_file(char *file_address);
void insertStr(char *string);


int main() {
    getcwd(parent_directory,STRING_SIZE);
    char string[STRING_SIZE];
    gets(string);
    char *address;
    address = address_handler(string);
    printf("%s\n",address);
    printf("%lld",pos_handler(string));
     insertStr(string);




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
            tree(entry);
            break;

        case Exit:
            printf("program finished\n");
            return 0;

    }
    return 1;
}

long long find_pattern(char *string , char *pattern){
    char temp[strlen(pattern)];
    long long i ,j , size ;
    size = strlen(string) - strlen(pattern);
    for(i=0;i<=size;i++){
        for(j=0;j< strlen(pattern); j++)
            temp[j]=string[i+j];
        if(!strcmp(pattern, temp))
            return i;
    }
    return -1;
}

char *address_handler(char *string){
    char c;
    char *address;
    long long index ,i;
    address = (char*) calloc(STRING_SIZE , sizeof(char));
    index = find_pattern(string, "-file ");
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
    path = (char*) calloc(STRING_SIZE , sizeof(char));
    strcpy(path , address);
    printf("path is:%s\n",path);
    for( i = strlen(address) ; path [i] != '\\'; i--) {
        path[i] = '\0';
    }
    printf("path :%s \n",path);
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
    address = address_handler(string);
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

int isFile(const char *address){
    struct stat ps;
    stat(address, &ps);
    return S_ISREG(ps.st_mode);
}

void tree_recursion(const int Depth , int depth){
    struct dirent *de;
    DIR *dr = opendir(".");
    if (dr == NULL || depth ==0){
        return;
    }

    while ((de = readdir(dr)) != NULL) {
        if (de->d_name[0] != '.') {
            for (int i = 0; i < Depth - depth; i++)
                printf("\t");
            printf("|__%s\n", de->d_name);
        }
        if (!isFile(de->d_name)) {
            chdir(de->d_name);
            tree_recursion(Depth, depth - 1);
            chdir("..");
        }
    }

    closedir(dr);
}

void tree(char *string){
    int depth;
    sscanf(string,"tree %d",&depth);
    chdir("root");
    printf("root\n");
    tree_recursion(depth, depth);
    chdir(parent_directory);
}

int isBlank(char c){
    int result = 0;
    result += (c == ' ');
    result += (c == '\n');
    result += (c == '\0');
    return result;
}

char *str_handler(char *string){
    char *str;
    long long index ,i,j=0;
    str = (char*) calloc(STRING_SIZE , sizeof(char));
    index = find_pattern(string, "-str ");
    index += strlen("-str ");

    if(string[index] == '"'){
        index++;
        for(i=index; string[i] != '"' && string[i-1] != '\\'; i++) {
            if(string[i] == '\\'){
                i++;
                switch (string[i]){
                    case '\\':
                        str[j] = '\\';
                        break;
                    case '"':
                        str[j] = '"';
                        break;
                    case 'n':
                        str[j] = '\n';
                        break;
                    case 't':
                        str[j] = '\t';
                        break;
                }
            }
            else
                str[j] = string[i];
            j++;
        }
    }
    else
        for(i=0; string[index+i] != ' ' && string[index+i] != '\0'; i++)
            str[i]=string[index + i];


    return str;
}

long long pos_handler(char *__string){
    long long lineNo , charNo , line_cnt=1 , char_cnt = 0 ,pos = 0;
    char *pos_str;
    char *address;
    char c = 0;
    long long index ,i;
    pos_str = (char*) calloc(STRING_SIZE , sizeof(char));
    index = find_pattern(__string, "-pos ");
    index += strlen("-pos ");

    for(i=0; __string[index + i] != ' ' && __string[index + i] != '\0'; i++)
        pos_str[i]=__string[index + i];


    sscanf(pos_str,"%lld:%lld",&lineNo,&charNo);
    address = address_handler(__string);
    FILE *file = fopen(address,"r");
    while(c != EOF){
        c = fgetc(file) ;
        if (lineNo == line_cnt && charNo == char_cnt){
            fclose(file);
            return pos ;
        }
        pos ++;
        char_cnt ++ ;
        if(c == '\n'){
            char_cnt = 0 ;
            line_cnt ++ ;
        }
    }
    fclose(file);
    return -1;
}

void copy_file(FILE *dest, FILE *source){
    char c;
    while ((c = fgetc(source)) != EOF)
        fputc(c, dest);
}

FILE *make_undo_file(char *file_address){
    char *name;
    char *undo_file_path;
    char address[STRING_SIZE];
    FILE *undo_file;
    FILE *file;
    strcpy(address , file_address);
    name = strrev(address);
    strtok(name , "\\");printf("check\n");
    name =strrev(name) ;
    undo_file_path = get_file_path(file_address);
    strcat( undo_file_path , ".undo_");
    strcat( undo_file_path , name);
    undo_file = fopen(undo_file_path , "w");
    file = fopen (file_address , "r");
    copy_file(undo_file , file);
    fclose(file);
    fclose(undo_file);
    undo_file = fopen(undo_file_path , "r");
    return undo_file;
}

void insertStr(char *string){
    char *str;
    char *address;
    char c;
    long long pos;
    FILE *file;
    FILE *undo_file;
    address = address_handler(string);
    str  = str_handler(string);
    pos = pos_handler(string);
    if(!file_exist(address)){
        printf("Error: file does not exist!\n");
        return;
    }

    if(pos == -1){
        printf("Error: invalid position\n");
        return;
    }
    undo_file = make_undo_file(address);
    file = fopen(address,"w");
    for(long long i = 0 ; i < pos ; i++){
        c = fgetc(undo_file);
        fputc(c , file);
    }

    for(long long i = 0 ; i < strlen ( str ) ; i ++)
        fputc(str[i], file);

    c = fgetc(undo_file);
    while(c != EOF){
        fputc(c ,file);
        c = fgetc(undo_file);
    }
}
