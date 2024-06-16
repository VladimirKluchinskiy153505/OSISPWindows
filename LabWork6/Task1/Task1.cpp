#include <iostream>
#include <conio.h>
#include <ctype.h>
#include<Windows.h>
using namespace std;
HANDLE ghSemaphore;
HANDLE* ghForkMutexes;
DWORD WINAPI ThreadPhilosopher(LPVOID);
size_t MaxFood;
int THREADCOUNT, MAX_SEM_COUNT, EAT_TIMEOUT=100, THINK_TIMEOUT=100, TAKE_TIMEOUT=20, DROP_TIMEOUT=20;
struct PhilosopherData {
    int id;
    size_t current_food=0;
};
PhilosopherData* DataArray;
int main()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    printf("Enter Threads Count > 1 and MaxFood\n");
    std::cin >> THREADCOUNT>>MaxFood;
    if (THREADCOUNT < 2) { THREADCOUNT = 2; }
    MAX_SEM_COUNT = THREADCOUNT - 1;
    HANDLE* aThread=new HANDLE[THREADCOUNT];
    DWORD ThreadID;
    size_t i;
    // Create a semaphore with initial and max counts of MAX_SEM_COUNT
    ghSemaphore = CreateSemaphore(
        NULL,           // default security attributes
        MAX_SEM_COUNT,  // initial count
        MAX_SEM_COUNT,  // maximum count
        NULL);          // unnamed semaphore

    if (ghSemaphore == NULL)
    {
        printf("CreateSemaphore error: %d\n", GetLastError());
        return 1;
    }
    ghForkMutexes = new HANDLE[THREADCOUNT];
    for (size_t i = 0; i < THREADCOUNT; ++i) {
        ghForkMutexes[i]= CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex

        if (ghForkMutexes[i] == NULL)
        {
            printf("CreateMutex error: %d\n", GetLastError());
            return 1;
        }
    }
    DataArray = new PhilosopherData[THREADCOUNT];
    for (size_t i = 0; i < THREADCOUNT; ++i) {
        DataArray[i].id = i;
    }
    for (i = 0; i < THREADCOUNT; ++i)
    {
        aThread[i] = CreateThread(
            NULL,       // default security attributes
            0,          // default stack size
            (LPTHREAD_START_ROUTINE)ThreadPhilosopher,
            (LPVOID)&DataArray[i],       // no thread function arguments
            0,          // default creation flags
            &ThreadID); // receive thread identifier

        if (aThread[i] == NULL)
        {
            printf("CreateThread error: %d\n", GetLastError());
            return 1;
        }
    }
    // Wait for all threads to terminate
    
    _getch();
    for (i = 0; i < THREADCOUNT; ++i) {
        TerminateThread(aThread[i],0);
    }
    // WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);
    for (size_t i = 0; i < THREADCOUNT; ++i) {

        printf("id: %d food: %u\n", DataArray[i].id, DataArray[i].current_food);
    }


    for (i = 0; i < THREADCOUNT; i++) {
        CloseHandle(aThread[i]);
    }
    for (i = 0; i < THREADCOUNT; ++i) {
        CloseHandle(ghForkMutexes[i]);
    }
    CloseHandle(ghSemaphore);


	return 0;
}
DWORD WINAPI ThreadPhilosopher(LPVOID lpParam)
{
    int& thread_id = static_cast<PhilosopherData*>(lpParam)->id;
    size_t& current_food = static_cast<PhilosopherData*>(lpParam)->current_food;
    // lpParam not used in this example
    DWORD semaphoreWaitResult;
    DWORD leftForkMutexResult, rightForkMutexResult;
    BOOL bContinue = TRUE;

    while (bContinue)
    {
        printf("Philosopher with id: %d is thinking now\n",thread_id);
        Sleep(THINK_TIMEOUT);
        // Try to enter the semaphore gate.

        semaphoreWaitResult = WaitForSingleObject(
            ghSemaphore,   // handle to semaphore
            0L);           // zero-second time-out interval

        switch (semaphoreWaitResult)
        {
            // The semaphore object was signaled.
        case WAIT_OBJECT_0:

        // Философ берет вилки
            printf("Philosopher with id: %d is taking left fork now\n", thread_id);
            leftForkMutexResult = WaitForSingleObject(
                ghForkMutexes[thread_id],    // handle to mutex
                INFINITE);  // no time-out interval
            switch (leftForkMutexResult)
            {
                // The thread got ownership of the mutex
            case WAIT_OBJECT_0:
                    printf("Thread %d took left fork\n", thread_id);
                break;

            case WAIT_ABANDONED:
                //return FALSE;
                break;
            }
            printf("Philosopher with id: %d is taking right fork now\n",thread_id);
            Sleep(TAKE_TIMEOUT); //филосов берёт другую вилку
            rightForkMutexResult = WaitForSingleObject(
                ghForkMutexes[(thread_id+1)&THREADCOUNT],    // handle to mutex
                INFINITE);  // no time-out interval
            switch (rightForkMutexResult)
            {
                // The thread got ownership of the mutex
            case WAIT_OBJECT_0:
                 printf("Thread %d took right fork\n",thread_id);
                break;
            case WAIT_ABANDONED:
                //return FALSE;
                break;
            }
            printf("Philosopher with id: %d is eating now\n",thread_id);
            Sleep(EAT_TIMEOUT);
            printf("Philosopher with id: %d is putting left fork now\n", thread_id);
            if (!ReleaseMutex(ghForkMutexes[thread_id]))
            {
                cerr << GetLastError();
            }
            Sleep(DROP_TIMEOUT);
            printf("Philosopher with id: %d is putting right fork now\n",thread_id);
            if (!ReleaseMutex(ghForkMutexes[(thread_id+1)%THREADCOUNT]))
            {
                cerr << GetLastError();
            }
            ++current_food;
            printf("Philosopher with id: %d stopped eating now, with currentFood %u\n", thread_id, current_food);
            bContinue = current_food < MaxFood;

            // Release the semaphore when task is finished

            if (!ReleaseSemaphore(
                ghSemaphore,  // handle to semaphore
                1,            // increase count by one
                NULL))       // not interested in previous count
            {
                printf("ReleaseSemaphore error: %d\n", GetLastError());
            }
            break;

            // The semaphore was nonsignaled, so a time-out occurred.
        case WAIT_TIMEOUT:
            printf("Thread %d: wait timed out\n", thread_id);
            break;
        }
    }
    return TRUE;
}