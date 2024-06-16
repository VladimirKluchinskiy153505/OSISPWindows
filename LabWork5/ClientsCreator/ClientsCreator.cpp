#include <iostream>
#include<windows.h>
using namespace std;
void StartProcess() {
    wchar_t Args1[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork5\\x64\\Debug\\Client.exe";
    //wchar_t Args[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork2\\x64\\Debug\\NQueens.exe 15";
    // ��������� ��� ������� ������ ��������
    LPCTSTR applicationName = NULL;  // ���� � ������������ �����
    LPWSTR commandLineArgs = Args1;  // ��������� ��������� ������ (���� �����)
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
        /* HANDLE H_PROCESS = processInfo.hProcess;
         HANDLE H_THREAD = processInfo.hThread;*/
        cout << "New client with process_ID: " << processInfo.dwProcessId << " successfully created\n";
    }
    else {
        cerr << "Error while creating client\n";
    }
}
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
    size_t count = 1;
    cout << "Enter amount of clients\n";
    cin >> count;
    while (count--) {
        StartProcess();
    }
    return 0;
}

