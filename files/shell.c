#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10
#define MAX_PATH_LENGTH 255


struct SubCommand
{
    char *line;
    char *argv[MAX_ARGS];
};

struct Command
{
    struct SubCommand sub_commands[MAX_SUB_COMMANDS];
    int num_sub_commands;
    char *stdin_redirect;
    char *stdout_redirect;
    int background;
};

void ReadArgs(char *in, char **argv, int size)
{
    *argv = (char*)malloc(sizeof(char*)*MAX_ARGS);

    int i;
    char *token = strtok(in, " \n");
    for (i=0; i< size-2;i++) {
        if (token != NULL){
            char *ptr = strdup(token);
            argv[i] = ptr;
        }else{
            break;
        }
        token = strtok(NULL, " \n");
    }
    argv[i] = NULL;
}

int isCommandExist(struct Command* command) {
    int i = 0;
    for (;i < command->num_sub_commands;i++){
        struct SubCommand *cmd = &command->sub_commands[i];
        if ( cmd->argv[0] != NULL){
            if ( strcmp(cmd->argv[0], "ls") == 0 || strcmp(cmd->argv[0], "cat") == 0 || strcmp(cmd->argv[0], "wc") == 0  || strcmp(cmd->argv[0], "cd") == 0 || strcmp(cmd->argv[0], "sleep") == 0 ){
                return 0;
            }
            else{
                 printf(" %s: Command not found\n", cmd->argv[0]);
                 return -1;
            }
        }

    }
    return 0;

}

void ReadCommand(char *line, struct Command *command){
    int i = 0;
    char *token = strtok(line, "|");
    for (;i < MAX_SUB_COMMANDS;i++) {
        if (token != NULL) {
            char *ptr = strdup(token);
            command->sub_commands[i].line = ptr;
        } else {
            break;
        }
        token = strtok(NULL, "|");
    }


    int j = 0;
    for (;j<i;j++) {
            ReadArgs(command->sub_commands[j].line, command->sub_commands[j].argv, MAX_ARGS);
    }
    command->num_sub_commands = i;
}

void ReadRedirectsAndBackground(struct Command *command){
    int j = 0;
    int i = MAX_ARGS - 1;
    struct SubCommand *cmd = &command->sub_commands[command->num_sub_commands-1];

    for(; i>=0; i--){
        if (command->sub_commands[command->num_sub_commands-1].argv[i] != NULL){
                if (strcmp(cmd->argv[i], "&") == 0){
                        command->background = 1;
                        j = i;
                        cmd->argv[i] = cmd->argv[i+1];
                        printf("[%d", (int) getpid());
                        printf("]");
                        printf("\n");

                }
                else if (strcmp(command->sub_commands[command->num_sub_commands-1].argv[i], "<") == 0){
                        command->stdin_redirect = malloc(sizeof(command->sub_commands[command->num_sub_commands-1].argv[i+1]));
                        strcpy(command->stdin_redirect, command->sub_commands[command->num_sub_commands-1].argv[i+1]);
                        j = i;
                        cmd->argv[i] = NULL;
                }
                else if (strcmp(command->sub_commands[command->num_sub_commands-1].argv[i], ">") == 0){
                        command->stdout_redirect = malloc(sizeof(command->sub_commands[command->num_sub_commands-1].argv[i+1]));
                        strcpy(command->stdout_redirect, command->sub_commands[command->num_sub_commands-1].argv[i+1]);
                        j = i;
                        cmd->argv[i] = NULL;
                }
        }
}
    if (j != 0){
        while (command->sub_commands[command->num_sub_commands-1].argv[j] != NULL){
                command->sub_commands[command->num_sub_commands-1].argv[j+1] = NULL;
        }
    }
}


int CDPWD(struct Command* command){
    if (command->num_sub_commands == 0){
            return 0;
    }
    int i = command->num_sub_commands;
    for (;i > 0;i--){
        struct SubCommand *cmd = &command->sub_commands[command->num_sub_commands-i];
        if (strcmp(cmd->argv[0], "cd") == 0) {
            struct stat st;
            if (!cmd->argv[1]){
                char *dir = getenv("HOME");
                int w = chdir(dir);
                printf("%s\n", dir);
            }
            else{
                if (strcmp(cmd->argv[1], "~") != 0 && strcmp(cmd->argv[1], "&") != 0){
                    stat(cmd->argv[1],&st);
                    if (S_ISDIR(st.st_mode)){
                        chdir(cmd->argv[1]);
                        printf("%s\n",getcwd(cmd->argv[0], MAX_PATH_LENGTH));
                    }
                    else{
                        printf("Error: cd: No such directory %s\n", cmd->argv[1]);
                        return -1;
                    }
                }
                if(strcmp(cmd->argv[1], "~") == 0){
                    char *dir = getenv("HOME");
                    int w = chdir(dir);
                    printf("%s\n", dir);
                }
                if(strcmp(cmd->argv[1], "&") == 0){
                    char *dir = getenv("HOME");
                    int w = chdir(dir);
                    printf("%s\n", dir);
                }
            }

        }
    }
    return 0;
}

