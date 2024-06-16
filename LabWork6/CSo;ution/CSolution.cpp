#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <semaphore>

const int num_philosophers = 5;
std::mutex forks[num_philosophers];
std::semaphore waiter;

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1, 1000);

void philosopher(int id) {
    while (true) {
        // ������� ������
        std::this_thread::sleep_for(std::chrono::milliseconds(distribution(generator)));

        // ������� ����� ����
        sem_wait(&waiter);

        // ������� ����� �����
        std::unique_lock<std::mutex> left_lock(forks[id]);
        std::this_thread::sleep_for(std::chrono::milliseconds(distribution(generator))); // ��� ����������� ������� ������� ��������
        std::unique_lock<std::mutex> right_lock(forks[(id + 1) % num_philosophers]);

        // ������� ���
        std::cout << "������� " << id << " ���." << std::endl;

        // ����������� �����
        right_lock.unlock();
        left_lock.unlock();

        // �������� ��������� ������ ��������� ����
        sem_post(&waiter);

        // ������� �������� ���� � ������ �����
        std::this_thread::sleep_for(std::chrono::milliseconds(distribution(generator)));
    }
}

int main() {
    sem_init(&waiter, 0, num_philosophers - 1); // �������������� ������� � (���������� ��������� - 1)

    for (int i = 0; i < num_philosophers; i++) {
        forks[i].lock();
    }

    std::vector<std::thread> philosophers;

    for (int i = 0; i < num_philosophers; i++) {
        philosophers.push_back(std::thread(philosopher, i));
    }

    for (int i = 0; i < num_philosophers; i++) {
        philosophers[i].join();
    }

    sem_destroy(&waiter);

    return 0;
}