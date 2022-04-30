#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LEN 100 /* The maximum length command */

int main(void) {
  char *args[MAX_LEN / 2 + 1]; /* command line arguments */
  int should_run = 1;          /* flag to determine when to exit program */
  int background = 0;

  while (should_run) {
    printf("my_shell> ");
    fflush(stdout);
    /**
     * 표준입출력으로부터 문자열을 입력 받은 후:
     * (1) fork()를 통해 자식 프로세스를 생성
     * (2) 자식 프로세스가 execvp()를 호출하도록 할 것
     * (3) 만약 입력받은 문자에 &가 포함되어 있으면,
     *     부모 프로세스는 wait() 호출하지 않음
     */
	char *input = (char*)malloc(MAX_LEN*sizeof(char));
	fgets(input, MAX_LEN, stdin);
	//free(input);
	if(input == NULL) {
		goto last;
	}
	
	char *p;
   	p = strtok(input, " ");

	pid_t pid;
   	pid = fork();
	

  }
  last:
	free(input);
  return 0;
}
