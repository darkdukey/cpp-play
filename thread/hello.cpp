#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>

using namespace std;

static const int num_threads = 8;
static recursive_mutex mut;

static vector<int> buffer[2];
static int currIndex = 0;

void doProduce(){
    int v;
    v = rand() % 10;
    
}

void doConsume(int id){
    lock_guard<recursive_mutex> lock(mut);

    cout << "Hello from thread:" << id  << endl;
}

int main()
{
    srand (time(NULL));
    thread t[num_threads];
    
    for(int i = 0; i < num_threads; i++)
    {
        t[i] = thread(doConsume, i);
    }

    //cout << "Launched from main" << endl;

    for(int i = 0; i < num_threads; i++)
    {
        t[i].join();
    }
    

    return 0;
}