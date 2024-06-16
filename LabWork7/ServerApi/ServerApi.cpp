#include <iostream>
#include<string>
#include <Windows.h>
#include "SoftwareDefinition.h"
HANDLE C_PROCESS, C_THREAD;
const size_t BufferSize = 3000, PortBufferSize = 255, NameBufferSize = 255, HostBufferSize = 8;
char Buffer[BufferSize];
char PortBuffer[PortBufferSize];
char HostBuffer[HostBufferSize];
void SetTextToWidget(HWND hWidget, std::string text) {
    SetWindowTextA(hWidget, "");
    SetWindowTextA(hWidget, text.c_str());
}
void AddTextToWidget(HWND hWidget, std::string&& textToAdd) {
    int textLength = GetWindowTextA(hWidget, Buffer, BufferSize);
    std::string currentText(Buffer);
    currentText += "\r\n" + textToAdd;
    SetWindowTextA(hWidget, currentText.c_str());
}
void CleanWidget(HWND hWidget) {
    SetTextToWidget(hWidget, "");
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", SoftwareMainProcedure);
    if (!RegisterClassW(&SoftwareMainClass)) {
        return -1;
    }
    MSG SoftwareMainMessage = { 0 };
    CreateWindow(L"MainWndClass", L"ProcessWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 250, 250, NULL, NULL, NULL, NULL);
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
        case OnCreateServerClicked:
            EnableWindow(CreateServerButton, FALSE);
            CreateServer();
            EnableWindow(CreateServerButton, TRUE);
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
    ReadPortData = CreateWindowA("edit", "127.0.0.1", WS_VISIBLE | WS_CHILD, 5, 5, 120, 20, hWnd, NULL, NULL, NULL);//x y width height
    ReadHost = CreateWindowA("edit", "1111", WS_VISIBLE | WS_CHILD, 5, 45, 120, 20, hWnd, NULL, NULL, NULL);
    CreateServerButton = CreateWindowA("button", "StartServer", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 85, 120, 20, hWnd, (HMENU)OnCreateServerClicked, NULL, NULL);
}
void CreateServer() {
    wchar_t Filename[] = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork7\\x64\\Debug\\Server.exe";
    GetWindowTextA(ReadPortData, PortBuffer, PortBufferSize);
    GetWindowTextA(ReadHost, HostBuffer, HostBufferSize);

    std::wstring filename(Filename);
    filename += L" ";

    std::wstring port;
    std::copy(PortBuffer, PortBuffer + strlen(PortBuffer), back_inserter(port));
    port += L" ";

    std::wstring host;
    std::copy(HostBuffer, HostBuffer + strlen(HostBuffer), back_inserter(host));
    host += L" ";


    std::wstring Args = filename + port + host;
    // Параметры для запуска нового процесса
    LPCTSTR applicationName = NULL;  // Путь к исполняемому файлу
    LPWSTR commandLineArgs = (wchar_t*)Args.c_str();  // Аргументы командной строки (если нужно)
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
    }
    else {
        // Ошибка создания процесса
        DWORD error = GetLastError();
        // Обработка ошибки...
    }
}
void Main() {
    C_PROCESS = GetCurrentProcess();
    C_THREAD = GetCurrentThread();
    SetPriorityClass(C_PROCESS, REALTIME_PRIORITY_CLASS);
    SetThreadPriority(C_THREAD, THREAD_PRIORITY_TIME_CRITICAL);
}
