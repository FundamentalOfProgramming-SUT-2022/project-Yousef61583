#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#define STRING_SIZE 500
#define TEXT_SIZE 100000
#define FILES_HANDLER_MAX 20

char parent_directory[STRING_SIZE];
char clipBord_directory[STRING_SIZE];

enum commands {
    Invalid, Create_File, Insert,
    Cat,Remove,Copy,
    Cut,Paste,Find,
    Replace,Grep,Undo,
    Auto_indent,Compare,Directory_tree,
    Exit
};

enum find_modes{
    Simple,At_end,At_start,
    Pattern,At_end_in_sentence , At_start_in_sentence
};

//function prototypes:
int Command_code(char *command);
int terminal();
long long find_pattern(char *string , char *pattern , long long index);
char *address_handler(char *string);
char *get_file_path(char *address);
int file_exist(char *address);
void create_path(char *address );
void create_file(char *string);
void tree_recursion(const int Depth , int depth);
int isFile(const char *address);
void tree(char *string);
int isBlank(char c);
char *str_handler(char *string , char *header);
long long pos_handler(char *string);
void copy_file(FILE *dest, FILE *source);
FILE *make_undo_file(char *file_address);
void insertStr(char *string);
void print_file(char *address);
void cat(char *string);
long long size_handler(char *string);
void removeStr(char *string);
void start_program();
void copyStr(char *string);
void cutStr(char *string);
void pasteStr(char *string);
long long byWord(char *string ,long long index );
char *find_mode(char *string , int *mode);
int Find_condition(int mode,long long i,long long *start_index ,long long *end_index,char *string,char *word,char *temp);
void find(char *string);
void replace(char *string);
char **files_handler(char *string);

