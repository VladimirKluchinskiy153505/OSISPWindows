#include <Windows.h>
#include <iostream>
using namespace std;
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    wchar_t Args[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork2\\x64\\Debug\\NQueens.exe 15";
    // Параметры для запуска нового процесса
    LPCTSTR applicationName = NULL;  // Путь к исполняемому файлу
    LPWSTR commandLineArgs = Args;  // Аргументы командной строки (если нужно)
    LPSECURITY_ATTRIBUTES processAttributes = NULL;
    LPSECURITY_ATTRIBUTES threadAttributes = NULL;
    BOOL inheritHandles = FALSE;  // Наследовать ли дескрипторы от родительского процесса
    DWORD creationFlags = NULL;  // Флаги создания процесса
    LPVOID environment = NULL;  // Переменные окружения (обычно NULL)
    LPCTSTR currentDirectory = NULL;  // Текущий рабочий каталог (обычно NULL)
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    // Создание нового процесса
    if (CreateProcess(
        applicationName,
        commandLineArgs,
        processAttributes,
        threadAttributes,
        inheritHandles,
        creationFlags,
        environment,
        currentDirectory,
        &startupInfo,
        &processInfo
    )) {
        cout << "Process ID: " << processInfo.dwProcessId << '\n';
        cout << "Thread Id: " << processInfo.dwThreadId << '\n';
        cout << "GetProcessID: " << GetProcessId(processInfo.hProcess) << '\n';
        cout << "GetThreadID: " << GetThreadId(processInfo.hThread) << '\n';
        WaitForSingleObject(processInfo.hProcess, INFINITE);
  

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    
    }
    else {
        // Ошибка создания процесса
        DWORD error = GetLastError();
        // Обработка ошибки...
    }
    system("pause");
    return 0;
}