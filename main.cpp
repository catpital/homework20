//homework20
#include <iostream>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>


using namespace std;
#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта


struct sockaddr_in serveraddress, client;
socklen_t length;
int socket_file_descriptor, connection, bind_status, connection_status;
char message[MESSAGE_LENGTH];

int main() {
// Создадим сокет
   socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        cout << "Creation of Socket failed!" << endl;
        exit(1);
    }

    // Установим адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Установим соединение с сервером
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        cout << "Connection with the server failed.!" << endl;
        
       //пробуем запустить сервер
serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    if (bind_status == -1) {
        cout << "Socket binding failed.!" << endl;
        exit(1);
    }
    // Поставим сервер на прием данных 
    connection_status = listen(socket_file_descriptor, 5);
    if (connection_status == -1) {
        cout << "Socket is unable to listen for new connections.!" << endl;
        exit(1);
    }
    else {
        cout << "Server is listening for new connection: " << endl;
    }
    length = sizeof(client);
    connection = accept(socket_file_descriptor, (struct sockaddr*)&client, &length);
    if (connection == -1) {
        cout << "Server is unable to accept the data from client.!" << endl;
        exit(1);
    }

    // Communication Establishment
    while (1) {
        bzero(message, MESSAGE_LENGTH);
        read(connection, message, sizeof(message));
        if (strncmp("end", message, 3) == 0) {
            cout << "Client Exited." << endl;
            cout << "Server is Exiting..!" << endl;
            break;
        }
        cout << "Data received from client: " << message << endl;
        bzero(message, MESSAGE_LENGTH);
        cout << "Enter the message you want to send to the client: " << endl;
        cin >> message;
        ssize_t bytes = write(connection, message, sizeof(message));
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (bytes >= 0) {
            cout << "Data successfully sent to the client.!" << endl;
        }
    }
        
    }
    // если не запущен сервер то запускаем
// иначе запускаем клиента
    else {
        while (1) {
        bzero(message, sizeof(message));
        cout << "Client is work " << endl;
        cout << "Enter the message you want to send to the server: " << endl;
        cin >> message;
        if ((strncmp(message, "end", 3)) == 0) {
            write(socket_file_descriptor, message, sizeof(message));
            cout << "Client Exit." << endl;
            break;
        }
        ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (bytes >= 0) {
            cout << "Data send to the server successfully.!" << endl;
        }
        bzero(message, sizeof(message));
        // Ждем ответа от сервера
        read(socket_file_descriptor, message, sizeof(message));
        cout << "Data received from server: " << message << endl;
    }
    }
// закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
    return 0;
}