// NQueens.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<vector>
#include<string>
#include <iostream>
#include<Windows.h>
using namespace std;
class Solution {
    int currentQ = 0, successful_var = 0;
    int Coordinates[20][2];//1<=Q<=10
    vector<vector<string>> variants;
    bool satisfy(int i, int j) {
        for (int k = 1; k <= currentQ; ++k) {
            if (Coordinates[k][0] == i || Coordinates[k][1] == j || abs(Coordinates[k][0] - i) == abs(Coordinates[k][1] - j)) {
                return false;
            }
        }
        return true;
    }
    void try_paste(int i, const int& n) {
        if (i >= n) { return; }
        for (int j = 0; j < n; ++j) {
            if (satisfy(i, j)) {
                ++currentQ;
                Coordinates[currentQ][0] = i;
                Coordinates[currentQ][1] = j;
                if (currentQ == n) {
                    ++successful_var;
                }
                try_paste(i + 1, n);
                --currentQ;
            }
        }
    }
public:
    int totalNQueens(int n) {
        try_paste(0, n);
        return successful_var;
    }
};
int main(int argc, char* argv[])
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    int k = 14;
    /*for (int i = 0; i < argc; ++i) {
        std::cout << "Argument" << i << ": " << argv[i] << std::endl;
    }*/
    if (argc > 1) {
        k = atoi(argv[1]);
    }
    cout << "Start Programm with: "<<k<<'\n';
    cout << Solution().totalNQueens(k) << '\n';
    cout<<"End Programm\n";
    system("pause");
    return 0;
}

