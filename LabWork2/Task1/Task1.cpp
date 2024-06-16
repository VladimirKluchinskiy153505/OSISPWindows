#include <Windows.h>
#include <iostream>
using namespace std;
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    wchar_t Args[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork2\\x64\\Debug\\NQueens.exe 15";
    // ��������� ��� ������� ������ ��������
    LPCTSTR applicationName = NULL;  // ���� � ������������ �����
    LPWSTR commandLineArgs = Args;  // ��������� ��������� ������ (���� �����)
    LPSECURITY_ATTRIBUTES processAttributes = NULL;
    LPSECURITY_ATTRIBUTES threadAttributes = NULL;
    BOOL inheritHandles = FALSE;  // ����������� �� ����������� �� ������������� ��������
    DWORD creationFlags = NULL;  // ����� �������� ��������
    LPVOID environment = NULL;  // ���������� ��������� (������ NULL)
    LPCTSTR currentDirectory = NULL;  // ������� ������� ������� (������ NULL)
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    // �������� ������ ��������
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
        // ������ �������� ��������
        DWORD error = GetLastError();
        // ��������� ������...
    }
    system("pause");
    return 0;
}