#include<vector>
#include<string>
#include <iostream>
#include<fstream>
#include<Windows.h>
typedef unsigned long long int ull;
using namespace std;
const wchar_t* FileName = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork2\\FindPrimeNumbers\\Doc.txt";
const wchar_t* FileNameNum = L"C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork2\\FindPrimeNumbers\\NumStop.txt";
void clean_file() {
    std::fstream outputFile(FileName, ios::trunc, ios::binary); // Открыть файл для записи (с флагом ios::trunc)
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file" <<'\n';
    }
    else {
        outputFile.close();
        cout << "File successfully cleaned." << '\n';
    }
}
void add_to_file(ull num, std::fstream& outputFile) {
        outputFile << " " << num;
}
ull get_last_number() {
    std::ifstream file(FileName);
    if (!file.is_open()) {
        std::cerr << "Error open " << FileName <<'\n';
        return 1;
    }
    std::string lastNumber;
    char ch;
    bool digitsRead = false;
    file.seekg(-1, std::ios::end);
    while (file.tellg() > 0) {
        file.get(ch);
        //cout << "position: " << file.tellg()<<"Symbol: "<<ch << '\n';
        file.seekg(-2, std::ios::cur); // Перемещаем указатель на один символ назад
        if (std::isdigit(ch)) {
            // Если найдена цифра, добавляем ее к последнему числу
            lastNumber = ch + lastNumber;
            digitsRead = true;
        }
        else if (digitsRead) {
            // Если найден символ, отличный от цифры, и были прочитаны цифры, значит, мы нашли последнее число
            break;
        }
    }
    file.close();
    ull lastNum = 0;
    if (!lastNumber.empty()) {
        lastNum = std::stoull(lastNumber);
    }
    std::cout << "Last num " << lastNum << std::endl;
    return lastNum;
}
void find_prime_numbers(ull start, ull& N, std::fstream& outputFile) {
    for (ull num = start; num <= N; ++num) {
        ull up_bound = sqrt(num);
        if (num > 2 && (num % 2 == 0)) {
            continue;
        }
        else if (num == 2) {
            add_to_file(num, outputFile);
        }
        else {
            bool prime = true;
            for (int i = 3; i <= up_bound; i += 2) {
                if (num % i == 0) {
                    prime = false;
                    break;
                }
            }
            if (prime) {
                add_to_file(num, outputFile);
            }
        }
    }
}
int main(int argc, char* argv[])
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    ull N = 100000000;
    ull Start = get_last_number();
    if (argc > 1) {
        N = std::strtoull(argv[1], nullptr, 10);
    }
    if (argc > 2) {
        if (argv[1] == "1") {
            clean_file();
        }
    }
    std::fstream outputFile(FileName, std::ios::app | std::ios::out | std::ios::binary);
    if (!outputFile.is_open()) {
        cout << "Failed to open file\n";
    }
    else {
        /*for (int i = 0; i < argc; ++i) {
            std::cout << "Argument" << i << ": " << argv[i] << std::endl;
        }*/
        //outputFile.imbue(std::locale("en_US.UTF-8"));

        cout << "Start Programm with: " << Start << '\n';
        find_prime_numbers(Start, N, outputFile);
        cout << "End Programm\n";
        outputFile.close();
    }
   // system("pause");
    return 0;
}

