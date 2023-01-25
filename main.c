#include <stdio.h>
#include <string.h>

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

int main() {
    char entry[500];
    char *token;
    while(1){
        gets(entry);
        token=strtok(entry , " ");
        switch (Command_code(token)) {
            case Invalid:
                printf("invalid command\n");
                break;

            case Create_File:
                printf("create file\n");
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
