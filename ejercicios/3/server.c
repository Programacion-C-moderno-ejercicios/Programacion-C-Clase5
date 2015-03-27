/* sudo apt-get install libev-libevent-dev */
/* gcc servidor.c -lev */

//Modificar a UNIX Datagram y hacer el cliente

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <event.h>

static struct event_base *ev_base;
static struct event sigint_event;
static int sigtype;

static void sigint_callback(int fd, short event, void *data)
{
	printf("recibida señal de SIGINT mediante ctrl-c\n");
	sigtype = SIGINT;
}

static void server_read_cb(int fd, short mask, void *data)
{
	int numbytes;
	char buf[1024];

	numbytes = recv(fd, buf, sizeof(buf), 0);
	if (numbytes < 0) {
		perror("recv");
		return;
	}

	buf[numbytes - 1] = '\0';
	printf("He recibido: %s\n", buf);
}

int main(void)
{
	struct event event;
	int fd, err;

	printf("lanzando servidor.\n");

	ev_base = event_init();
	if (ev_base == NULL)
		return -1;

	signal_set(&sigint_event, SIGINT, sigint_callback, NULL);
	signal_add(&sigint_event, NULL);

	fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in sockaddr = {
		.sin_family	= PF_INET,
		.sin_addr	= INADDR_ANY,
		.sin_port	= htons(9999),
	};

	err = bind(fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if (err < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	event_set(&event, fd, EV_READ | EV_PERSIST, server_read_cb, NULL);
	event_add(&event, NULL);

	while (!sigtype)
		event_loop(EVLOOP_ONCE);

	signal_del(&sigint_event);
	event_base_free(ev_base);

	printf("cerrando servidor.\n");
}
