#include <iostream>
#include<Windows.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
using namespace std;

int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);

    HANDLE hFile = CreateFile(
        L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\input.txt",                // Имя файла
        GENERIC_READ,                   // Доступ для чтения
        0,                              // Не разделять файл для других процессов
        NULL,                           // Нет атрибутов безопасности
        OPEN_EXISTING,                   // Открываем существующий файл
        FILE_FLAG_OVERLAPPED,            // Флаг для асинхронного чтения
        NULL                            // Шаблон файла
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }

    // Создаем буфер для чтения данных
    const DWORD bufferSize = 8024;
    char ReadData[bufferSize];
    DWORD bytesRead;
    OVERLAPPED overlapped = { 0 };
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Начинаем асинхронное чтение
    if (!ReadFile(hFile, ReadData, bufferSize, &bytesRead, &overlapped)) {
        if (GetLastError() != ERROR_IO_PENDING) {
            std::cerr << "Failed during async read." << std::endl;
            CloseHandle(overlapped.hEvent);
            CloseHandle(hFile);
            return 1;
        }
    }

    // Ожидаем завершения асинхронной операции
    DWORD result = WaitForSingleObject(overlapped.hEvent, INFINITE);

    if (result == WAIT_OBJECT_0) {
        if (GetOverlappedResult(hFile, &overlapped, &bytesRead, FALSE)) {
            std::cout << "Async Reading successfully." << std::endl;
            std::cout << "Read " << bytesRead << " bytes of data." << std::endl;
            std::cout << "File Content: " << ReadData << std::endl;
        }
        else {
            std::cerr << "Failed when receiving results." << std::endl;
        }
    }
    else {
        std::cerr << "Failed awaiting async read." << std::endl;
    }

    // Закрываем файл и освобождаем ресурсы
    CloseHandle(overlapped.hEvent);
    CloseHandle(hFile);







    HANDLE hFile1 = CreateFile(
        L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\output.txt",                  // Имя файла
        GENERIC_WRITE,                   // Доступ для записи
        0,                               // Не разделять файл для других процессов
        NULL,                            // Нет атрибутов безопасности
        CREATE_ALWAYS,                   // Создать файл или перезаписать существующий
        FILE_FLAG_OVERLAPPED,            // Флаг для асинхронной записи
        NULL                             // Шаблон файла
    );

    if (hFile1 == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    // Создаем буфер для записи данных
    //const char* dataToWrite = "Example of async writing.";
    DWORD bytesToWrite = static_cast<DWORD>(strlen(ReadData));
    OVERLAPPED overlapped1 = { 0 };
    DWORD bytesWritten;

    // Записываем данные асинхронно
    if (!WriteFile(hFile1, ReadData, bytesToWrite, &bytesWritten, &overlapped1)) {
        if (GetLastError() != ERROR_IO_PENDING) {
            std::cerr << "Error while async wrighting." << std::endl;
            CloseHandle(hFile1);
            return 1;
        }
    }

    // Дожидаемся завершения асинхронной операции
    DWORD result1 = WaitForSingleObject(hFile1, INFINITE);

    if (result == WAIT_OBJECT_0) {
        std::cout << "Async righting successfully." << std::endl;
    }
    else {
        std::cerr << "Error while Async righting" << std::endl;
    }

    // Закрываем файл и освобождаем ресурсы
    CloseHandle(hFile1);
    return 0;
}

