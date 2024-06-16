#include <iostream>
#include <thread>
#include<chrono>
#include<vector>
#include<deque>
#include<fstream>
#include <windows.h>
using namespace std;
const char* filename= "C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork333\\WriteNumbers\\random_numbers.txt"; // Имя файла для записи
vector<int> test_vec{23, 56, 12, 89, 45, 34, 72, 9, 63, 17, 50, 98, 31, 5, 41, 77, 28, 67, 14, 82, 37, 70, 3, 61, 8, 93, 25, 59, 16, 48, 95, 22, 53, 11, 87, 43, 36, 75, 6, 30, 67, 19, 51, 99, 33, 4, 44, 73, 10, 66, 21, 52, 12, 96, 27, 62, 15, 49, 84, 38, 71, 7, 54, 13, 90, 46, 29, 60, 18, 64, 26, 58, 20, 65, 6, 76, 32, 55, 5, 85, 35, 79, 9, 57, 1, 80, 42, 68, 2, 81, 47, 91, 24, 88, 40, 78, 7, 94, 39, 74, 7, 69, 1, 92, 47, 64, 2, 85, 31, 59, 10, 73, 27, 62, 19, 51, 15, 89, 38, 76, 14, 80, 34, 70, 13, 68, 8, 88, 44, 91, 17, 83, 29, 67, 11, 63, 5, 75, 21, 82, 37, 71, 3, 79, 33, 55, 6, 94, 23, 57, 12, 97, 32, 69, 18, 46, 16, 84, 40, 78, 2, 54, 22, 74, 8, 93, 41, 99, 20, 52, 24, 86, 1, 66, 4, 77, 30, 58, 7, 96, 26, 56, 9, 65, 28, 47, 1, 81, 45, 84, 18, 57, 7, 67, 2, 93, 43, 75, 14, 59, 3, 78, 9, 88, 22, 65, 6, 98, 25, 60, 5, 91, 33, 64, 11, 77, 31, 73, 13, 55, 4, 94, 20, 62, 8, 89, 27, 61, 12, 76, 29, 70, 15, 66, 28, 69, 17, 68, 1, 96, 41, 79, 10, 86, 37, 72, 21, 54, 16, 90, 35, 71, 6, 97, 26, 58, 4, 83, 32, 63, 7, 95, 23, 59, 8, 77, 35, 69, 14, 62, 31, 66, 13, 55, 12, 89, 34, 72, 11, 80, 28, 64, 9, 56, 27, 73, 16, 51, 37, 75, 5, 91, 24, 60, 3, 82, 30, 68, 15, 87, 33, 71, 10, 98, 22, 63, 7, 85, 26, 61, 1, 99, 39, 74, 17, 79, 29, 67, 8, 94, 25, 58, 6, 86, 36, 70, 4, 78, 32, 65, 14, 81, 20, 57, 5, 93, 30, 61, 13, 75, 27, 64, 9, 87, 38, 73, 3, 90, 22, 68, 12, 89, 21, 76, 2, 83, 31, 66, 10, 97, 23, 59, 7, 84, 25, 62, 1, 100, 48, 90, 35, 81, 40, 76, 16, 96, 31, 67, 14, 94, 37, 71, 12, 86, 27, 63, 5, 85, 29, 69, 9, 99, 33, 70, 11, 79, 32, 74, 7, 98, 23, 60, 4, 82, 28, 65, 3, 88, 21, 77, 2, 91, 19, 72, 8, 93, 25, 61, 6, 92, 24, 73, 15, 83, 30, 68, 10, 95, 26, 64, 7, 87, 38, 75, 13, 89, 36, 78, 3, 96, 18, 66, 4, 91, 20, 72, 1, 98, 32, 67, 9, 85, 35, 69, 6, 94, 29, 63, 2, 100, 46, 79, 11, 88, 22, 77, 5, 92, 26, 61, 8, 97, 33, 70, 12, 93, 37, 74, 7, 95, 30, 64, 3, 86, 17, 71, 1, 99, 40, 75, 14, 90, 31, 68, 16, 92, 39, 80, 10, 89, 34, 73, 15, 84};
vector<int> test_vec1{402, 128, 214, 44, 233, 370, 72, 311, 278, 173, 480, 318, 367, 196, 162, 231, 70, 404, 187, 309, 56, 268, 66, 447, 389, 251, 208, 179, 250, 265, 22, 103, 483, 437, 406, 150, 191, 119, 417, 40, 332, 99, 238, 471, 218, 395, 242, 288, 216, 264, 384, 121, 115, 455, 376, 4, 110, 329, 34, 183, 222, 131, 85, 291, 376, 258, 61, 376, 442, 384, 472, 329, 75, 404, 204, 393, 381, 137, 232, 389, 213, 494, 465, 45, 21, 372, 83, 235, 462, 121, 41, 196, 78, 198, 322, 340, 17, 431, 400, 464, 152, 187, 392, 153, 254, 484, 71, 159, 422, 294, 430, 431, 204, 272, 132, 411, 19, 82, 278, 273, 184, 274, 145, 47, 424, 338, 143, 96, 399, 192, 374, 229, 337, 68, 483, 422, 67, 134, 490, 330, 409, 431, 32, 265, 288, 233, 53, 98, 394, 109, 480, 256, 19, 163, 226, 67, 474, 272, 3, 321, 124, 149, 255, 50, 12, 498, 81, 121, 153, 452, 360, 405, 244, 151, 378, 20, 147, 209, 77, 237, 476, 327, 286, 252, 65, 194, 56, 355, 336, 445, 391, 175, 32, 339, 254, 492, 246, 269, 68, 8, 131, 487, 246, 174, 434, 226, 67, 165, 477, 48, 366, 148, 342, 155, 45, 421, 460, 118, 209, 495, 24, 276, 243, 166, 487, 18, 395, 337, 58, 190, 112, 90, 366, 433, 429, 214, 194, 430, 343, 315, 355, 366, 258, 9, 106, 488, 62, 139, 356, 1, 414, 261, 318, 150, 296, 261, 251, 414, 155, 51, 495, 221, 96, 272, 458, 243, 288, 97, 440, 429, 463, 7, 382, 440, 97, 240, 339, 296, 420, 38, 86, 434, 306, 199, 383, 46, 215, 207, 441, 139, 108, 437, 277, 253, 498, 413, 394, 310, 147, 23, 100, 310, 13, 346, 135, 47, 296, 33, 242, 235, 384, 115, 488, 255, 429, 425, 11, 347, 251, 215, 184, 104, 410, 22, 66, 155, 98, 344, 299, 318, 7, 204, 116, 139, 47, 281, 126, 69, 337, 37, 145, 42, 215, 394, 386, 277, 103, 171, 405, 367, 302, 189, 152, 320, 226, 85, 483, 237, 437, 253, 38, 455, 212, 479, 269, 20, 102, 141, 372, 27, 240, 87, 456, 263, 345, 94, 258, 33, 91, 350, 35, 318, 453, 293, 16, 249, 389, 132, 497, 364, 254, 180, 399, 125, 400, 9, 127, 244, 1, 464, 321, 134, 482, 105, 360, 464, 228, 244, 90, 128, 266, 372, 2, 211, 362, 480, 169, 198, 377, 138, 80, 226, 276, 201, 355, 27, 312, 105, 294, 76, 429, 338, 79, 452, 178, 269, 139, 475, 246, 335, 60, 160, 9, 412, 28, 321, 128, 381, 200, 474, 34, 261, 218, 103, 399, 73, 469, 93, 236, 207, 477, 143, 237, 10, 174, 421, 92, 407, 203, 294, 358, 170, 362, 114, 452, 57, 174, 5, 346, 37, 472, 13, 139, 429, 263, 206, 470, 61, 125, 232, 66, 381, 360, 176, 312, 282, 143, 120, 212, 490, 262, 75};
vector<int> test_vec2;
void read_numbers(vector<int>& numbers) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to read" <<'\n';
        return;
    }
    int number;
    while (inputFile >> number) {
        numbers.push_back(number);
    }
    // Закрываем файл
    inputFile.close();
    std::cout << "Read " << numbers.size() << " Numbers from " << filename <<'\n';
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
void display_vec(vector<int>& vec) {
    for (auto& now : vec) {
        cout << now << ' ';
    }
    cout << '\n';
}
void async_merge_sort4(vector<int>& array) {
    //std::vector<int> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }; // Ваш массив
    const int num_intervals = 4, size = array.size(); // Количество интервалов
    int step = size / num_intervals-1; // Размер каждого интервала

    vector<pair<int, int>> intervals(num_intervals);
    intervals[0].first = 0, intervals[0].second = step;
    for (int i = 1; i < num_intervals; ++i) {
        intervals[i].first = intervals[i-1].second + 1;
        intervals[i].second = intervals[i].first + step;
    }
    intervals[num_intervals - 1].second = size - 1;
    cout << "Intervals\n";
    for (auto& now : intervals) {
        cout << now.first << ' '<<now.second<<'\n';
    }
    vector<thread> threads(num_intervals);
    for (int i = 0; i < num_intervals; ++i) {
        threads[i] = thread(merge_sort,ref(array), intervals[i].first, intervals[i].second);
    }
    vector<thread> second_threads;
    /*for (int i = 0; i < num_intervals; ++i) {
        threads[i].join();
    }*/
    for (int i = 0; i < num_intervals - 1; i += 2) {
        threads[i].join();
        threads[i + 1].join();
        second_threads.push_back(thread(merge, ref(array), intervals[i].first, intervals[i].second, intervals[i + 1].second));
    }
    for (auto& now : second_threads) {
        now.join();
    }
    merge(array, intervals[0].first, intervals[1].second, intervals[3].second);
}
void async_merge_sort42(vector<int>& array, size_t num_intervals) {
    cout << "Threads count: " << num_intervals << '\n';
    //std::vector<int> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }; // Ваш массив
    size_t size = array.size(); // Количество интервалов
    int step = size / num_intervals - 1; // Размер каждого интервала

    vector<pair<int, int>> intervals(num_intervals);
    intervals[0].first = 0, intervals[0].second = step;
    for (int i = 1; i < num_intervals; ++i) {
        intervals[i].first = intervals[i - 1].second + 1;
        intervals[i].second = intervals[i].first + step;
    }
    intervals[num_intervals - 1].second = size - 1;
    cout << "Intervals\n";
    for (auto& now : intervals) {
        cout << now.first << ' ' << now.second << '\n';
    }
    vector<thread> threads(num_intervals);
    for (int i = 0; i < num_intervals; ++i) {
        threads[i] = thread(merge_sort, ref(array), intervals[i].first, intervals[i].second);
    }
    
    /*for (int i = 0; i < num_intervals; ++i) {
        threads[i].join();
    }*/
    while (intervals.size() >= 2) {
        int count_intervals = intervals.size();
        int count_threads = threads.size();
        vector<pair<int, int>> second_intervals;
        vector<thread> second_threads;
        for (int i = 0; i < count_intervals - 1; i += 2) {//2 transform to one
            threads[i].join();
            if (count_threads >= i + 2) {
                threads[i + 1].join();
            }
            second_threads.push_back(thread(merge, ref(array), intervals[i].first, intervals[i].second, intervals[i + 1].second));
            second_intervals.push_back({ intervals[i].first, intervals[i+1].second });
        }
        if (count_intervals & 1) {
            int last = count_intervals - 1;
            if (count_threads >= last+1) {
                threads[last].join();
            }
            second_intervals.push_back(intervals[last]);
        }
        threads.clear();
        threads = std::move(second_threads);
        intervals = second_intervals;
    }
    threads[0].join();
    /*for (auto& now : second_threads) {
        now.join();
    }*/
   /* merge(array, intervals[0].first, intervals[1].second, intervals[3].second);*/
}
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    read_numbers(test_vec2);
    //display_vec(test_vec2);

    vector<int> vec(test_vec2);
    auto start = chrono::high_resolution_clock::now();
    async_merge_sort42(vec, 8);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Asynchronous Execution Duration " << duration.count() << "Seconds\n";
   // display_vec(vec);

    vector<int> vec1(test_vec2);
    auto start1 = chrono::high_resolution_clock::now();
    merge_sort(vec1, 0, vec1.size() - 1);
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration1 = end1 - start1;
    //display_vec(vec1);
    cout << "Synchronous Execution Duration " << duration1.count() << "Seconds\n";;

    return 0;
}