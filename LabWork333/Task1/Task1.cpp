#include <iostream>
#include<thread>
#include<chrono>
#include <windows.h>
using namespace std;
void DoWork1() {
    for (size_t i = 0; i < 10; ++i) {
        cout << "Id Thread: "<<this_thread::get_id()<<"\tDoWork\t"<<i<<'\n';
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
void DoWork(int a, int b) {
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "=====\t" << "DoWork Started\t====\n";
    this_thread::sleep_for(chrono::milliseconds(2000));
    cout << "a+b = " << a + b << '\n';
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "=====\t" << "DoWork Stoped\t====\n";
}
void multiply(int& q) {
    this_thread::sleep_for(chrono::milliseconds(1000));
    q *= 2;
    this_thread::sleep_for(chrono::milliseconds(1000));
}
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    thread th(DoWork1);
    thread th1(DoWork1);
    th.join();//вызываем там где хотим дождаться его выполнения
    //th1.detach();//вызываем там где хотим дождаться его выполнения
    //th.join();//вызываем там где хотим дождаться его выполнения
    //th.detach();//обрывание при завершении основного потока
    int q = 5;
    thread th2(multiply, std::ref(q));
    th2.join();
    cout << q << '\n';
    for (size_t i = 0; i < 10; ++i) {
        cout << "Id Thread: " << this_thread::get_id() << "\tmain\t" << i << '\n';;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return 0;
}