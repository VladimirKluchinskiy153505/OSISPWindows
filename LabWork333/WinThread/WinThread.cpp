#include <iostream>
#include<chrono>
#include <random>
#include<string>
#include<fstream>
#include <Windows.h>
#include "SoftwareDefinition.h"
#include "test_vars.h"
using namespace std;
HANDLE C_PROCESS, C_THREAD;
const size_t BufferSize = 3000, ThreadNumBufferSize = 4, VarNumBufferSize=20;
char Buffer[BufferSize];
char ThreadNumBuffer[ThreadNumBufferSize];
char VarNumBuffer[VarNumBufferSize];
vector<int> Vector, AS_Vector, S_Vector;
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
void display_vec(vector<int>& vec) {
    string text;
    for (auto& now : vec) {
        text += to_string(now) + " ";
    }
    SetWindowTextA(hDataViewer, text.c_str());
}
void display_vec1(vector<int>& vec) {
    string text;
    for (auto& now : vec) {
        text += to_string(now) + " ";
    }
    SetWindowTextA(hDataViewer1, text.c_str());
}
void GenerateNumbers(vector<int>& vec) {
    size_t num = 1;
    GetWindowTextA(ReadVarCountField, VarNumBuffer, VarNumBufferSize);
    try {
        num = stoul(VarNumBuffer);
        AddTextToWidget(hEditControl, "Generating " + to_string(num) + " Random Numbers");
        vec.clear();
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(1, num); // Здесь 1 и 1000 - диапазон случайных чисел
        vec.resize(num);
        for (int i = 0; i < num; ++i) {
            vec[i] = dis(gen);
        }
    }
    catch (std::exception& ex) {
        AddTextToWidget(hEditControl, ex.what());
    }
    AddTextToWidget(hEditControl, "Successfully Generated " + to_string(num) + " Random Numbers");
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
        case OnAsyncSortClicked:
            EnableWindow(StartAsyncButton, FALSE);
            StartAsyncSortProcess();
            EnableWindow(StartAsyncButton, TRUE);
            break;
        case OnShowAsyncClicked:
            display_vec(AS_Vector);
            break;
        case OnSyncSortClicked:
            EnableWindow(StartSyncButton, FALSE);
            StartSyncSortProcess();
            EnableWindow(StartSyncButton, TRUE);
            break;
        case OnShowSyncClicked:
            display_vec1(S_Vector);
            break;
        case OnCleanClicked:
            CleanWidget(hInfoControl);
            break;
        case OnGenerateClicked:
            EnableWindow(GenerateButton, FALSE);
            GenerateNumbers(Vector);
            EnableWindow(GenerateButton, TRUE);
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
    //CreateWindowA("static", "hello window!", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 5, 490, 200, hWnd, NULL, NULL, NULL);//x y width height
    hEditControl = CreateWindowA("edit", NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, 0, 600,400, hWnd, NULL, NULL, NULL);
    hInfoControl = CreateWindowA("edit", NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 725, 0, 600, 400, hWnd, NULL, NULL, NULL);
    hDataViewer = CreateWindowA("edit", NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 130, 410, 1200, 200, hWnd, NULL, NULL, NULL);
    hDataViewer1 = CreateWindowA("edit", "hello window!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 130, 620, 1200, 200, hWnd, NULL, NULL, NULL);
    ShowAsyncButton = CreateWindowA("button", "ShowAsync", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 520, 120, 20, hWnd, (HMENU)OnShowAsyncClicked, NULL, NULL);
    StartAsyncButton = CreateWindowA("button", "AsyncSort", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 610, 120, 20, hWnd, (HMENU)OnAsyncSortClicked, NULL, NULL);
    ShowSyncButton = CreateWindowA("button", "ShowSync", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 730, 120, 20, hWnd, (HMENU)OnShowSyncClicked, NULL, NULL);
    StartSyncButton = CreateWindowA("button", "SyncSort", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 820, 120, 20, hWnd, (HMENU)OnSyncSortClicked, NULL, NULL);
    CleanButton = CreateWindowA("button", "CleanInfo", WS_VISIBLE | WS_CHILD | ES_CENTER, 1350,0, 120, 20, hWnd, (HMENU)OnCleanClicked, NULL, NULL);
    ReadThreadField = CreateWindowA("edit", "4", WS_VISIBLE | WS_CHILD , 5, 580, 60, 20, hWnd, NULL, NULL, NULL);
    ReadVarCountField = CreateWindowA("edit", "100000", WS_VISIBLE | WS_CHILD, 1350, 40, 120, 20, hWnd, NULL, NULL, NULL);
    GenerateButton = CreateWindowA("button", "SetVarNumber", WS_VISIBLE | WS_CHILD | ES_CENTER, 1350, 80, 120, 20, hWnd, (HMENU)OnGenerateClicked, NULL, NULL);
}
void set_vector(vector<int>& vec, size_t num) {
    vec.clear();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(1, num); // Здесь 1 и 1000 - диапазон случайных чисел
    vec.resize(num);
    for (int i = 0; i < num; ++i) {
        vec[i] = dis(gen);
    }
}
void merge(std::vector<int>& arr, int left, int middle, int right) {
    int n1 = middle - left + 1, n2 = right - middle;

    vector<int> leftArr(n1);
    vector<int> rightArr(n2);

    for (int i = 0; i < n1; ++i) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        rightArr[j] = arr[middle + 1 + j];
    }
    int i = 0, j = 0, k = left;
    // Объединяем временные массивы обратно в исходный массив
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            ++i;
        }
        else {
            arr[k] = rightArr[j];
            ++j;
        }
        ++k;
    }
    while (i < n1) {
        arr[k] = leftArr[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        ++j;
        ++k;
    }
}
void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}
struct SortData {
    vector<int>& array;
    int left, right;
};
struct MergeData {
    vector<int>& array;
    int left, middle, right;
};
DWORD WINAPI ThreadMergeSort(LPVOID lpParam) {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    //AddTextToWidget(hEditControl, "Started Thread with Id: ");
    SortData* threadData = static_cast<SortData*>(lpParam);
    merge_sort(threadData->array, threadData->left, threadData->right);
    delete threadData;
   // AddTextToWidget(hEditControl, "Ended Thread with Id: " + to_string(GetCurrentThreadId()));
    return 0;
}
DWORD WINAPI ThreadMerge(LPVOID lpParam) {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    MergeData* threadData = static_cast<MergeData*>(lpParam);
    merge(threadData->array, threadData->left, threadData->middle, threadData->right);
    delete threadData;
    return 0;
}

