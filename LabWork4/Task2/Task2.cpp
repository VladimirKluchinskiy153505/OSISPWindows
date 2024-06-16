#include <iostream>
#include <windows.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

int main() {
    // Инициализация OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    OPENSSL_config(NULL);

    // Открываем входной и выходной файлы
    const wchar_t* inputFile = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\input1.txt";
    const wchar_t* outputFile = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\output1.txt";

    HANDLE hInputFile = CreateFile(inputFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    HANDLE hOutputFile = CreateFile(outputFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

    if (hInputFile == INVALID_HANDLE_VALUE || hOutputFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Не удалось открыть файлы." << std::endl;
        return 1;
    }

    // Создаем OpenSSL структуры для шифрования
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    // Инициализация шифра (AES-256-CBC)
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, NULL, NULL)) {
        handleErrors();
    }

    // Определяем буферы для чтения и записи данных
    const size_t bufferSize = 4096;
    unsigned char* readBuffer = (unsigned char*)malloc(bufferSize);
    unsigned char* encryptedBuffer = (unsigned char*)malloc(bufferSize + EVP_MAX_BLOCK_LENGTH);

    DWORD bytesRead, bytesWritten;
    OVERLAPPED readOverlapped = { 0 };
    readOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    OVERLAPPED writeOverlapped = { 0 };
    writeOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    while (true) {
        if (!ReadFile(hInputFile, readBuffer, bufferSize, &bytesRead, &readOverlapped)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                std::cerr << "Ошибка при асинхронном чтении из файла." << std::endl;
                break;
            }
        }

        if (!GetOverlappedResult(hInputFile, &readOverlapped, &bytesRead, TRUE)) {
            std::cerr << "Ошибка при получении результата асинхронного чтения." << std::endl;
            break;
        }

        if (bytesRead == 0) {
            // Достигнут конец файла
            break;
        }

        // Шифруем данные
        int outLength = 0;
        if (1 != EVP_EncryptUpdate(ctx, encryptedBuffer, &outLength, readBuffer, bytesRead)) {
            handleErrors();
        }

        if (!WriteFile(hOutputFile, encryptedBuffer, outLength, &bytesWritten, &writeOverlapped)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                std::cerr << "Ошибка при асинхронной записи в файл." << std::endl;
                break;
            }
        }

        if (!GetOverlappedResult(hOutputFile, &writeOverlapped, &bytesWritten, TRUE)) {
            std::cerr << "Ошибка при получении результата асинхронной записи." << std::endl;
            break;
        }
    }

    // Финализация шифра
    int outLength = 0;
    if (1 != EVP_EncryptFinal_ex(ctx, encryptedBuffer, &outLength)) {
        handleErrors();
    }

    if (!WriteFile(hOutputFile, encryptedBuffer, outLength, &bytesWritten, &writeOverlapped)) {
        if (GetLastError() != ERROR_IO_PENDING) {
            std::cerr << "Ошибка при асинхронной записи в файл." << std::endl;
        }
    }

    if (!GetOverlappedResult(hOutputFile, &writeOverlapped, &bytesWritten, TRUE)) {
        std::cerr << "Ошибка при получении результата асинхронной записи." << std::endl;
    }

    // Очистка и освобождение ресурсов
    EVP_CIPHER_CTX_free(ctx);
    CloseHandle(hInputFile);
    CloseHandle(hOutputFile);
    free(readBuffer);
    free(encryptedBuffer);
    ERR_free_strings();

    return 0;
}