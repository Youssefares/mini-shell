#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 512
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

char* input_string(FILE*, size_t);
char** array_of_tokens(char*, char*, bool*); 


int main(void)
{
  char* s = malloc(sizeof(char)*(MAX_COMMAND_LENGTH+1));
  pid_t pid;
  while(true){
    printf("%s$mini-shell> %s", KGRN, KWHT);
    s = input_string(stdin, MAX_COMMAND_LENGTH+1);
    if(s == NULL){
      fprintf(stderr, "Error: Command too long, use <= %i chars\n", MAX_COMMAND_LENGTH);
      continue;
    }


    if(strcasecmp(s, "exit") == 0){
      printf("Exiting..\n");
      return 0;
    }

    if (strcmp(s, "") == 0) {
      continue;
    }

    bool background = false;
    char** args = array_of_tokens(s, " ", &background);
    if (args == NULL){
      continue;
    }
    if (strcmp(args[0], "cd") == 0){
      if(args[1] == NULL || args[2] != NULL){
        fprintf(stderr, "Error: wrong number of arguments\n");
        continue;
      }
      if(chdir(args[1]) == -1){
        fprintf(stderr, "Error: %s\n", strerror(errno));
      }
      continue;
    }

    pid = fork();
    if(pid == 0){
      if(execvp(args[0], args)){
        perror(strerror(errno));
      }
      return 0;
    } else {
      if(!background){
        waitpid(pid, NULL, 0);
      }
      continue;
    }
  }
  return -1;
}


char *input_string(FILE* fp, size_t max){
    char *str;
    int ch;
    size_t size = 5;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);
    bool c_inputted = false;
    if(!str)return str;

    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
      if (!c_inputted && (ch == ' ' || ch == '\n')){
        continue;
      }
      if (ch != ' '){
        c_inputted = true;
      }
      if(len == max){
        continue;
      }
      str[len++]=ch;
      if(len==size){
          str = realloc(str, sizeof(char)*(size+=16));
          if(!str)return str;
      }
    }
    if(len >= max){
      return NULL;
    }
    str[len++]='\0';
    return realloc(str, sizeof(char)*len);
}


char** array_of_tokens(char* s, char* terminators, bool* background){
    int i = 0;
    int size = 2;
    char** args = realloc(NULL, sizeof(char*)*size);
    char* pch = strtok (s, terminators);
    args[i++] = pch;
    while(pch != NULL)
    {
      pch = strtok (NULL, terminators);
      args[i++] = pch;
      if (i == size){
        args = realloc(args, sizeof(char*)*(size*=2));
      }
    }

    if(strcmp(args[i-2], "&") == 0){
      *background = true;
      args[i-2] = "";
    }
    return realloc(args, sizeof(char*)*i);
}