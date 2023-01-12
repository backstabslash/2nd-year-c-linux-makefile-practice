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
#define PERM_FILE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
#define SERVER_FIFO_NAME "/tmp/fifo.request"
#define FIFO_PREFIX "/tmp/fifo.answer"

struct simplemessage { 
    pid_t sm_clientpid;
    char sm_data[200];
}; 
char * makefifoname(pid_t pid,  const char* prefix) 
{
   char *pfifoname;
   asprintf(&pfifoname,"%s.%ld", prefix, (long) pid);
   return pfifoname;
}

int main(int argc,   char *argv[])
{
   int fd_server,
   fd_client = -1;
   ssize_t nread;
   struct simplemessage msg;
   char* pfifoname;
   
   msg.sm_clientpid = getpid();
   printf("Клиент %ld запущен\n", (long) msg.sm_clientpid);
   pfifoname = makefifoname(msg.sm_clientpid, FIFO_PREFIX);
   if (mkfifo(pfifoname, PERM_FILE)== -1  && errno  != EEXIST)
   {
      fprintf(stderr, "Ошибка при создании канала %s: %s\n",
              pfifoname, strerror(errno));
      return 1;
   }
   if ((fd_server = open(SERVER_FIFO_NAME, O_WRONLY))<0)
   {
      fprintf(stderr, "Ошибка открытия канала %s: %s\n",
              SERVER_FIFO_NAME, strerror(errno));
      return 2;
   }
   scanf("%s",msg.sm_data);
   write(fd_server ,&msg, sizeof(msg));
   if ((fd_client = open(pfifoname, O_RDONLY)) == -1)
   {
      fprintf(stderr, "Ошибка открытия канала %s: %s\n",
              pfifoname, strerror(errno));
      return 3;
   }
   nread = read(fd_client, &msg, sizeof(msg));
   printf("%s\n",  msg.sm_data); 
   close(fd_server); 
   close(fd_client); 
   //unlink(SERVER_FIFO_NAME);
   printf("Клиент %ld закончил  работу\n",  msg.sm_clientpid);
}

