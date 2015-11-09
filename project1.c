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
char ** parse(char * c, char b);
char * findPath(char ** envp);

int main(int argc, char ** argv, char ** envp) {
    char *fullpath = findPath(envp);
    //get the path variables
    char ** path = parse(fullpath, ':');
    pointerPrint(path);

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

    //parsing the path does not require this sys out
    if (b == ' ') {
        printf("Number of pointers = %i\n", pointers);
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

        int h = 0;

        //check command for parameter
        while (String[h] != ' ' && String[h] != 0) {
            h++;
        }
        char *command = malloc(h + 1);
        for (int l = 0; l < h; ++l) {
            command[l] = String[l];
        }
        command[h] = 0;


        //find the longest path variable
        int lv = 0;
        for (int i = 0; argv[i] != NULL; i++) {
            if (strlen(argv[i]) > lv) {
                lv = strlen(argv[i]);
            }
        }

        char *st = malloc(strlen(command) + 2 + lv);


        for (int i = 0; argv[i] != NULL; i++) {
            char *a = malloc(strlen(argv[i]));
            for (int j = 0; j < strlen(argv[i]); j++) {
                a[j] = argv[i][j];
            }

            //find the commands path
            char *s = strcat(a, "/");
            st = strcat(s, command);
            printf("Checking %s\n", st);
            if (file_exist(st)) {
                printf("Found!\n");
                break;
            }
        }

        //format the command's path
        char **parameters = parse(String, ' ');
        int count = 0;
        for (int k = 0; parameters[k]; k++) {
            count++;
        }
        pointerPrint(parameters);

        if (file_exist(st)) {
            //run the command via fork
            pid_t p;
            int result = fork();
            if (result == 0) {
                p = getpid();
		printf("%schild\n", st);
                execv(st, parameters);

            } else {
                p = getpid();
		printf("blargparent\n");
                wait(result);
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

