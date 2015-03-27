//Ejercicio: Pequenyo servidor de datagrama
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORTNUM 9999

int main(int argc, char *argv[])
{
	char* msg = "Hola mundo!\n";
	struct sockaddr_in dest;
	struct sockaddr_in serv;
	int server;
	socklen_t socksize = sizeof(struct sockaddr_in);
	char buf[512];
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(PORTNUM);

	server = socket(AF_INET, SOCK_DGRAM, 0);
	if (server < 0) {
		perror("Error: Socket()\n");
		return -1;
	}

	int chk_bind = bind(server, (struct sockaddr *)&serv, sizeof(struct sockaddr));
	if (chk_bind < 0) {
		perror("Error: Bind()\n");
		return -1;
	}

//	printf("IP %s - envía un hola\n", inet_ntoa(dest.sin_addr));
//	sendto(client, msg, strlen(msg), 0, NULL, 0);

	for (;;)
	{
		int n = recv(server, buf, sizeof(buf), 0);
		buf[n] = 0;
		printf("Received the following:%s", buf);
		if(!strcmp("exit\n", buf)) return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
