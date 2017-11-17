#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_COMMAND_LENGTH 3
char *input_string(FILE*, size_t);

int main(void)
{
  char* s = malloc(sizeof(char)*(MAX_COMMAND_LENGTH+1));
  while(true){
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
    printf("%s\n", s);
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