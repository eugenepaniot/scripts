#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>

int main( int argc, char *argv[] )
{
    sigset_t set;
    char* buffer;
    unsigned long long max = -1;
    unsigned long long mb = 0;
    pid_t *p;
    int i;
    int max_proc = 1;

    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    if(argc > 1)
        max = atoi(argv[1]);

    if(argc > 2)
        max_proc = atoi(argv[2]);

    p = (pid_t *)malloc(sizeof(pid_t) * max_proc);
    if (p == NULL) {
        perror("malloc failed");
        return 255;
    }

    for (i = 0; i < max_proc; i++) {
      p[i] = fork();

      if(p[i]<0) {
          perror("fork failed");
          exit(255);
      } else if (p[i] == 0) {
          pid_t pid = getpid();

          while((buffer=malloc(1024*1024)) != NULL && mb != max) {
              mb++;
              memset(buffer, 0, 1024*1024);
              printf("%d(%d) allocated %llu MB \n", pid, i, mb);
          }

          if (buffer == NULL) {
              perror("malloc failed");
              exit(255);
          }

          sleep(-1);
      }
    }

    sigsuspend(&set);
    return 0;
}
