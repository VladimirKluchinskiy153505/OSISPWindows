#include <iostream>
#include<chrono>
#include<Windows.h>
#include <openssl/evp.h>
#include<openssl/conf.h>
#include<vector>
//int EVP_EncryptUpdate(EVP_CIPHER_CTX* ctx, unsigned char* out, int* outl, const unsigned char* in, int inl);
//int EVP_EncryptFinal(EVP_CIPHER_CTX* ctx, unsigned char* out, int* outl);
using namespace std;
size_t Parts = 3;
size_t* Chipher_Lens;
size_t* Decrypt_Lens;
int encrypt(unsigned char* text, int text_len, unsigned char* key, unsigned char* cipher) {
    int cipher_len = 0;
    int len = 0;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cerr << "CIPHER_NEW FAILED\n";
        exit(-1);
    }
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) {
        cerr << "ERROR EncryptInitEx failed\n";
        exit(-1);
    }
    if (!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len)) {
        cerr << "Encrypt update failed\n";
    }
    cipher_len += len;
    if (!EVP_EncryptFinal_ex(ctx, cipher + len, &len)) {
        cerr << "Encrypt final failed\n";
        exit(-1);
    }
    cipher_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return cipher_len;
}
int decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text) {
    int text_len = 0;
    int len = 0;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cerr << "CIPHER_NEW FAILED\n";
        exit(-1);
    }
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) {
        cerr << "ERROR DecryptInitEx failed\n";
        exit(-1);
    }
    if (!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len)) {
        cerr << "Decrypt update failed\n";
    }
    text_len += len;
    if (!EVP_DecryptFinal_ex(ctx, text + len, &len)) {
        cerr << "Decrypt final failed\n";
        exit(-1);
    }
    text_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return text_len;
}
//int EVP_EncryptUpdate(EVP_CIPHER_CTX* ctx, unsigned char* out, int* outl, const unsigned char* in, int inl);
//int EVP_EncryptFinal(EVP_CIPHER_CTX* ctx, unsigned char* out, int* outl);
//BOOL ReadFile(
//    [in]                HANDLE       hFile,
//    [out]               LPVOID       lpBuffer,
//    [in]                DWORD        nNumberOfBytesToRead,
//    [out, optional]     LPDWORD      lpNumberOfBytesRead,
//    [in, out, optional] LPOVERLAPPED lpOverlapped
//);
struct EncryptData {
    size_t num = 0;
    unsigned char* text;
    int text_len;
    unsigned char* key;
    unsigned char* cipher;
};
struct DecryptData {
    size_t num = 0;
    unsigned char* cipher;
    int cipher_len;
    unsigned char* key;
    unsigned char* text;
};
DWORD WINAPI ThreadEncrypt(LPVOID lpParam) {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    EncryptData* threadData = static_cast<EncryptData*>(lpParam);
    Chipher_Lens[threadData->num] = encrypt(threadData->text, threadData->text_len, threadData->key, threadData->cipher);
    cout <<"Thread with id: "<< GetCurrentThreadId() << " made cipher with length: " << Chipher_Lens[threadData->num] << '\n';
    delete threadData;
    return 0;
}
DWORD WINAPI ThreadDecrypt(LPVOID lpParam) {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    DecryptData* threadData = static_cast<DecryptData*>(lpParam);
    Decrypt_Lens[threadData->num] = decrypt(threadData->cipher, threadData->cipher_len, threadData->key, threadData->text);
    cout << "Thread with id: " << GetCurrentThreadId() << " decrypt cipher with length: " << Decrypt_Lens[threadData->num] << '\n';
    delete threadData;
    return 0;
}
//HANDLE hInputFile = CreateFile(
    //    inputFile,                // Имя файла
    //    GENERIC_READ,                   // Доступ для чтения
    //    0,                              // Не разделять файл для других процессов
    //    NULL,                           // Нет атрибутов безопасности
    //    OPEN_EXISTING,                   // Открываем существующий файл
    //    FILE_FLAG_OVERLAPPED,            // Флаг для асинхронного чтения
    //    NULL                            // Шаблон файла
    //);
