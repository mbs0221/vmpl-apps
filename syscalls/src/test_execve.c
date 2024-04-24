#include <unistd.h>

int main() {
    char *newargv[] = { "/bin/ls", "-l", "/", NULL };
    char *newenviron[] = { NULL };

    execve("/bin/ls", newargv, newenviron);

    perror("execve");   /* execve() only returns on error */
    return 0;
}