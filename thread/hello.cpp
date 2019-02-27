#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>

using namespace std;

static const int num_threads = 8;
static const int buffer_count = 2;
static const int cmd_count = 1000;

static mutex mut_buffer[buffer_count];

static vector<int> buffer[buffer_count];
static int curr_prod_index = 0;

void doProduce(){
    int v;
    int buf_idx = curr_prod_index;
    lock_guard<mutex> guard(mut_buffer[buf_idx]);

    cout << "Filling buffer: " << buf_idx << endl;

    for(size_t i = 0; i < cmd_count; i++)
    {
        v = rand() % 100;
        buffer[buf_idx].push_back(v);
    }

    curr_prod_index = (curr_prod_index + 1) % buffer_count;
}

void doConsume(){
    int idx = (curr_prod_index + 1) % buffer_count;
    lock_guard<mutex> guard(mut_buffer[idx]);

    cout << "Consume buffer: " << idx << endl;
    cout << "Item: ";

    for(auto it : buffer[idx]){
        cout << it;
    }

    cout << endl;
}

void t_prod(int count){
    for(size_t i = 0; i < count; i++)
    {
        doProduce();
    }
}

void t_consume(int count){
    for(size_t i = 0; i < count; i++)
    {
        doConsume();
    }
    
}

int main(int argc, char* argv[])
{
    srand (time(NULL));
    thread t1(t_prod, 10);
    thread t2(t_consume, 10);
    
    t1.join();
    t2.join();

    return 0;
}