//BOOL ReadFile(
//    [in]                HANDLE       hFile,
//    [out]               LPVOID       lpBuffer,
//    [in]                DWORD        nNumberOfBytesToRead,
//    [out, optional]     LPDWORD      lpNumberOfBytesRead,
//    [in, out, optional] LPOVERLAPPED lpOverlapped
//);
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    const wchar_t* inputFile = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\inputr.txt";
    const wchar_t* inputFile1 = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\inputr1.txt";
    const wchar_t* inputFile2 = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\inputr2.txt";

    const wchar_t* outputFile = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\output.txt";
    const wchar_t* outputFile1 = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\output1.txt";
    const wchar_t* outputFile2 = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\output2.txt";

    const wchar_t* decryptOutputFile = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\decrypted.txt";
    const wchar_t* decryptOutputFile1 = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\decrypted1.txt";
    const wchar_t* decryptOutputFile2 = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork4\\decrypted2.txt";
    unsigned char* key = (unsigned char*)"012345678abcdef";

    HANDLE* hInputFiles = new HANDLE[Parts];
    hInputFiles[0] = CreateFile(inputFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    hInputFiles[1] = CreateFile(inputFile1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    hInputFiles[2] = CreateFile(inputFile2, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

    HANDLE* hOutputFiles = new HANDLE[Parts];
    hOutputFiles[0] = CreateFile(outputFile, GENERIC_WRITE| GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
    hOutputFiles[1] = CreateFile(outputFile1, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
    hOutputFiles[2] = CreateFile(outputFile2, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

    HANDLE* hDecryptOutputFiles = new HANDLE[Parts];
    hDecryptOutputFiles[0] = CreateFile(decryptOutputFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
    hDecryptOutputFiles[1] = CreateFile(decryptOutputFile1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
    hDecryptOutputFiles[2] = CreateFile(decryptOutputFile2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

    for (size_t i = 0; i < Parts; ++i) {
        if (hInputFiles[i] == INVALID_HANDLE_VALUE || hOutputFiles[i] == INVALID_HANDLE_VALUE || hDecryptOutputFiles[i] == INVALID_HANDLE_VALUE) {
            cerr << "Failed to open file\n";
            exit(1);
        }
    }
    // Создаем буферы для чтения данных
    const DWORD textBufferSize = 7000000;
    unsigned char** readData = new unsigned char* [Parts];
    for (size_t i = 0; i < Parts; ++i) {
        readData[i] = new unsigned char[textBufferSize];
    }

    unsigned char** ciphers = new unsigned char* [Parts];
    for (size_t i = 0; i < Parts; ++i) {
        ciphers[i] = new unsigned char[textBufferSize + EVP_MAX_BLOCK_LENGTH];
    }

    unsigned char** decryptCiphers = new unsigned char* [Parts];
    for (size_t i = 0; i < Parts; ++i) {
        decryptCiphers[i] = new unsigned char[textBufferSize + EVP_MAX_BLOCK_LENGTH];
    }

    DWORD* bytesRead = new DWORD[Parts];
    DWORD* bytesWritten = new DWORD[Parts];
    DWORD* bytesDecryptWritten = new DWORD[Parts];
    
    Chipher_Lens = new size_t[Parts];
    Decrypt_Lens = new size_t[Parts];

    OVERLAPPED* readOverlapped = new OVERLAPPED[Parts]{ 0 };
    for (size_t i = 0; i < Parts; ++i) {
        readOverlapped[i].hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    OVERLAPPED* writeOverlapped = new OVERLAPPED[Parts]{ 0 };
    for (size_t i = 0; i < Parts; ++i) {
        writeOverlapped[i].hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    OVERLAPPED* writeDecryptOverlapped = new OVERLAPPED[Parts]{ 0 };
    for (size_t i = 0; i < Parts; ++i) {
        writeDecryptOverlapped[i].hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    // Начинаем асинхронное чтение
    for (size_t i = 0; i < Parts; ++i) {
        if (ReadFile(hInputFiles[i], readData[i], textBufferSize, &bytesRead[i], &readOverlapped[i])) {
            std::cout << "File was read synchroniously with " << bytesRead[i] << " bytes\n";
        }
    }
    for (size_t i = 0; i < Parts; ++i) {
       if (WaitForSingleObject(readOverlapped[i].hEvent, INFINITE) == WAIT_OBJECT_0) {
        if (GetOverlappedResult(hInputFiles[i], &readOverlapped[i], &bytesRead[i], TRUE)) {
            std::cout << "File was read asynchroniosly with " << bytesRead[i] << " bytes\n";
        }
        }
        else {
            cerr << "Error while awaiting ending of file\n";
        }
        //CloseHandle(readOverlapped[i].hEvent);
        CloseHandle(hInputFiles[i]);
    }

    std::cout << "\n<--------Started Encription process-------->\n";
    vector<HANDLE> encrypt_threads(Parts);
    for (size_t i = 0; i < Parts; ++i) {//ждём прочтения и запускаем encrypt
        int text_len = strlen((const char*)readData[i]);
        EncryptData* threadData = new EncryptData{i, readData[i], text_len, key,ciphers[i]};
        encrypt_threads[i] = CreateThread(NULL, 0, ThreadEncrypt, threadData, 0, NULL);
    }

    for (size_t i = 0; i < Parts; ++i) {//запись шифра в файл
        WaitForSingleObject(encrypt_threads[i], INFINITE);
        CloseHandle(encrypt_threads[i]);
    }
    for (size_t i = 0; i < Parts; ++i) {
        //DWORD bytesToWrite = static_cast<DWORD>(strlen((const char*)ciphers[i]));
        if (WriteFile(hOutputFiles[i], ciphers[i], Chipher_Lens[i], &bytesWritten[i], &writeOverlapped[i])) {
            std::cout << "File was written synchroniously\n";
        }
    }
    for (size_t i = 0; i < Parts; ++i) {
        if (WaitForSingleObject(writeOverlapped[i].hEvent, INFINITE) == WAIT_OBJECT_0){
            GetOverlappedResult(hInputFiles[i], &writeOverlapped[i], &bytesWritten[i], TRUE);
            std::cout << "File was write asynchroniosly with "<<bytesWritten[i]<<" bytes\n";
        }
        else {
            cerr << "Error of Async write " << GetLastError() << std::endl;
        }
        CloseHandle(writeOverlapped[i].hEvent);
        //CloseHandle(hOutputFiles[i]);
    }
    std::cout << "\n<--------Extracting Cipher From File-------->\n";

    // Начинаем асинхронное чтение
    for (size_t i = 0; i < Parts; ++i) {
        if (ReadFile(hOutputFiles[i], readData[i], textBufferSize, &bytesRead[i], &readOverlapped[i])) {
            std::cout << "Cipher was read synchroniously with " << bytesRead[i] << " bytes\n";
        }
    }
    for (size_t i = 0; i < Parts; ++i) {
        if (WaitForSingleObject(readOverlapped[i].hEvent, INFINITE) == WAIT_OBJECT_0) {
            if (GetOverlappedResult(hInputFiles[i], &readOverlapped[i], &bytesRead[i], TRUE)) {
               std::cout << "Cipher was read asynchroniosly with " << bytesRead[i] << " bytes\n";
            }
        }
        else {
            cerr << "Error while awaiting ending of file\n";
        }
        CloseHandle(readOverlapped[i].hEvent);
        CloseHandle(hOutputFiles[i]);
    }

    
    std::cout << "\n<--------Started Decription process-------->\n";
     vector<HANDLE> decrypt_threads(Parts);
    for (size_t i = 0; i < Parts; ++i) {
        int cipher_len = Chipher_Lens[i];
            DecryptData* threadData = new DecryptData{ i, readData[i], cipher_len, key, decryptCiphers[i]};
            decrypt_threads[i] = CreateThread(NULL, 0, ThreadDecrypt, threadData, 0, NULL);
    }

    for (size_t i = 0; i < Parts; ++i) {//запись расшифровки в файл
        WaitForSingleObject(decrypt_threads[i], INFINITE);
        CloseHandle(decrypt_threads[i]);
    }


    for (size_t i = 0; i < Parts; ++i) {
        //DWORD bytesToWrite = static_cast<DWORD>(strlen((const char*)ciphers[i]));
        if (WriteFile(hDecryptOutputFiles[i], decryptCiphers[i], Decrypt_Lens[i], &bytesDecryptWritten[i], &writeDecryptOverlapped[i])) {
            cout << "File was written synchroniosly.\n";
        }
    }
    for (size_t i = 0; i < Parts; ++i) {
        //DWORD bytesToWrite = static_cast<DWORD>(strlen((const char*)ciphers[i]));
        if (WaitForSingleObject(writeDecryptOverlapped[i].hEvent, INFINITE) == WAIT_OBJECT_0) {
            GetOverlappedResult(hDecryptOutputFiles[i], &writeDecryptOverlapped[i], &bytesDecryptWritten[i], TRUE);
            cout << "File was written asynchroniosly with " << bytesDecryptWritten[i]<<" bytes \n";
        }
        else {
            cerr << "Error while getting result " << GetLastError() << std::endl;
        }
                
        CloseHandle(writeDecryptOverlapped[i].hEvent);
        CloseHandle(hDecryptOutputFiles[i]);
    }
    delete[] Chipher_Lens;
    delete[] Decrypt_Lens;

    delete[] bytesRead;
    delete[] bytesWritten;
    delete[] bytesDecryptWritten;

    delete[] readOverlapped;
    delete[] writeOverlapped;
    delete[] writeDecryptOverlapped;

    for (size_t i = 0; i < Parts; ++i) {
        delete[] readData[i];
    }
    delete[] readData;

    for (size_t i = 0; i < Parts; ++i) {
        delete[] ciphers[i];
    }
    delete[] ciphers;

    for (size_t i = 0; i < Parts; ++i) {
        delete[] decryptCiphers[i];
    }
    delete[] decryptCiphers;
    return 0;
}