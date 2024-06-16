#pragma once
#define OnMenuAction1 1
#define OnMenuAction2 2
#define OnMenuAction3 3
#define OnExitSoftware 4
#define OnCreateServerClicked 5
#define OnTerminateClientClicked 6

HWND CleanButton;
HWND ReadPortData;
HWND ReadHost;
HWND CreateServerButton;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void MainWndAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void Main();
void CleanWidget(HWND hwnd);
void CreateServer();
