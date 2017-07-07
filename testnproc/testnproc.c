#include <stdio.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <errno.h>

int main( int argc, char *argv[] )
{
    int i;
    int p[1500];
    static char template[] = "/tmp/testnprocXXXXXX";
    // get nproc limit
    struct rlimit rl, rlp;

    if ( getrlimit( RLIMIT_NPROC , &rl) != 0 ) {
        printf("getrlimit() RLIMIT_NPROC failed with errno=%d\n", errno);
        return 255;
    }

    if ( getrlimit( RLIMIT_NOFILE , &rlp) != 0 ) {
        printf("getrlimit() RLIMIT_NOFILE failed with errno=%d\n", errno);
        return 255;
    }

    // fork 1500 times
    for( i=1 ; i<= 1500 ; i++ ) {
        p[i] = fork();
        if ( p[i] >= 0 ) {
            if (  p[i] == 0 ) {
                printf("parent says fork number %d sucessful \n" , i ); 
            } else {
                printf(" child says fork number %d pid %d \n" , i , p[i] ); 
                mkstemp(template);
                int fd = open(template, O_CREAT|O_RDWR|O_TRUNC, 0600);
                if (fd < 0) {
                    printf("Could not open file %s, errno: %d, (nofile: soft=%d hard=%d)", template, errno, rlp.rlim_cur, rlp.rlim_max);
                }

                sleep(10000);
                if(fd) {
                    close(fd);
                    unlink(template);
                }
                break;
            }
        } else {
                printf("parent says fork number %d failed (nproc: soft=%d hard=%d) (nofile: soft=%d hard=%d) with errno=%d\n", i, rl.rlim_cur, rl.rlim_max, rlp.rlim_cur, rlp.rlim_max, errno);
            return 255;
        }
    }
}
