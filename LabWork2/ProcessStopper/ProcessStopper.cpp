#include <Windows.h>
#include <iostream>
using namespace std;
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    char chose = '0';
    cout << "To stop process press 1\n";
    cout << "To exit stopper press 0\n";
    while(true){
        cin >> chose;
        if (chose != '1') {
            break;
        }
            HWND hwnd = FindWindow(NULL, L"ProcessWindow");  // �������� �� ������ �������� ����

            if (hwnd) {
                DWORD processId;
                GetWindowThreadProcessId(hwnd, &processId);

                // �������� ���������� �������� �� ��� ��������������
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);

                if (hProcess) {
                    cout << "FOUND\n";
                    cout << "Process Id: " << hProcess << '\n';
                    // ��������� ��������� WM_CLOSE � ����
                    SendMessage(hwnd, WM_CLOSE, 0, 0);

                    // ��������� ���������� �������� (���� ��� ����������)
                    // WaitForSingleObject(hProcess, INFINITE);

                    // �������� ���������� ��������
                    CloseHandle(hProcess);
                }
                else {
                    // ��������� ������ ��� �������� ��������
                }
            }
            else {
                cout << "Not Found\n";
                // ��������� ������ ��� ������ ����
            }
    }
    
    system("pause");
    return 0;
}
