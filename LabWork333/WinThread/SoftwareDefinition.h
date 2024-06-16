#pragma once
#define OnMenuAction1 1
#define OnMenuAction2 2
#define OnMenuAction3 3
#define OnExitSoftware 4
#define OnAsyncSortClicked 5
#define OnSyncSortClicked 6
#define OnCleanClicked 7
#define OnReadClicked 8
#define OnGenerateClicked 9
#define OnShowAsyncClicked 10
#define OnShowSyncClicked 11

HWND hEditControl;
HWND hInfoControl;
HWND hDataViewer;
HWND hDataViewer1;
HWND StartAsyncButton;
HWND StartSyncButton;
HWND CleanButton;
HWND GenerateButton;
HWND ReadThreadField;
HWND ReadVarCountField;
HWND ShowAsyncButton;
HWND ShowSyncButton;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
void MainWndAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void Main();
void StartSyncSortProcess();
void StartAsyncSortProcess();
void CleanWidget(HWND hwnd);