void CleanWidget(HWND hWidget) {
    SetTextToWidget(hWidget, "");
}
void async_merge_sort(vector<int>& array, size_t num_intervals) {
    size_t size = array.size(); // Количество интервалов
    int step = size / num_intervals - 1; // Размер каждого интервала
    vector<pair<int, int>> intervals(num_intervals);
    intervals[0].first = 0, intervals[0].second = step;
    for (int i = 1; i < num_intervals; ++i) {
        intervals[i].first = intervals[i - 1].second + 1;
        intervals[i].second = intervals[i].first + step;
    }
    intervals[num_intervals - 1].second = size - 1;
   /* string text=" Intervals(Theads): " + to_string(num_intervals)+": ";
    for (auto& now : intervals) {
        text += to_string(now.first)+"," +to_string(now.second)+"; ";
    }
    AddTextToWidget(hInfoControl,text);*/
    vector<HANDLE> threads(num_intervals);
    for (int i = 0; i < num_intervals; ++i) {
        SortData* threadData = new SortData{ ref(array), intervals[i].first, intervals[i].second };
        threads[i] = CreateThread(NULL,0, ThreadMergeSort, threadData, 0, NULL);
    }
 /*   for (auto& now : threads) {
        WaitForSingleObject(now, INFINITE);
    }
    for (auto& now : threads) {
        CloseHandle(now);
    }*/
    while (intervals.size() >= 2) {
        int count_intervals = intervals.size();
        int count_threads = threads.size();
        vector<pair<int, int>> second_intervals;
        vector<HANDLE> second_threads;
        for (int i = 0; i < count_intervals - 1; i += 2) {//2 transform to one
            WaitForSingleObject(threads[i], INFINITE);
            CloseHandle(threads[i]);
            if (count_threads >= i + 2) {
                WaitForSingleObject(threads[i+1], INFINITE);
                CloseHandle(threads[i+1]);
            }
            MergeData* threadData = new MergeData{ref(array), intervals[i].first, intervals[i].second, intervals[i + 1].second };
            HANDLE hThread = CreateThread(NULL,0, ThreadMerge, threadData, 0, NULL);
            second_threads.push_back(hThread);
            second_intervals.push_back({ intervals[i].first, intervals[i + 1].second });
        }
        if (count_intervals & 1) {
            int last = count_intervals - 1;
            if (count_threads >= last + 1) {
                WaitForSingleObject(threads[last], INFINITE);
                CloseHandle(threads[last]);
            }
            second_intervals.push_back(intervals[last]);
        }
        //threads.clear();
        threads = std::move(second_threads);
        intervals = second_intervals;
    }
    WaitForSingleObject(threads[0], INFINITE);
    CloseHandle(threads[0]);
}
void read_numbers(vector<int>& numbers) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        //std::cerr << "Failed to read" << '\n';
        return;
    }
    int number;
    while (inputFile >> number) {
        numbers.push_back(number);
    }
    // Закрываем файл
    inputFile.close();
   // std::cout << "Read " << numbers.size() << " Numbers from " << filename << '\n';
}


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
    //read_numbers(test_vec2);
    //read_numbers(Vector);
    //display_vec(Vector);
}
void StartSyncSortProcess() {
    S_Vector = Vector;
    auto start = chrono::high_resolution_clock::now();
    merge_sort(S_Vector,0, S_Vector.size()-1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    AddTextToWidget(hInfoControl, "Execution Time Of Sync Sort: " + to_string(duration.count()) + " Seconds");
    //display_vec1(S_Vector);
}
void StartAsyncSortProcess() {
        size_t thread_count = 1;
        GetWindowTextA(ReadThreadField, ThreadNumBuffer, ThreadNumBufferSize);
        try {
            AS_Vector = Vector;
            thread_count=stoul(ThreadNumBuffer);
            auto start = chrono::high_resolution_clock::now();
            async_merge_sort(AS_Vector, thread_count);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;
            AddTextToWidget(hInfoControl, "Execution Time Of Async Sort With " + to_string(thread_count) + " Threads: " + to_string(duration.count()) + " Seconds");
            //display_vec(AS_Vector);
        }
        catch (std::exception& ex) {
            AddTextToWidget(hEditControl, ex.what());
            return;
        }
}