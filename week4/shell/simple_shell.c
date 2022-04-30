#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LEN 100 /* The maximum length command */
void newline(char *parsed);
int compare(char *);

/*
* 표준입출력으로부터 문자열을 입력 받은 후:
* (1) fork()를 통해 자식 프로세스를 생성
* (2) 자식 프로세스가 execvp()를 호출하도록 할 것
* (3) 만약 입력받은 문자에 &가 포함되어 있으면,
*     부모 프로세스는 wait() 호출하지 않음
*/

int main(void) {
    char *line = (char*)malloc(MAX_LEN*sizeof(char));
    char *args[MAX_LEN / 2 + 1]; /* command line arguments */
    int should_run = 1;          /* flag to determine when to exit program */
    int background = 0;

    while (should_run) {
        printf("my_shell> ");
        fflush(stdout);

        if ( !fgets(line, MAX_LEN, stdin) ) {
            break;
        }

        char *p;
        int i, argc=0;
        p = strtok(line, " ");
        while( p != NULL ) {
            args[argc] = p;
            p = strtok(NULL, " ");
            argc++;
        }

        for ( i=0; i<argc; i++ ) {
            newline(args[i]);
        }

        for ( i=0; i<argc; i++ ) {
            background = compare(args[i]);
            if ( background ) {
                args[i] = NULL;
                break;
            }
        }

        args[argc] = NULL;

        if ( strcmp(args[0],"\0") == 0 ) {
            continue;
        }
        if ( strcmp(args[0], "exit") == 0 ) {
            should_run = 0;
            continue;
        }

        int pid = fork(), stat;
        if ( pid < 0 ) {
            perror("fork error\n");
            should_run = 0;
            continue;
        } else if ( pid == 0 ) {
            execvp(args[0], args);
        } else {
            if (!background) {
                printf("waiting for child, not a background process\n");
                waitpid(pid, &stat, 0);
                printf("child process complete\n");
            } else {
                printf("background process\n");
            }
        }
        sleep(1);
    }
    return 0;
}

void newline(char *parsed) {
    int i;
    for ( i=0; i<strlen(parsed); i++ ) {
        if ( parsed[i] == '\n' ) {
            parsed[i] = '\0';
        }
    }
}

int compare(char *args) {
    if ( strcmp(args, "&") == 0 ) {
        return 1;
    } else {
        return 0;
    }
}
