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
            HWND hwnd = FindWindow(NULL, L"ProcessWindow");  // Замените на нужное название окна

            if (hwnd) {
                DWORD processId;
                GetWindowThreadProcessId(hwnd, &processId);

                // Получите дескриптор процесса по его идентификатору
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);

                if (hProcess) {
                    cout << "FOUND\n";
                    cout << "Process Id: " << hProcess << '\n';
                    // Отправьте сообщение WM_CLOSE в окно
                    SendMessage(hwnd, WM_CLOSE, 0, 0);

                    // Дождитесь завершения процесса (если это необходимо)
                    // WaitForSingleObject(hProcess, INFINITE);

                    // Закройте дескриптор процесса
                    CloseHandle(hProcess);
                }
                else {
                    // Обработка ошибки при открытии процесса
                }
            }
            else {
                cout << "Not Found\n";
                // Обработка ошибки при поиске окна
            }
    }
    
    system("pause");
    return 0;
}
