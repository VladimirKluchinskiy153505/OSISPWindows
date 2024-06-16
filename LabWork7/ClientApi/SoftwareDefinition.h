#pragma once
#define OnMenuAction1 1
#define OnMenuAction2 2
#define OnMenuAction3 3
#define OnExitSoftware 4
#define OnCreateClientClicked 5
#define OnTerminateClientClicked 6

HWND hEditControl;
HWND hInfoControl;
HWND hDataViewer;
HWND hDataViewer1;
HWND CleanButton;
HWND ReadPortData;
HWND ReadUsername;
HWND ReadHost;
HWND CreateUserButton;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void MainWndAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void Main();
void CleanWidget(HWND hwnd);
void CreateClient();
