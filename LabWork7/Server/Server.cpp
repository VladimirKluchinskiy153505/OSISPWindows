#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include<winsock2.h>
#include <iostream>
SOCKET Connections[100];
int Counter = 0;
std::string Ip;
unsigned short int PortToNetwork;
void Clienthandler(int index) {
    int msg_size;
    while (true) {
        int recvResult = recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
        if (recvResult <= 0) {
            std::cout << "Client disconnected from thread " << index << std::endl;
            closesocket(Connections[index]);
            Connections[index] = -1;
            //TerminateThread(GetCurrentThread(),0);
            ExitThread(0);
        }
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        int recvResult1 = recv(Connections[index], msg, msg_size, NULL);
        if (recvResult1 <= 0) {
            std::cout << "Client disconnected from thread " << index << std::endl;
            closesocket(Connections[index]);
            Connections[index] = -1;
            ExitThread(0);
        }
        for (int i = 0; i < Counter; ++i) {
            if (i != index && Connections[i]>=0) {
                send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                send(Connections[i], msg, msg_size, NULL);
            }
        }
        delete[] msg;
    }
}
int main(int argc, char* argv[])
{
    system("color 0A");
    if (argc > 2) {
        Ip = argv[1];
        PortToNetwork = std::strtoul(argv[2], nullptr, 10);
    }
    else {
        Ip = "127.0.0.1";
        PortToNetwork = 1111;
    }
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error\n";
        exit(1);
    }
    SOCKADDR_IN addr; //структура для хранения адреса
    int size_of_addr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr(Ip.c_str()); //хранит id адрес
    addr.sin_port = htons(PortToNetwork); //порт для идентификации программы с постю данными
    addr.sin_family = AF_INET;//семейство для интернет протоколов

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);
    SOCKET newConnection;
    for (int i = 0; i < 100; ++i) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &size_of_addr);
        if (newConnection == 0) {
            std::cout << "Error#2\n";
        }
        else {
            std::cout << "Client is connected\n";
            std::string msg = "Hello. It's my first network programm!\n";
            int msg_size = msg.size();
            send(newConnection, (char*)&msg_size, sizeof(int), NULL);
            send(newConnection, msg.c_str(), msg_size, NULL);
            Connections[i] = newConnection;
            ++Counter;
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Clienthandler, (LPVOID)i, NULL, NULL);
        }
    }
    system("pause");
    return 0;
}