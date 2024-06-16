#include <iostream>
#include <fstream>
#include <random>
using namespace std;
void set_vector(vector<int>& vec, size_t num){
    vec.clear();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(1, num); // Здесь 1 и 1000 - диапазон случайных чисел
    vec.resize(num);
    for (int i = 0; i < num; ++i) {
        vec[i] = dis(gen);
    }
}
int main() {
    int numNumbers = 1000000; // Количество случайных чисел
    std::string filename = "C:\\Users\\avdot\\Desktop\\3Cource\\OperationSystem\\LabWorks\\LabWork333\\WriteNumbers\\random_numbers.txt"; // Имя файла для записи

    // Открываем файл для записи
    std::ofstream outputFile(filename, ios::trunc, ios::binary);

    if (!outputFile.is_open()) {
        std::cerr << "Не удалось открыть файл для записи." <<'\n';
        return 1;
    }

    // Инициализируем генератор случайных чисел
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_int_distribution<int> dis(1, 100000); // Здесь 1 и 1000 - диапазон случайных чисел

    //// Генерируем и записываем случайные числа в файл
    //for (int i = 0; i < numNumbers; ++i) {
    //    int randomNumber = dis(gen);
    //    outputFile << randomNumber <<' ';
    //}
    for (int i = 0; i < numNumbers;++i) {
        outputFile << i << ' ';
    }
    // Закрываем файл
    outputFile.close();

    std::cout << "Succed" << numNumbers << "intoFile" << filename << "." << std::endl;

    return 0;
}

