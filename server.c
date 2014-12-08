#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h> // for close
#include <stdlib.h> // for EXIT_SUCCESS
#include <stdio.h>

#define ADDRESS "mysocketname" // Имя файла для сокета

int main() {
	int listensocket;
	int connectsocket;
	FILE *fp;
	char msgbuff[255];
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;
	socklen_t ca_len;

	// Создание слушающего сокета:
	if ((listensocket = socket(
		AF_UNIX,		// домен сокетов в файловом пространстве имен,
		SOCK_STREAM,	// потоковый сокет,
		0))				// протокол
			< 0) {
		perror("Server: socket");
		return EXIT_FAILURE;
	}
	printf("Server: socket ready!\n");

	// Создание адреса для сокета:
	memset (&server_addr, 0, sizeof (struct sockaddr_un)); // обнуление,
	server_addr.sun_family = AF_UNIX;		// домен,
	strcpy(server_addr.sun_path, ADDRESS);	// файл.

	// Связывание слушайщего сокета и его адреса:
	if (bind(
		listensocket,						// дескриптор слушающего сокета,
		(struct sockaddr*) &server_addr,	// адрес слушающего сокета,
		sizeof(server_addr))				// размер адреса.
			< 0) {
		perror("Server: bind");
		return EXIT_FAILURE;
	}
	printf("Server: bind is ok!\n");

	// Прослушиваем сокет на предмет новых соединений
	if (listen(
		listensocket,	// дескриптор слушающего сокета,
		5)				// максимальное кол-во одновременных запросов.
			< 0) {
		perror("Server: listen");
		return EXIT_FAILURE;
	}
	printf("Server: listening...\n");

	// Установка связи с клиентским сокетом
	memset (&client_addr, 0, sizeof (struct sockaddr_un)); // обнуление,
	ca_len = sizeof(client_addr);		// размер адреса клиента,
	if ((connectsocket = accept(
		listensocket,					// дескриптор слушающего сокета,
		(struct sockaddr*) &client_addr,	// адрес клиентского сокета,
		&ca_len))						// размер адреса.
			< 0) {
		perror("Server: accept");
		return EXIT_FAILURE;
	}
	printf("Server: accept new connection.\n");

	// Отправка сообщения клиенту
	send(connectsocket, "Hello", 6, 0);

	// Читаем сообщение клиента из сокета соединения
	recv(connectsocket, msgbuff, sizeof(msgbuff), 0);
	printf("%s\n", msgbuff);

	// Обработка информации от клиента

	// Завершение связи
	close(listensocket);
	close(connectsocket);
	unlink(ADDRESS);

	printf("Server: exiting...\n");
	return EXIT_SUCCESS;
}