int main() {
    // start_program();
    char string[STRING_SIZE];
    gets(string);
    char **files;
    printf("check\n");
    files = files_handler(string);
    printf("check -files\n");
    int index=0;
    while(files[index] != NULL){
        printf("%s \n",files[index]);
        index++;
    }


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
            insertStr(entry);
            break;

        case Cat:
            cat(entry);
            break;

        case Remove:
            removeStr(entry);
            break;

        case Copy:
            copyStr(entry);
            break;

        case Cut:
            cutStr(entry);
            break;

        case Paste:
            pasteStr(entry);
            break;

        case Find:
            find(entry);
            break;

        case Replace:
            replace(entry);
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

long long find_pattern(char *string , char *pattern ,  long long index){
    char temp[strlen(pattern)];
    long long i ,j , size ;
    size = strlen(string) - strlen(pattern);
    for(i= index ; i<=size ;i++){
        for(j=0;j< strlen(pattern); j++)
            temp[j]=string[i+j];
        temp[strlen(pattern)]= '\0';
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
    index = find_pattern(string, "-file " ,0);
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
    for( i = strlen(address) ; path [i] != '\\'; i--) {
        path[i] = '\0';
    }
    return path;
}

int file_exist(char *address){
    FILE *file;
    file = fopen(address,"r");
    if(file) {
        fclose(file);
        return 1;
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

char *str_handler(char *string , char *header){
    char *str;
    long long index ,i,j=0;
    str = (char*) calloc(STRING_SIZE , sizeof(char));
    index = find_pattern(string, header , 0);
    index += strlen(header);

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
                    case '*':
                        str[j] = '*';
                }
            }
            else if(string[i] == '*') {
                str[j] = '^';
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

long long pos_handler(char *string){
    long long lineNo , charNo , line_cnt=1 , char_cnt = 0 ,pos = 0;
    char *pos_str;
    char *address;
    char c = 0;
    long long index ,i;
    pos_str = (char*) calloc(STRING_SIZE , sizeof(char));
    index = find_pattern(string, "-pos ", 0);
    index += strlen("-pos ");

    for(i=0; string[index + i] != ' ' && string[index + i] != '\0'; i++)
        pos_str[i]=string[index + i];


    sscanf(pos_str,"%lld:%lld",&lineNo,&charNo);
    address = address_handler(string);
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
    strtok(name , "\\");
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
    str  = str_handler(string ,"-str ");
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
    fclose(file);
    fclose(undo_file);
}

void print_file(char *address){
    FILE *fp ;
    char c;
    printf("check\n");
    fp = fopen(address,"r");
    c = fgetc(fp);
    printf("check\n");
    while(c!=EOF){
        printf("%c",c);
        c = fgetc(fp);
    }
    printf("check\n");
    fclose(fp);
}

void cat(char *string){
    char *address;
    address = address_handler(string);
    if(!file_exist(address)){
        printf("Error: file does not exist!\n");
        return;
    }
    print_file(address);

}

long long size_handler(char *string){
    long long size =0 , index , i ;
    char *size_str;
    size_str = (char*) calloc(STRING_SIZE, sizeof(char));
    index = find_pattern(string, "-size " , 0);
    index += strlen("-size ");
    for( i = 0 ; string[i+index] != ' ' && string[index + i] != '\0' ; i++)
        size_str[i] = string[i + index];
    sscanf(size_str , "%lld" , &size);
    return size ;
}

void removeStr(char *string){
    char *address;
    long long pos , size , start , end , i=0;
    char fb_header ,c;
    FILE *file, *undo_file;
    address = address_handler(string);
    if(!file_exist(address)){
        printf("Error: file does not exist!\n");
        return;
    }
    pos = pos_handler(string);
    size = size_handler(string);
    fb_header = string [ strlen(string) - 1];
    if(pos == -1){
        printf("Error: invalid position\n");
        return;
    }
    if(size < 1){
        printf("Error: size should be greater than zero\n");
        return;
    }
    if(fb_header != 'b' && fb_header != 'f'){
        printf("Error:wrong -f -b header\n");
        return;
    }
    start = pos - size * (fb_header == 'b');
    start *= (start > 0);
    end = pos + size * (fb_header == 'f') ;
    undo_file = make_undo_file(address);
    file = fopen(address , "w");
    c = fgetc(undo_file);
    while(c != EOF){
        if( i < start || i >= end )
            fputc(c, file);
        c = fgetc(undo_file);
        i++;
    }
    fclose(file);
    fclose(undo_file);
}

void start_program() {
    getcwd(parent_directory, STRING_SIZE);
    strcpy(clipBord_directory, parent_directory);
    strcat(clipBord_directory, "\\clipBord.txt");
    if (!file_exist(clipBord_directory)) {
        FILE *ClipBord;
        ClipBord = fopen(clipBord_directory, "w");
        fclose(ClipBord);
    }
    if (chdir("root") == -1) {
        mkdir("root");
    }
    chdir(parent_directory);
}

void copyStr(char *string){
    char *address;
    long long pos , size , start , end , i=0;
    char fb_header  , c;
    FILE *file , *clipBord;
    address = address_handler(string);
    if(!file_exist(address)){
        printf("Error: file does not exist!\n");
        return;
    }
    pos = pos_handler(string);
    size = size_handler(string);
    fb_header = string [ strlen(string) - 1];
    if(pos == -1){
        printf("Error: invalid position\n");
        return;
    }
    if(size < 1){
        printf("Error: size should be greater than zero\n");
        return;
    }
    if(fb_header != 'b' && fb_header != 'f'){
        printf("Error:wrong -f -b header\n");
        return;
    }
    start = pos - size * (fb_header == 'b');
    start *= (start > 0);
    end = pos + size * (fb_header == 'f') ;
    file = fopen(address , "r");
    clipBord  = fopen( clipBord_directory , "w");
    c = fgetc(file);
    while(c != EOF){
        if( i >= start && i < end )
            fputc(c, clipBord);
        c = fgetc(file);
        i++;
    }
    fclose(file);
    fclose(clipBord);
}

void cutStr(char *string){
    char *address;
    long long pos , size , start , end , i=0 ;
    char fb_header , c ;
    FILE *file, *undo_file , *clipBord;
    address = address_handler(string);
    if(!file_exist(address)){
        printf("Error: file does not exist!\n");
        return;
    }
    pos = pos_handler(string);
    size = size_handler(string);
    fb_header = string [ strlen(string) - 1];
    if(pos == -1){
        printf("Error: invalid position\n");
        return;
    }
    if(size < 1){
        printf("Error: size should be greater than zero\n");
        return;
    }
    if(fb_header != 'b' && fb_header != 'f'){
        printf("Error:wrong -f -b header\n");
        return;
    }
    start = pos - size * (fb_header == 'b');
    start *= (start > 0);
    end = pos + size * (fb_header == 'f') ;
    undo_file = make_undo_file(address);
    file = fopen(address , "r");
    clipBord = fopen(clipBord_directory , "w");
    c = fgetc(file);
    while(c != EOF){
        if( i >= start && i < end )
            fputc(c, clipBord);
        c = fgetc(file);
        i++;
    }
    fclose(file);
    fclose(clipBord);
    file = fopen(address , "w");
    i=0;
    c = fgetc(undo_file);
    while(c != EOF){
        if( i < start || i >= end )
            fputc(c, file);
        c = fgetc(undo_file);
        i++;
    }
    fclose(file);
    fclose( undo_file);
}

void pasteStr(char *string){
    char *address;
    char c;
    long long pos;
    FILE *file;
    FILE *undo_file;
    FILE *clipBord;
    address = address_handler(string);
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
    clipBord = fopen(clipBord_directory , "r");
    file = fopen(address,"w");
    for(long long i = 0 ; i < pos ; i++){
        c = fgetc(undo_file);
        fputc(c , file);
    }
    c = fgetc(clipBord);
    while(c != EOF){
        fputc(c ,file);
        c = fgetc(clipBord);
    }
    c = fgetc(undo_file);
    while(c != EOF){
        fputc(c ,file);
        c = fgetc(undo_file);
    }
    fclose(file);
    fclose(undo_file);
}

long long byWord(char *string ,long long index ){
    long long word_count = 1;
    for( long long i =1  ; i <  index ; i ++)
        word_count += !isBlank(string[i-1])* isBlank(string[i]);
    return word_count ;
}

char *find_mode(char *string , int *mode){
    if(find_pattern(string , "^" ,0) == -1){
        *mode = Simple;
        return NULL;
    }
    if(string[strlen(string)-1] == '^' && string[0]=='^'){
        string[strlen(string)-1] = '\0';
        strrev(string);
        string[strlen(string)-1] = '\0';
        strrev(string);
        *mode = Pattern;
        return NULL;
    }
    if(string[strlen(string)-1] == '^'){
        string[strlen(string)-1] = '\0';
        *mode = At_end ;
        return NULL;
    }
    if(string[0]=='^'){
        strrev(string);
        string[strlen(string)-1] = '\0';
        strrev(string);
        *mode = At_start ;
        return NULL;
    }
    if(find_pattern(string , "^ " , 0) != -1){
        char *temp;
        temp = (char*) calloc(STRING_SIZE , sizeof(char));
        for(long long i = 0 ; i < strlen(string) - (find_pattern(string , "^ " , 0) + 2); i++)
            temp[i] = string [ i + find_pattern(string , "^ " , 0) + 2];
        temp[strlen(string) - (find_pattern(string , "^ " , 0) + 2)] = '\0';
        *mode = At_end_in_sentence ;
        strtok(string , "^ ");
        return temp ;
    }
    if(find_pattern(string ," ^" , 0) != -1){
        char *temp;
        temp = (char*) calloc(STRING_SIZE , sizeof(char));
        for(long long i = 0 ; i < find_pattern(string , " ^" , 0) ; i++)
            temp[i] = string [ i ];
        temp[find_pattern(string , " ^" , 0)] = '\0';
        *mode = At_start_in_sentence ;
        strrev(string);
        strtok(string , " ^");
        strrev(string);
        return temp ;
    }
}

int Find_condition(int mode , long long i , long long *start_index , long long *end_index , char *string , char *word , char *temp ){
    long long middle_index;
    int condition;

    switch (mode){
        case Simple:
            *start_index = i ;
            *end_index  = i + strlen (word) ;
            condition = (isBlank(string[*end_index]) );
            if(*start_index > 0)
                condition *= (isBlank(string[*start_index - 1])) ;

            break;

        case At_end:
            *start_index = i ;
            *end_index  = i + strlen (word) ;
            while(!isBlank(string[*end_index]))
                (*end_index) ++;
            condition = 1 ;
            if(*start_index > 0)
                condition *= isBlank(string[(*start_index) - 1]);
            break;

        case At_start:
            *start_index = i ;
            while(*start_index>0 && !isBlank(string[*start_index - 1]))
                (*start_index) -- ;
            *end_index = i + strlen(word);
            condition = isBlank(string[*end_index]);
            break;

        case Pattern:
            condition = 1 ;
            *start_index = i ;
            *end_index  = i + strlen (word) ;
            while(*start_index>0 && !isBlank(string[*start_index - 1]))
                (*start_index) -- ;
            while(!isBlank(string[*end_index]))
                (*end_index)++;
            break;

        case At_end_in_sentence:
            *start_index = i ;
            middle_index = i + strlen (word);
            while(!isBlank(string[middle_index]))
                middle_index ++;
            *end_index = middle_index + 1 + strlen(temp) ;
            condition = isBlank(string[middle_index]);
            condition *= (find_pattern(string , temp , middle_index) == middle_index + 1);
            condition *= isBlank(string[*end_index]);
            if(*start_index > 0)
                condition *= isBlank(string[*start_index - 1]) ;

            break;

        case At_start_in_sentence:
            middle_index = i ;
            while(middle_index>0 && !isBlank(string[middle_index - 1]))
                middle_index --;
            *start_index = middle_index - strlen(temp) -1 ;
            *end_index = i + strlen(word);
            condition = (*start_index >= 0);
            condition *= isBlank(string [middle_index - 1] );
            condition *= isBlank(string[*end_index]);
            condition *= (find_pattern(string , temp , *start_index) == *start_index);

            break;
    }
    return condition ;
}

void find(char *string){
    int byWord_command  ,at = 1 ,at_command = 0 ,all_command ,count_command ;
    int output_stat=0,mode =0 ;
    long long i , start_index ,end_index=0 , out ;
    char *text ,*str ,*temp ,*address;
    int outPut_condition ,find_condition ,count =0;
    char c;
    FILE *file;

    address = address_handler(string);
    str  = str_handler(string ,"-str ");
    byWord_command = (find_pattern(string,"-byword",0) != -1 );
    all_command = (find_pattern(string , "-all",0) != -1);
    count_command = (find_pattern(string , "-count" , 0) != -1);
    if(find_pattern(string , "-at " ,0 ) != -1){
        at_command = 1;
        long long x ;
        long long j;
        char *at_str;
        at_str = (char*) calloc(100 , sizeof(char));
        x = find_pattern(string , "-at " , 0);
        x += strlen("-at ");
        for( j=0; string[x + j] != ' ' && string[x + j] != '\0'; j++)
            at_str[j] = string[x + j];
        at_str[j] = '\0';
        sscanf(at_str,"%d",&at);
        free(at_str);
    }

    if(!file_exist(address)){
        printf("Error: file does not exist!\n");
        return;
    }
    if( all_command && at_command){
        printf("Error: -at and -all cannot used at the same time\n");
        return;
    }
    if(count_command && (all_command + at_command + byWord_command)){
        printf("Error:you can't use other commands with -count\n");
        return;
    }

    text = (char*) calloc(TEXT_SIZE , sizeof(char));
    file = fopen(address , "r");
    c= fgetc(file);
    i=0;
    while(c != EOF){
        text[i] = c ;
        c = fgetc(file);
        i++;
    }
    fclose(file);

    temp = find_mode(str , &mode);
    i = find_pattern(text , str , end_index);
    while(i != -1){
        find_condition= Find_condition(mode , i , &start_index , &end_index , text , str , temp);
        if(find_condition){
            count ++;
            out = (!byWord_command) * start_index + byWord_command * byWord(text , start_index);
            outPut_condition = (count == at || all_command) * !count_command ;
            if(outPut_condition) {
                if(all_command && output_stat)
                    printf(", ");
                printf("%lld", out);
                output_stat = 1;
            }
        }

        i = find_pattern(text , str , start_index+1);
    }
    if(count_command) {
        printf("%d", count);
        output_stat = 1;
    }
    if(output_stat)
        printf("\n");
    else
        printf("str not found\n");

    free(text);

}

void replace(char *string){
    int at = 1 ,at_command = 0 ,all_command ;
    int replace_stat=0,mode =0 ;
    long long i , start_index ,end_index=0 ,perv_end=-1, text_index =0 ;
    char *text ,*str_1 ,*str_2 ,*temp ,*address;
    int replace_condition ,find_condition ,count =0;
    char c;
    FILE *file;
    FILE *undo_file;

    address = address_handler(string);
    str_1 = str_handler(string , "-str1 ");
    str_2 = str_handler(string , "-str2 ");
    all_command = (find_pattern(string , "-all",0) != -1);
    if(find_pattern(string , "-at " ,0 ) != -1){
        at_command = 1;
        long long x ;
        long long j;
        char *at_str;
        at_str = (char*) calloc(100 , sizeof(char));
        x = find_pattern(string , "-at " , 0);
        x += strlen("-at ");
        for( j=0; string[x + j] != ' ' && string[x + j] != '\0'; j++)
            at_str[j] = string[x + j];
        at_str[j] = '\0';
        sscanf(at_str,"%d",&at);
        free(at_str);
    }



    if(!file_exist(address)){
        printf("Error: file does not exist!\n");
        return;
    }
    if( all_command && at_command){
        printf("Error: -at and -all cannot used at the same time\n");
        return;
    }


    text = (char*) calloc(TEXT_SIZE , sizeof(char));
    file = fopen(address , "r");
    c= fgetc(file);
    i=0;
    while(c != EOF){
        text[i] = c ;
        c = fgetc(file);
        i++;
    }
    fclose(file);
    undo_file = make_undo_file(address);
    file = fopen(address , "w");


    temp = find_mode(str_1 , &mode);
    i = find_pattern(text , str_1 , end_index);
    while(i != -1){
        find_condition= Find_condition(mode , i , &start_index , &end_index , text , str_1 , temp);
        if(find_condition){
            count ++;
            replace_condition = (count == at || all_command) * (start_index >= perv_end)  ;
            if(replace_condition){
                while(text_index < start_index){
                    fputc(text[text_index ], file);
                    text_index++;
                }
                for(long long j  =0 ; j < strlen(str_2) ; j++)
                    fputc(str_2[j],file);
                text_index = end_index ;

            }
            replace_stat += replace_condition;
            perv_end = end_index ;
        }

        i = find_pattern(text , str_1 , start_index + 1);
    }
    while(text_index < strlen(text)){
        fputc(text[text_index ], file);
        text_index++;
    }
    fclose(file);
    fclose(undo_file);
    free(text);

    if(replace_stat)
        printf("replaced!\n");
    else
        printf("str_1 not found\n");


}

char **files_handler(char *string){
    char c;
    char *address;
    char **files;
    long long index ,i;
    int file_count=0;


    files = (char**) calloc(STRING_SIZE , sizeof(char*));
    index = find_pattern(string, "-files " ,0);
    index += strlen("-files ");

    printf("check fh\n");
    while(string[index] != '\0' && string[index] != '-') {
        address = (char*) calloc(STRING_SIZE , sizeof(char));
        if (string[index] == '"') {
            c = '"';
            index++;
        } else
            c = ' ';

        i = index;
        while (string[index] != c && string[index] != '\0') {
            address[index - i] = string[index];
            index++;
        }
        index += 1 + (c == '"') ;

        files[file_count] =address  ;
        printf("adrs :%s\n",address);
        file_count ++;
        free(address);
    }

    return files;


}