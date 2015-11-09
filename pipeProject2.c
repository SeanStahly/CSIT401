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
int files_exist (char **filenames);
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

    char **st = malloc(sizeof(char *) * (pipes +1));
    char ***parameters = malloc(sizeof(char *) * (pipes +1));

    for(int i= 0; pipedCommands[i] != NULL; i++) {
      h =0;
      g=0;

      while (pipedCommands[i][g] != '|' && pipedCommands[i][g] != 0) {
        g++;
      }
      // printf("blarg %s\n", pipedCommands[i]);
      // pointerPrint(pipedCommands);
      char ** commands = malloc(g+1);

      //check command for parameter
      // while (String[h] != ' ' && String[h] != 0) {
      //     h++;
      // }
      while (pipedCommands[i][h] != ' ' && pipedCommands[i][h] != 0) {
        h++;
      }
      // printf("h =%i\n", h);
      commands[i] = malloc(h + 1);
      for (int l = 0; l < h; ++l) {
        commands[i][l] = pipedCommands[i][l];
        // printf("commands = %s\n",commands[i] );
      }
      commands[i][h] = 0;

      // pointerPrint(commands);

      //find the longest path variable
      int lv = 0;
      for (int j = 0; argv[j] != NULL; j++) {
        if (strlen(argv[j]) > lv) {
          lv = strlen(argv[j]);
        }
      }


      st[i] = malloc(strlen(commands[i]) + 2 + lv);

      bool f = false;
      for (int k = 0; argv[k] != NULL; k++) {
        char *a = malloc(strlen(argv[k]));
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
        printf("Command was not found.\n");
      }

      //format the command's path
      parameters[i] = parse(pipedCommands[i], ' ');
      int count = 0;
      for (int k = 0; parameters[i][k]; k++) {
        count++;
      }
      // pointerPrint(parameters[i]);

    }
    // pointerPrint(st);


    // for (int i=0; st[i] != NULL; i++) {

    // if(pipes ==0) {
    //   if (file_exist(st[0])) {
    //     // printf("%s\n", "hmm");
    //     //run the command via fork
    //     pid_t result;
    //     result = fork();
    //     if (result == 0) {
    //       // printf("blarg\n");
    //       execv(st[0], parameters[0]);
    //
    //     } else {
    //       wait(&result);
    //     }
    //   }
    // } else {



    // if (file_exist(st[0])) {
    //     int writepipe[2] = {-1, -1},
    //     readpipe[2]  = {-1, -1};
    //     pid_t result;
    //
    //     writepipe[0] = -1;
    //
    //     #define PARENT_READ   readpipe[0]
    //     #define CHILD_WRITE   readpipe[1]
    //     #define CHILD_READ    writepipe[0]
    //     #define PARENT_WRITE  writepipe[1]
    //
    //     result = fork();
    //     if(result == 0) {
    //       close(PARENT_WRITE);
    //       close(PARENT_READ);
    //
    //       dup2(CHILD_READ, 0);
    //       close(CHILD_READ);
    //       dup2(CHILD_WRITE, 1);
    //       close(CHILD_WRITE);
    //
    //       execv(st[0], parameters[0]);
    //     } else {
    //       close(CHILD_READ);
    //       close(CHILD_WRITE);
    //
    //       wait(&result);
    //     }
    //   }

    // if (files_exist(st)) {
      int i;
      pid_t result;
      int in, fd [2];
      in =0;
      // result = fork();
      // if (result == 0) {
        /* code */

      for (i = 0; i < pipes; i++) {
        pipe(fd);

        if ((result = fork()) ==0) {
        // if (result ==0) {
          if ((in != 0)) {
            dup2(in, 0);
            close(in);
          }

          if(fd[1] != 1) {
            dup2(fd[1], 1);
            close(fd[1]);
          }

          execv(st[i], parameters[i]);
        // }else {
        //   wait(&result);
        }

        close(fd [1]);

        in = fd [0];
      }
      if (in != 0) {
        dup2(in, 0);
      }

      // if ((result = fork()) == 0) {
      //   execv(st[i], parameters[i]);
      // } else {
      //   wait(&result);
      // }
      execv(st[i], parameters[i]);
    // } else {
    //   wait(&result);
    //   }
    // }


    printf("Dat Bash $");
    // }
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

int files_exist (char **filenames) {
  bool b = true;
  for (int i = 0; filenames[i] != NULL; i++) {
    if (access(filenames[i], F_OK) == -1) {
      b = false;
    }
  }

  return b;
}
