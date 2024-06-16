#pragma once
#define OnExitSoftware 1
#define OnCleanClicked 2
#define OnReadClicked 3
#define OnGenerateClicked 4
#define OnTerminateClicked 5
#define OnNewClientClicked 6

HWND hEditControl;
HWND hInfoControl;
HWND GenerateButton;
HWND TerminateButton;
HWND ReadClientCountField;
HWND NewClientButton;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void MainWndAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void Main();
void CleanWidget(HWND hwnd);
void StartProcess();
void GenerateClients();
void TerminateClients();
