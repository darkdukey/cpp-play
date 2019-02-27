#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>
#include <atomic>

using namespace std;

static const int num_threads = 8;
static const int buffer_count = 2;
static const int cmd_count = 1000;

struct CmdQueue
{
    vector<int> buffer;
    mutex lock;
    atomic<bool> filled;
};

static CmdQueue queues[buffer_count];
static atomic<int> curr_prod_index(0);


void doProduce(){
    int v;
    int buf_idx = curr_prod_index;
    lock_guard<mutex> guard(queues[buf_idx].lock);

    cout << "Filling buffer: " << buf_idx << endl;

    for(size_t i = 0; i < cmd_count; i++)
    {
        v = rand() % 100;
        queues[buf_idx].buffer.push_back(v);
    }
    queues[buf_idx].filled = true;
    curr_prod_index = (curr_prod_index.load() + 1) % buffer_count;
}

void doConsume(){
    int idx = (curr_prod_index.load() + 1) % buffer_count;
    int sum = 0;
    lock_guard<mutex> guard(queues[idx].lock);

    cout << "Consume buffer: " << idx << endl;

    for(auto it : queues[idx].buffer){
        sum += it;
    }
    queues[idx].buffer.clear();
    queues[idx].filled = false;

    cout << "Sum: " << sum << endl;
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