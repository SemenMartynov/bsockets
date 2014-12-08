#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h> // for close
#include <stdlib.h> // for EXIT_SUCCESS
#include <stdio.h>

#define ADDRESS "mysocketname" // Имя файла для сокета

int main() {
	int clientsocket;
	FILE *fp;
	char msgbuff[255];
	struct sockaddr_un sa;

	// Создание сокета
	if ((clientsocket = socket(
		AF_UNIX,		// домен сокетов в файловом пространстве имен,
		SOCK_STREAM,	// потоковый сокет,
		0))				// протокол
			< 0) {
		perror("Client: socket");
		return EXIT_FAILURE;
	}
	printf("Client: socket ready!\n");

	// Создание адреса для сокета:
	memset (&sa, 0, sizeof (sa)); // обнуление,
	sa.sun_family = AF_UNIX;		// домен,
	strcpy(sa.sun_path, ADDRESS);	// файл.

	// Соединение с сервером:
	if (connect(
		clientsocket,				// дескриптор сокета
		(struct sockaddr*) &sa,		// адрес сокета
		(socklen_t) sizeof(sa))		// размер адреса
			< 0) {
		perror("Client: connect");
		return EXIT_FAILURE;
	}
	printf("Client: connecting...\n");

	// Чтение сообщение из сокета
	recv(clientsocket, msgbuff, sizeof(msgbuff), 0);
	printf("%s\n", msgbuff);

	// Обработка информации от сервера

	// Отправка информации серверу
	send(clientsocket, "Client", 7, 0);

	// Завершение связи
	close(clientsocket);

	printf("Client: exiting...\n");
	return EXIT_SUCCESS;
}
