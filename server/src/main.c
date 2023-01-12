#define PERM_FILE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
#define SERVER_FIFO_NAME "/tmp/fifo.request"
#define FIFO_PREFIX "/tmp/fifo.answer"

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include <ftw.h>
#include <limits.h>     
#include <unistd.h>  
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

char* makefifoname(pid_t pid, const char* prefix)
{
    char* pfifoname;
    asprintf(&pfifoname, "%s.%ld", prefix, (long)pid);
    return pfifoname;
}

struct simplemessage
{
    pid_t sm_clientpid;
    char sm_data[200];
};
void func(char* str)
{
	int fdrd;
	if((fdrd = open(str, O_RDONLY)) < 0)
      {
        str[0]='W';
        str[1]='r';
        str[2]='o';
        str[3]='n';
        str[4]='g';
        str[5]=' ';
        str[6]='F';
        str[7]='i';
        str[8]='l';
        str[9]='e';
      }else{
	struct stat st;
	stat(str, &st);
	char *atime = ctime(&st.st_atime);
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = ' ';
	}
	str[0] = 'a';
	str[1] = 't';
	str[2] = 'i';
	str[3] = 'm';
	str[4] = 'e';
	str[5] = ' ';
	int counter = 6;
	for (int i = 0; i < strlen(atime); i++)
	{
	str[counter] = atime[i];
	counter++;
	}
	str[counter++] = 'm';
	str[counter++] = 't';
	str[counter++] = 'i';
	str[counter++] = 'm';
	str[counter++] = 'e';
	str[counter++] = ' ';
	char *mtime = ctime(&st.st_mtime);
	for (int i = 0; i < strlen(mtime); i++)
	{
	str[counter] = mtime[i];
	counter++;
	}
	str[counter++] = 'c';
	str[counter++] = 't';
	str[counter++] = 'i';
	str[counter++] = 'm';
	str[counter++] = 'e';
	str[counter++] = ' ';
	char *cctime = ctime(&st.st_ctime);
	for (int i = 0; i < strlen(cctime); i++)
	{
	str[counter] = cctime[i];
	counter++;
	}
	}
}
int main(void)
{
    int fd_server,
        fd_dummy,
        fd_client;
    ssize_t nread;
   struct simplemessage msg;
printf("сервер запущен\n");
if (mkfifo(SERVER_FIFO_NAME, PERM_FILE) == -1 && errno != EEXIST)
{
    fprintf(stderr, "Ошибка при создании канала %s: %s\n", SERVER_FIFO_NAME, strerror(errno));
    return 1;
}
if ((fd_server = open(SERVER_FIFO_NAME, O_RDONLY)) < 0)
{
    fprintf(stderr, "Ошибка открытия канала %s: %s\n", SERVER_FIFO_NAME, strerror(errno));
    return 2;
}
if ((fd_dummy = open(SERVER_FIFO_NAME, O_WRONLY)) < 0)
{
    fprintf(stderr, "Ошибка открытия канала %s: %s\n", SERVER_FIFO_NAME, strerror(errno));
    return 2;
}
while ((nread = read(fd_server, &msg, sizeof(msg))) > 0)
{
    char* pfifoname;
    func(msg.sm_data);
    pfifoname = makefifoname(msg.sm_clientpid, FIFO_PREFIX);
    fd_client = open(pfifoname, O_WRONLY);
    write(fd_client, &msg, sizeof(msg));
    close(fd_client);
    free(pfifoname);
}
}
