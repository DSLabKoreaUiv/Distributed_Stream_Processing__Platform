 /* 
 * HTTPServer.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFSIZE 1024
#define LINESIZE 100

void * clnt_connection(void *arg);
char* strtrim(char *str);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char * file);
void send_error(FILE* fp);
void error_handling(char *message);

int main(int argc, char **argv)
{	
  int serv_sock;
  int clnt_sock;

  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  int clnt_addr_size;
  char buf[BUFSIZE];
  pthread_t thread; 

  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  serv_sock=socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock==-1)
    error_handling("socket() error");
  
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
    error_handling("bind() error");
  if(listen(serv_sock, 20)==-1)
    error_handling("listen() error");

  while (1) {     
    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    printf("연결요청 : %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    pthread_create(&thread, NULL, clnt_connection, &clnt_sock); 
  }
  return 0;
}

void * clnt_connection(void *arg)
{
  int clnt_sock=*((int*)arg);
  char req_line[LINESIZE];
  FILE* clnt_read;
  FILE* clnt_write;

  char method[10];
  char ct[15];
  char file_name[30];
  
  clnt_read = fdopen(clnt_sock, "r");
  clnt_write = fdopen(dup(clnt_sock), "w");
   
  fgets(req_line, LINESIZE, clnt_read);
  fputs(req_line, stdout);

  if(strstr(req_line, "HTTP/")==NULL){
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return;
  }
  strcpy(method, strtok(req_line, " /"));
  strcpy(file_name, strtok(NULL, " /"));
  strcpy(ct, content_type(file_name));
  
  if(strcmp(method, "GET")!=0){
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return;
  }
      
  while(1){
    fgets(req_line, LINESIZE, clnt_read);
    fputs(req_line, stdout);
    if(strcmp(strtrim(req_line),"")==0)
      break;
  }
  fclose(clnt_read);
  send_data(clnt_write, ct, file_name); 
}

char* strtrim(char *str)
{
  int end=strlen(str);
  while(str[end]<=' ' && end>0)  end--;
  str[end+1]=0;
  while(*str<=' ' && *str!=0) str++;
  
  return str;
}

void send_data(FILE* fp, char* ct, char* file_name)
{     
  	char protocol[]="HTTP/1.0 200 OK\r\n";
	char server[]="Server:Best Http Server \r\n";
	char cnt_len[]="Content-length:2048\r\n";
	char cnt_type[LINESIZE];
	char buf[BUFSIZE];
	FILE* send_file;
	int len;

	sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);

	send_file = fopen(file_name, "r");
	if(send_file == NULL){
		send_error(fp);
		return;
	}

	/* 헤더 정보 전송 */
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);

	/* 요청 데이터 전송 */
	while( len=fgets(buf, BUFSIZE, send_file ) != 0) {
		fputs(buf, fp);
		fflush(fp);
	}
	fflush(fp);
	fclose(fp);
}

char* content_type(char* file){
  char extension[LINESIZE];
  char file_name[LINESIZE];
  strcpy(file_name, file);
  strtok(file_name, ".");
  strcpy(extension, strtok(NULL, "."));

  if(!strcmp(extension, "html")||!strcmp(extension, "htm")) return "text/html";
  if(!strcmp(extension, "txt")||!strcmp(extension, "c")) return "text/plain"; 

  return "text/plain";
}

void send_error(FILE* fp)
{	
	char protocol[]="HTTP/1.0 400 Bad Request\r\n";
	char server[]="Server:Best Http Server \r\n";
	char cnt_len[]="Content-length:2048\r\n";
	char cnt_type[]="Content-type:text/html\r\n\r\n";
	char content[]="<html><head><title>NETWORK</title></head>"
	       "<body><font size=+5><br>오류 발생! 요청 파일명 및 요청 방식 확인!"
		   "</font></body></html>";

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	fflush(fp);
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