int CAT(struct Command* command){
    if (command->num_sub_commands == 0){
            return 0;
    }
    int i = command->num_sub_commands;
    for (;i > 0;i--){
        struct SubCommand *cmd = &command->sub_commands[command->num_sub_commands-i];
        if (strcmp(cmd->argv[0], "cat") == 0) {
            char *path = cmd->argv[1];
            char buf[4096];
            int fd = open(path,'r');
            int n;
            while((n=read(fd,buf,4096))>0){
                write(STDOUT_FILENO,buf,n);
                printf("\n");
            }
        }
    }
    return 0;

}


int WC(struct Command* command){
    if (command->num_sub_commands == 0){
            return 0;
    }
    int numLine=0;
    int numword=0;
    int numletter=0;
    int i = command->num_sub_commands;
    int k = 0;
    for (;i > 0;i--){
        struct SubCommand *cmd = &command->sub_commands[command->num_sub_commands-i];
        if (strcmp(cmd->argv[0], "wc") == 0) {
            struct stat st;
            if (cmd->argv[1]){
                if (strcmp(cmd->argv[1], "<") == 0){
                    char buf[1024];
                    FILE *fp;

                    if((fp = fopen(cmd->argv[2],"r")) == NULL)
                    {
                        perror("Fail to read");
                        exit (1) ;
                    }
                    while(fgets(buf,1024,fp) != NULL)
                    {
                        int count=0;
                        int length = strlen(buf);
                        for (;k<length;k++)
                        {
                            if(buf[k]!=' ')
                            {
                                count++;
                            while(buf[k]!=' '&&buf[k]!='\0')
                            {
                                k++;
                            }
                        }
                    }
                        numletter+=length;
                        numword+=count;
                        numLine++;
                    }
                    printf("%8d %7d %7d\n",numLine,numword,numletter);
                   }
                }
        }
    }
    return 0;
}

int lsdashl(struct Command* command){
    if (command->num_sub_commands == 0){
            return 0;
    }
    int i = command->num_sub_commands;
    for (;i > 0;i--){
        struct SubCommand *cmd = &command->sub_commands[command->num_sub_commands-i];
        if (strcmp(cmd->argv[0], "ls") == 0) {
            if (strcmp(cmd->argv[1], "-l") == 0){
            pid_t pid;
            pid = fork();
            if (pid < 0) {
             fprintf(stderr, "Fork Failed\n");
             return 1;
            }
            if (pid < 0) {
             fprintf(stderr, "Fork Failed\n");
             return 1;
            }
            else if (pid > 0) {
             wait(NULL);
            }
            else {
                int error = execl("/bin/ls", "ls", "-l", NULL);
             }
            if (cmd->argv[2]){
                if (strcmp(cmd->argv[2], ">") == 0){
                    if (cmd->argv[3]){
                        int f = 0;
                        char* str = cmd->argv[3];
                        int len = strlen(str);
                        int path = 0;
                        for (;f<len-1;f++){
                          if (str[f] == '/') path = 1;
                        }
                        if (path == 1){
                         char *path1 = cmd->argv[3];
                         struct stat stats;
                         stat(path1, &stats);
                         if (stats.st_mode & F_OK){
                            printf("%s invalid/path: Cannot create file", path1);
                            exit(1);
                        }
                         char str[256];
                         char *ch;
                          ch = strtok(path1, "\\");
                          while (ch != NULL) {
                              strcpy(str, ch);
                              printf("%s\n", ch);
                              ch = strtok(NULL, "\\");
                          }
                            }
                        char *cmd = "ls -l";
                        FILE *ptr;
                        char *result = malloc(sizeof(char)*1024);
                        char buf_ps[1024];
                        char ps[1024]={0};
                        strcpy(ps, cmd);
                        if((ptr=popen(ps, "r"))!=NULL)
                        {
                            while(fgets(buf_ps, 1024, ptr)!=NULL)
                            {
                               strcat(result, buf_ps);
                               if(strlen(result)>1024)
                                   break;
                            }
                            pclose(ptr);
                            ptr = NULL;
                        }
                        else
                        {
                            printf("popen %s error\n", ps);
                        }
                        FILE *fptr;
                        fptr = fopen(str, "w");
                        if (fptr == NULL) {
                            printf("Error!");
                            exit(1);
                        }
                        fprintf(fptr, "%s", result);
                        fclose(fptr);
                        return 0;
                     }
                    }
                 }
            }
        }
    }
            return 0;
        }




int main(int argc, char **argv){
    char s[200];
    while(1){
        printf("$ ");
        fgets(s, sizeof(s), stdin);
        struct Command *command = malloc(sizeof(struct Command));
        ReadCommand(s, command);
        CDPWD(command);
        CAT(command);
        lsdashl(command);
        WC(command);
        isCommandExist(command);
        int N = command->num_sub_commands;
        int status;
        int i;
        int n;
        int p;
        ReadRedirectsAndBackground(command);
    }


    return 0;
}

