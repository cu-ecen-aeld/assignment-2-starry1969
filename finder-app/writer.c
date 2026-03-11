#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <string>\n", argv[0]);
        return 1;
    }

    const char *filepath = argv[1];
    const char *writestr = argv[2];

    openlog("writer", 0, LOG_USER);
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, filepath);

    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        syslog(LOG_ERR, "Error opening file %s", filepath);
        closelog();
        return 1;
    }

    ssize_t rc = write(fd, writestr, strlen(writestr));
    if (rc == -1) {
        syslog(LOG_ERR, "Error writing %s to %s", writestr, filepath);
        close(fd);
        closelog();
        return 1;
    }

    close(fd);
    closelog();
    return 0;
}
