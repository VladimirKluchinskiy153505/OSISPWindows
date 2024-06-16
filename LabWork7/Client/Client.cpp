#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include<winsock2.h>
#include <iostream>
#include<string>
SOCKET Connection;
std::string Ip;
unsigned short int PortToNetwork;
std::string Username;
BOOL WINAPI ConsoleHandler(DWORD dwCtrlType) {
    switch (dwCtrlType) {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
        closesocket(Connection);
        std::cout << "Received Ctrl+C or Close Event. Cleaning up..." << std::endl;
        system("pause");
        return TRUE;

    default:
        return FALSE;
    }
}
void ClientHandler() {
    int msg_size;
    while (true) {
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, NULL);
        std::cout << msg << '\n';
        delete[] msg;
    }
}
int main(int argc, char* argv[])
{
    system("color 0A");
    /*  if (SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
          std::cout << "Set ConsoleHAndler\n";
      }
      else {
          std::cerr << "Error setting console control handler" << std::endl;
          return 1;
      }*/
    if (argc > 3) {
        Ip = argv[1];
        PortToNetwork = std::strtoul(argv[2], nullptr, 10);
        Username = argv[3];
    }
    else {
        Ip = "127.0.0.1";
        PortToNetwork = 1111;
        Username = "User";
    }
    std::cout << "Great Chat\n" << "Username: " << Username << "\nIp : " << Ip << " PortToNetwork: " << PortToNetwork << '\n';
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

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        std::cout << "Error, failed connect to server\n";
        exit(1);
    }
    std::cout << "Connected!\n";
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
    std::string msg1;
    while (true) {
        std::getline(std::cin, msg1);
        msg1 = Username + ": " + msg1;
        int msg_size = msg1.size();
        send(Connection, (char*)&msg_size, sizeof(int), NULL);
        send(Connection, msg1.c_str(), msg_size, NULL);
        Sleep(10);
    }
    system("pause");
    return 0;
}