#include <iostream>
#include<string>
//#include<thread>
#include <Windows.h>
#include "SoftwareDefinition.h"
using namespace std;
bool EndProcess = true;
HANDLE H_PROCESS;
HANDLE H_THREAD;
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){

    WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", SoftwareMainProcedure);
    if (!RegisterClassW(&SoftwareMainClass)) {
        return -1;
    }
    MSG SoftwareMainMessage = { 0 };
    CreateWindow(L"MainWndClass", L"ProcessWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1024, 768, NULL, NULL, NULL, NULL);
    while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL)) {
        TranslateMessage(&SoftwareMainMessage);
        DispatchMessage(&SoftwareMainMessage);
    }
    return (int)SoftwareMainMessage.wParam;
}
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
    WNDCLASS NWC = { 0 };
    NWC.hIcon = Icon;
    NWC.hCursor = Cursor;
    NWC.hInstance = hInst;
    NWC.lpszClassName = Name;
    NWC.hbrBackground = BGColor;
    NWC.lpfnWndProc = Procedure;
    return NWC;
}
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CLOSE:
        DropProcess1();
        //PostQuitMessage(0);
    case WM_COMMAND:
        switch (wp) {
        case OnButtonClicked:
            //if (EndProcess) {
                //EndProcess = false;
                StartProcess();
            //}
           // else {
             //   MessageBoxA(hWnd, "Error", "Process is already start", MB_OK);
           // }
            //SetWindowTextA(hEditControl, "");
            //MessageBoxA(hWnd, "Thanks", "Button clicked", MB_OK);
            break;
        case OnMenuAction1:
            MessageBoxA(hWnd, "Menu 1 was clicked", "Menu worked", MB_OK);
            break;
        case OnMenuAction2:
            MessageBoxA(hWnd, "Menu 2 was clicked", "Menu worked", MB_OK);
            break;
        case OnMenuAction3:
            MessageBoxA(hWnd, "Menu 3 was clicked", "Menu worked", MB_OK);
            break;
        case OnExitSoftware:
            PostQuitMessage(0);
        default: break;
        }
        break;
    case WM_CREATE:
        MainWndAddMenus(hWnd);
        MainWndAddWidgets(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default: return DefWindowProc(hWnd, msg, wp, lp);
    }
}
void MainWndAddMenus(HWND hWnd) {
    HMENU RootMenu = CreateMenu();

    HMENU SubMenu = CreateMenu();
    HMENU SubActionMenu = CreateMenu();
    AppendMenu(SubActionMenu, MF_STRING, OnMenuAction1, L"Menu 1");
    AppendMenu(SubActionMenu, MF_STRING, OnMenuAction2, L"Menu 2");
    AppendMenu(SubActionMenu, MF_STRING, OnMenuAction3, L"Menu 3");
    
    AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)SubActionMenu, L"Action");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Exit");

    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
    SetMenu(hWnd, RootMenu);
}
void MainWndAddWidgets(HWND hWnd) {
    CreateWindowA("static", "hello window!", WS_VISIBLE | WS_CHILD|ES_CENTER, 5, 5, 490, 200, hWnd, NULL, NULL, NULL);
   hEditControl= CreateWindowA("edit", "hello window!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 30, 490, 40, hWnd, NULL, NULL, NULL);
   hErrorControl = CreateWindowA("edit", "hello window!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 70, 490, 40, hWnd, NULL, NULL, NULL);
    CreateWindowA("button", "Start", WS_VISIBLE | WS_CHILD|ES_CENTER, 5, 110, 120, 20, hWnd, (HMENU)OnButtonClicked, NULL, NULL);
}
void WaitForProcessCompletion(HANDLE hProcess) {
    WaitForSingleObject(hProcess, INFINITE);
    // ������� ������� ��������
    EndProcess = true;
}
void DropProcess() {
    HANDLE Old_H_PROCESS = H_PROCESS;
    HANDLE Old_H_THREAD = H_THREAD;
    if (H_PROCESS) {
        StartProcess();
        // �������� ���������� ��������
        if (TerminateProcess(Old_H_PROCESS, 0)) {
            SetWindowTextA(hErrorControl, ("Success"));
            // ������� ������� �������
        }
        else {
            // ������ ��� ���������� ��������
            DWORD error = GetLastError();
            SetWindowTextA(hErrorControl, ("Error : " + to_string(error)).c_str());
            // ��������� ������...
        }

        // �������� ���������� ��������
        CloseHandle(Old_H_PROCESS);
        CloseHandle(Old_H_THREAD);
    }
    else {
        // ������ ��� �������� ��������
        DWORD error = GetLastError();
        SetWindowTextA(hErrorControl, ("Error : " + to_string(error)).c_str());
        // ��������� ������...
    }
}
void DropProcess1() {
    if (H_PROCESS) {
        // �������� ���������� ��������
        if (TerminateProcess(H_PROCESS, 0)) {
            SetWindowTextA(hErrorControl, ("Success"));
            // ������� ������� �������
        }
        else {
            // ������ ��� ���������� ��������
            DWORD error = GetLastError();
            SetWindowTextA(hErrorControl, ("Error : " + to_string(error)).c_str());
            // ��������� ������...
        }

        // �������� ���������� ��������
        CloseHandle(H_PROCESS);
        CloseHandle(H_THREAD);
        StartProcess();
    }
    else {
        // ������ ��� �������� ��������
        DWORD error = GetLastError();
        SetWindowTextA(hErrorControl, ("Error : " + to_string(error)).c_str());
        // ��������� ������...
    }
}
void StartProcess() {
    wchar_t Args1[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork2\\x64\\Debug\\FindPrimeNumbers.exe 1000000000000000";
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
        H_PROCESS = processInfo.hProcess;
        H_THREAD = processInfo.hThread;
        SetWindowTextA(hErrorControl,"");
        SetWindowTextA(hEditControl, ("Process ID : " + to_string(processInfo.dwProcessId)+'\n'+
            "Thread Id: "+to_string(processInfo.dwThreadId)+'\n'+
                "GetProcessID: "+to_string(GetProcessId(processInfo.hProcess)) + '\n'+
                "GetThreadID: " +to_string(GetThreadId(processInfo.hThread)) + '\n').c_str());

        //std::thread processThread(WaitForProcessCompletion, processInfo.hProcess);
       // DWORD waitResult = WaitForSingleObject(processInfo.hProcess, INFINITE);
        //if (waitResult == WAIT_OBJECT_0) {
        //    // ������� ������� ��������
        //    end_process = true;
        //}
        //else {
        //    // ������ �������� ��� ������� �� ��� ������� ��������
        //    DWORD error = GetLastError();
        //    // ��������� ������...
        //}
        //CloseHandle(processInfo.hProcess);
        //CloseHandle(processInfo.hThread);

        // ������� ���������� ������, ������� ���� ���������� ��������
        //processThread.join();

    }
    else {
        // ������ �������� ��������
        DWORD error = GetLastError();
        // ��������� ������...
    }
    //system("pause");
}