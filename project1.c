#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>    //found on stack overflow

void pointerPrint(char ** argv);
char ** getPath(char * fullpath);
void runShell(char ** argv);
char * findPathName(char ** argv);
int file_exist (char *filename);
char ** parse(char * c);
char * findPath(char ** envp);
int main(int argc, char ** argv, char ** envp) {
    char *fullpath = findPath(envp);
    //full path from environmental variable
//    char * fullpath  = envp[20];
    //get the path variables
    char ** path = getPath(fullpath);
    pointerPrint(path);

    //Run shell
    runShell(path);

    return 0;
}
char * findPath(char ** envp) {
    int r = 0;
    for (int i = 0; envp[i] != NULL ; ++i) {
        char * p = malloc(5);
        for (int j = 0; j < 4; ++j) {
            p[j] = envp[i][j];
        }
        p[4] = 0;
        if (strcmp(p, "PATH") ==0 ) {
            r = i;
            break;
        }
    }
    return envp[r];
}

/**
 * gets the path variables, taken from HW2
 */
char ** getPath(char * fullpath) {
    int count =1;

    //count number of ':' or path variables
    for (int i = 0; i < strlen(fullpath); ++i) {
        if (fullpath[i] == ':') {
            count+=1;
        }
    }

    //allocate memory
    char ** path = malloc(count * sizeof(char *));

    //place path variables within array
    char * p = fullpath;
    for (int i = 0; i < count; i++) {
        int a =0;
        while(p[a] != ':' && p[a] != 0) {
            a++;
        }
        path[i] = malloc(a+1);
        int count = 0;
        while (p[0] != ':' && p[0] != 0) {
            path[i][count] = p[0];
            p++;
            count++;
        }
        path[i][count] = 0;
        p++;
    }

    return path;
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
//    while (!feof(stdin)) {
    printf("Dat Bash $");
//    while (scanf(" %199[^\n]s", String) && String != 'quit') {
    while (scanf(" %199[^\n]s", String) && !feof(stdin)) {
        //printf("Dat Bash $");
//        scanf("%199[^\n]s", String);
        int h = 0;
        while (String[h] != ' ' && String[h] != 0) {
            h++;
        }
        char * command = malloc(h+1);
        for (int l = 0; l < h; ++l) {
            command[l] = String[l];
        }
        command[h] =0;

        char * st = malloc(20);

        for (int i =0; argv[i] != NULL; i++) {
            char * a = malloc(strlen(argv[i]));
            for (int j = 0; j < strlen(argv[i]); j++) {
                a[j] = argv[i][j];
            }
            //strcpy( a, argv[i]);
            //char * s = malloc(sizeof(char) * ((strlen(argv[i])) + 2));
            char * s = strcat(a, "/");
            st = strcat(s, command);
            printf("Checking %s\n", st);
            if (file_exist (st)) {
                printf("Found!\n");
                break;
            }
        }


//        char ** parameters = parse(String);
//        pointerPrint(parameters);
//        char *const parmList[] = {"/bin/ls", NULL};

        char ** parameters = parse(String);
        int count =0;
        for (int k = 0; parameters[k]; k++) {
            count++;
        }
        pointerPrint(parameters);
//        char ** parmList = malloc(sizeof(char *) * (count+1));
//        parmList[0] = st;
//        for (int i = 0; parameters[i] != NULL; i++) {
//            parmList[i+1] = parameters[i];
//        }
//        parmList[count + 2] = NULL;
//        pointerPrint(parameters);

        //find pathName

        pid_t p;
        int result = fork();
        if (result == 0) {
            p = getpid();
//            printf("%s\n", st);
            execv(st, parameters);
//            printf("child");

        } else {
            p = getpid();
            wait(result);

        }
//        char *String = malloc(200);
        printf("Dat Bash $");
        //String = malloc(200);
    }
}

char * findPathName(char ** argv) {

}

int file_exist (char *filename) {
    struct stat   buffer;
    return (stat (filename, &buffer) == 0);
}

char ** parse(char * c)
{

    char ** result;
    //TODO add code here!!


    printf("String = '%s'\n", c);
    int pointers = 0;
    char *cfp = c;
    for( int i =0;  i < strlen(c); i++) {
        if(cfp[i] == ' ') {
            pointers++;
        }
    }

    pointers+=2;
    printf("Number of pointers = %i\n", pointers);

    result = malloc(pointers * sizeof(int *));

    char *letters = c;

    for (int i = 0; i < pointers; i++) {
        int a =0;
        while(letters[a] != ' ' && letters[a] != 0) {
            a++;
        }
        result[i] = malloc(a+1);
        int count = 0;
        while (letters[0] != ' ' && letters[0] != 0) {
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