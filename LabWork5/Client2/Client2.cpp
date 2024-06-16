#include <windows.h> 
#include<iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#define BUFSIZE 2048
using namespace std;
DWORD WINAPI InstanceThreadRead(LPVOID lpvParam);
DWORD WINAPI InstanceThreadWrite(LPVOID lpvParam);
int main(int argc, TCHAR* argv[])
{
    DWORD  dwThreadId = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "Client with Id:" << GetCurrentProcessId() << '\n';
    cout << "To know result press 0\n";
    HANDLE hPipe;
    const wchar_t* lpvMessage = TEXT("Default message from client.");
    //LPTSTR lpvMessage = "Default message from client.";
    TCHAR  chBuf[BUFSIZE];
    BOOL   fSuccess = FALSE;
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    const wchar_t* lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
    if (argc > 1)
        lpvMessage = argv[1];

    // Try to open a named pipe; wait for it, if necessary.
    while (true)
    {
        hPipe = CreateFile(
            lpszPipename,   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE,
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 

        // Break if the pipe handle is valid. 

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        // Exit if an error other than ERROR_PIPE_BUSY occurs. 

        if (GetLastError() != ERROR_PIPE_BUSY)
        {
            _tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
            return -1;
        }

        // All pipe instances are busy, so wait for 20 seconds. 

        if (!WaitNamedPipe(lpszPipename, 20000))
        {
            printf("Could not open pipe: 20 second wait timed out.");
            return -1;
        }
        // The pipe connected; change to message-read mode. 

        dwMode = PIPE_READMODE_MESSAGE;
        fSuccess = SetNamedPipeHandleState(
            hPipe,    // pipe handle 
            &dwMode,  // new pipe mode 
            NULL,     // don't set maximum bytes 
            NULL);    // don't set maximum time 
        if (!fSuccess)
        {
            _tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
            return -1;
        }
    }
    HANDLE hThreadRead = CreateThread(
        NULL,              // no security attribute 
        0,                 // default stack size 
        InstanceThreadRead,    // thread proc
        (LPVOID)hPipe,    // thread parameter 
        0,                 // not suspended 
        &dwThreadId);      // returns thread ID 
    while (true) {
        std::wstring message;
        std::wcout << L"Enter String:\n";
        std::getline(std::wcin, message);
        // Send a message to the pipe server. 
        cbToWrite = (message.size() + 1) * sizeof(wchar_t);
        //_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, message);
        cout << "Sending " << cbToWrite << " byte message: "; wcout << message << '\n';
        fSuccess = WriteFile(
            hPipe,                  // pipe handle 
            message.c_str(),             // message 
            cbToWrite,              // message length 
            &cbWritten,             // bytes written 
            NULL);                  // not overlapped 

        if (!fSuccess)
        {
            _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
            return -1;
        }
        printf("\nMessage sent to server, receiving reply as follows:\n");
    }
    printf("\n<End of message, press ENTER to terminate connection and exit>");
    _getch();
    CloseHandle(hPipe);
}
DWORD WINAPI InstanceThreadRead(LPVOID lpvParam)
{
    BOOL   fSuccess = FALSE;
    TCHAR  chBuf[BUFSIZE];
    DWORD  cbRead;
   HANDLE hPipe = (HANDLE)lpvParam;
   DWORD bytesAvailable = 0;
   while (true) {
       if (PeekNamedPipe(hPipe, nullptr, 0, nullptr, &bytesAvailable, nullptr) && bytesAvailable > 0) {

           do
           {
               // Read from the pipe. 
               fSuccess = ReadFile(
                   hPipe,    // pipe handle 
                   chBuf,    // buffer to receive reply 
                   BUFSIZE * sizeof(TCHAR),  // size of buffer 
                   &cbRead,  // number of bytes read 
                   NULL);    // not overlapped 

               if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
                   break;

               _tprintf(TEXT("\"%s\"\n"), chBuf);
           } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

           if (!fSuccess)
           {
               _tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
               return -1;
           }
       }
   }
}

  