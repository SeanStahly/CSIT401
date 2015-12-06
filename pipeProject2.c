#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>    //found on stack overflow

void pointerPrint(char ** argv);
void runShell(char ** argv);
char * findPathName(char ** argv);
int file_exist (char *filename);
int files_exist (char **filenames, int pipes);
char ** parse(char * c, char b);
char * findPath(char ** envp);

int main(int argc, char ** argv, char ** envp) {
  char *fullpath = findPath(envp);
  //get the path variables
  char ** path = parse(fullpath, ':');
  //    pointerPrint(path);

  //Run shell
  runShell(path);

  return 0;
}

/**
* Find the path from environment
*/
char * findPath(char ** envp) {
  int r = 0;
  //search environmental variables
  for (int i = 0; envp[i] != NULL ; ++i) {
    char * p = malloc(5);
    for (int j = 0; j < 4; ++j) {
      p[j] = envp[i][j];
    }
    p[4] = 0;
    //check if it's PATH
    if (strcmp(p, "PATH") ==0 ) {
      r = i;
      break;
    }
  }
  return envp[r];
}

/**
* parses the string based on the given char, stolen from HW2
*/
char ** parse(char * c, char b)
{
  //parsing the path does not require this sys out
  if (b == ' ') {
    printf("String = '%s'\n", c);
  }

  int pointers = 2;
  for( int i =0;  i < strlen(c); i++) {
    if(c[i] == b) {
      pointers++;
    }
  }

  char ** result = malloc(pointers * sizeof(int *));

  char *letters = c;

  for (int i = 0; i < pointers; i++) {
    int a =0;
    while(letters[a] != b && letters[a] != 0) {
      a++;
    }
    result[i] = malloc(a+1);
    int count = 0;
    while (letters[0] != b && letters[0] != 0) {
      result[i][count] = letters[0];
      letters++;
      count++;
    }

    result[i][count] = 0;
    letters++;
  }

  result[pointers -1] = NULL;

  return result;
}

/**
* Print stuff within char ** , stolen from HW2... and 1 maybe?
*/
void pointerPrint(char ** argv)
{
  printf("Address %p\n", &argv);
  for(int i=0; argv[i] != NULL; i++)
  {
    printf("Base[%d] Address %p PointerValue %p String=%s\n", i, &argv[i], argv[i], argv[i]);
  }
}

/**
* Running the shell, cool stuff happens here
*/
void runShell(char ** argv) {

  //loop
  char *String = malloc(200);
  printf("Dat Bash $");

  //run shell
  while (scanf(" %199[^\n]s", String)) {
    if (strcmp(String, "quit") == 0) {
      break;
    }

    int h;
    int g;


    char ** pipedCommands = parse(String, '|');
    int pipes =0;
    for (int i =0; i < strlen(String); i++) {
      if(String[i] == '|') {
        pipes++;
      }
    }
    // printf("pipes = %i\n", pipes);

    char **st = malloc((sizeof(char *)) * (pipes +1));
    char ***parameters = malloc(sizeof(char *) * (pipes +1));



    for(int i= 0; pipedCommands[i] != NULL; i++) {
      h =0;
      g=0;

      while (pipedCommands[i][g] != '|' && pipedCommands[i][g] != 0) {
        g++;
      }

      char ** commands = malloc(g+1);

      //check command for parameter
      while (pipedCommands[i][h] != ' ' && pipedCommands[i][h] != 0) {
        h++;
      }
      commands[i] = malloc(h + 1);
      for (int l = 0; l < h; ++l) {
        commands[i][l] = pipedCommands[i][l];
      }
      commands[i][h] = 0;

      //find the longest path variable
      int lv = 0;
      int envVariables =0;
      for (int j = 0; argv[j] != NULL; j++) {
        if (strlen(argv[j]) > lv) {
          lv = strlen(argv[j]);
          envVariables++;
        }
      }



      st[i] = malloc(strlen(commands[i]) + 2 + lv);

      bool f = false;

      for (int k = 0; argv[k] != NULL; k++) {


        char *a = malloc(strlen(argv[k]) * 10);
        for (int j = 0; j < strlen(argv[k]); j++) {
          a[j] = argv[k][j];
        }
        //find the commands path
        char *s = strcat(a, "/");
        st[i] = strcat(s, commands[i]);

        if (file_exist(st[i])) {
          printf("Found \'%s\'!\n", st[i]);
          f = true;
          break;
        }
      }

      if(!f) {
        printf("\'%s\' was not found.\n", st[i]);
      }

      //format the command's path
      parameters[i] = parse(pipedCommands[i], ' ');
      int count = 0;
      for (int k = 0; parameters[i][k]; k++) {
        count++;
      }

    }

    //check if the files_exist
    if (files_exist(st, pipes)) {
      int i;
      pid_t result;

      //create pipes
      int in, fd [2];
      in =0;

      //begin executing commands
      result = fork();
      if (result == 0) {

        //is a pipe needed? and how many
        for (i = 0; i < pipes; i++) {
          pipe(fd);

          //create more branches for children
          if ((result=fork()) ==0) {

            //child close input pipe
            if ((in != 0)) {
              dup2(in, 0);
              close(in);
            }

            //parents close write pipe
            if(fd[1] != 1) {
              dup2(fd[1], 1);
              close(fd[1]);
            }

            //execute piped commands
            execv(st[i], parameters[i]);
          }else {
            wait(&result);
          }

          //close write pipe
          close(fd [1]);

          //direct output to input for next child
          in = fd [0];
        }
        //set stdin for final command
        if (in != 0) {
          dup2(in, 0);
        }
        //close pipe
        close(fd[0]);

        //execute final command
        execv(st[i], parameters[i]);
      } else {
        wait(&result);
      }
    }

    printf("Dat Bash $");
  }

}

/**
* checks if a file exists
*/
int file_exist (char *filename) {
  bool b = false;
  if (access(filename, F_OK) != -1) {
    b = true;
  }
  return b;
}

int files_exist (char **filenames, int pipes) {
  bool b = true;
  for (int i = 0; i < pipes+1; i++) {

    if (!file_exist(filenames[i])) {
      b = false;
    }
  }

  return b;
}
