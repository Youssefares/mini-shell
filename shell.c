#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_COMMAND_LENGTH 512
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

char *input_string(FILE*, size_t);


int main(void)
{
  char* s = malloc(sizeof(char)*(MAX_COMMAND_LENGTH+1));
  while(true){
    printf("%s$mini-shell> %s", KGRN, KWHT);
    s = input_string(stdin, MAX_COMMAND_LENGTH+1);
    if(s == NULL){
      fprintf(stderr, "Command too long, use <= %i chars\n", MAX_COMMAND_LENGTH);
      continue;
    }

    if(strcasecmp(s, "exit") == 0){
      printf("Exiting..\n");
      return 0;
    }

    if (strcmp(s, "") == 0) {
      continue;
    }

    int i = 0;
    int size = 2;
    char** args = realloc(NULL, sizeof(char*)*size);
    char* pch = strtok (s," ");
    args[i++] = pch;
    while(pch != NULL)
    {
      pch = strtok (NULL, " ");
      args[i++] = pch;
      if (i == size){
        args = realloc(args, sizeof(char*)*(size*=2));
      }
    }
    args = realloc(args, sizeof(char*)*i--);
    for(int j = 0; j < i; j++){
      printf("%i %s\n", j, args[j]);
    }
  }
  return -1;
}


char *input_string(FILE* fp, size_t max){
    char *str;
    int ch;
    size_t size = 5;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
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