#include <iostream>
#include<string>
#include<list>
#include <Windows.h>
#include "SoftwareDefinition.h"
using namespace std;
HANDLE C_PROCESS, C_THREAD;
const size_t BufferSize = 10;
char Buffer[BufferSize];
list<HANDLE> Clients;
void SetTextToWidget(HWND hWidget, std::string text) {
    SetWindowTextA(hWidget, "");
    SetWindowTextA(hWidget, text.c_str());
}
void AddTextToWidget(HWND hWidget, string&& textToAdd) {
    int textLength = GetWindowTextA(hWidget, Buffer, BufferSize);
    string currentText(Buffer);
    currentText += "\r\n" + textToAdd;
    SetWindowTextA(hWidget, currentText.c_str());
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", SoftwareMainProcedure);
    if (!RegisterClassW(&SoftwareMainClass)) {
        return -1;
    }
    MSG SoftwareMainMessage = { 0 };
    CreateWindow(L"MainWndClass", L"ProcessWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1600, 900, NULL, NULL, NULL, NULL);
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
            PostQuitMessage(0);
        case WM_COMMAND:
            switch (wp) {
                case OnGenerateClicked:
                    EnableWindow(GenerateButton, FALSE);
                    GenerateClients();
                    EnableWindow(GenerateButton, TRUE);
                    break;
                case OnTerminateClicked:
                    TerminateClients();
                    break;
                case OnNewClientClicked:
                    StartProcess();
                    break;
                case OnExitSoftware:
                PostQuitMessage(0);
                default: break;
            }
        break;
    case WM_CREATE:
        MainWndAddMenus(hWnd);
        MainWndAddWidgets(hWnd);
        Main();
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

    AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)SubActionMenu, L"Action");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Exit");

    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
    SetMenu(hWnd, RootMenu);
}
void MainWndAddWidgets(HWND hWnd) {
    //CreateWindowA("static", "hello window!", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 5, 490, 200, hWnd, NULL, NULL, NULL);//x y width height
    hEditControl = CreateWindowA("edit", NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 0, 600, 400, hWnd, NULL, NULL, NULL);
    hInfoControl = CreateWindowA("edit", NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 725, 0, 600, 400, hWnd, NULL, NULL, NULL);
    ReadClientCountField = CreateWindowA("edit", "1", WS_VISIBLE | WS_CHILD, 1350, 40, 120, 20, hWnd, NULL, NULL, NULL);
    GenerateButton = CreateWindowA("button", "StartClientsNumber", WS_VISIBLE | WS_CHILD | ES_CENTER, 1350, 80, 140, 20, hWnd, (HMENU)OnGenerateClicked, NULL, NULL);
    TerminateButton = CreateWindowA("button", "Terminate", WS_VISIBLE | WS_CHILD | ES_CENTER, 1350, 120, 120, 20, hWnd, (HMENU)OnTerminateClicked, NULL, NULL);
    NewClientButton = CreateWindowA("button", "AddClient", WS_VISIBLE | WS_CHILD | ES_CENTER, 1350, 160, 120, 20, hWnd, (HMENU)OnNewClientClicked, NULL, NULL);
}
//void CleanWidget(HWND hWidget) {
//    SetTextToWidget(hWidget, "");
//}
void Main() {
    C_PROCESS = GetCurrentProcess();
    C_THREAD = GetCurrentThread();
    SetPriorityClass(C_PROCESS, REALTIME_PRIORITY_CLASS);
    SetThreadPriority(C_THREAD, THREAD_PRIORITY_TIME_CRITICAL);
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();
    int processPriority = GetPriorityClass(C_PROCESS);
    int threadPriority = GetThreadPriority(C_THREAD);

    SetWindowTextA(hEditControl, ("Main Process ID : " + to_string(processId) + '\n' +
        "Main Thread Id: " + to_string(threadId) + '\n' +
        " Process Priority: " + to_string(processPriority) +
        " Thread Priority: " + to_string(threadPriority)).c_str());
}
void StartProcess() {
    wchar_t Args1[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork5\\x64\\Debug\\Client.exe";
    //wchar_t Args[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork2\\x64\\Debug\\NQueens.exe 15";
    // Параметры для запуска нового процесса
    LPCTSTR applicationName = NULL;  // Путь к исполняемому файлу
    LPWSTR commandLineArgs = Args1;  // Аргументы командной строки (если нужно)
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
        HANDLE client = processInfo.hProcess;
        Clients.push_back(client);
        /* HANDLE H_PROCESS = processInfo.hProcess;
         HANDLE H_THREAD = processInfo.hThread;*/
        AddTextToWidget(hEditControl, "New client with process_ID: " +to_string(processInfo.dwProcessId) + "successfully created");
    }
    else {
        AddTextToWidget(hEditControl, "Error while creating client");
    }
}
void GenerateClients()
{
    size_t num = 1;
    GetWindowTextA(ReadClientCountField, Buffer, BufferSize);
    num = stoul(Buffer);
    while (num--) {
        StartProcess();
    }
}
void TerminateClients() {
    for (auto& now : Clients) {
        TerminateProcess(now, 0);
        CloseHandle(now);
    }
    Clients.clear();
}
