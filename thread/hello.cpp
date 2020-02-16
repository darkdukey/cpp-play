#include <time.h>
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

#define LOGURU_WITH_STREAMS 1

#include "../common/loguru.cpp"

using namespace std;
using namespace std::chrono;

static const int num_threads = 8;
static const int buffer_count = 2;
static const int cmd_count = 1000;
static const int FPS_TIME = 16666;
static const int INVALID = INT_MAX;
static mutex acquire_lock;

// simulate command queue
struct CmdQueue {
    vector<int> buffer;
    atomic<bool> isReady;
};

static CmdQueue queues[buffer_count];

int acquireBuffer(bool isReady) {
    lock_guard<mutex> guard(acquire_lock);

    // Acquire empty queue
    for (int i = 0; i < buffer_count; i++) {
        if (queues[i].isReady == isReady) {
            return i;
        }
    }

    return INVALID;
}

void doProduce() {
    int v;
    int idx = acquireBuffer(false);
    steady_clock::time_point time_start, time_end;

    time_start = steady_clock::now();

    if (idx == INVALID) {
        LOG_S(INFO) << "All Queues are full";
        this_thread::sleep_for(milliseconds(1));
        return;
    }

    LOG_S(INFO) << "Filling buffer: " << idx;

    for (size_t i = 0; i < cmd_count; i++) {
        v = rand() % 100;
        queues[idx].buffer.push_back(v);
    }
    queues[idx].isReady = true;

    time_end = steady_clock::now();

    int t, remaining_time;
    t = duration_cast<microseconds>(time_end - time_start).count();
    remaining_time = FPS_TIME - t;
    if (remaining_time > 0) {
        this_thread::sleep_for(microseconds(remaining_time));
    }

    LOG_S(INFO) << "Time: " << t;
}

void doConsume() {
    int sum = 0;
    int idx = acquireBuffer(true);

    auto time_start = steady_clock::now();

    if (idx == INVALID) {
        LOG_S(INFO) << "All buffers are empty";
        this_thread::sleep_for(milliseconds(1));
        return;
    }

    LOG_S(INFO) << "Consume buffer: " << idx;

    for (auto it : queues[idx].buffer) {
        sum += it;
    }
    queues[idx].buffer.clear();
    queues[idx].isReady = false;

    LOG_S(INFO) << "Sum: " << sum;

    auto time_end = steady_clock::now();

    int t = duration_cast<microseconds>(time_end - time_start).count();
    int remaining_time = FPS_TIME - t;
    if (remaining_time > 0) {
        this_thread::sleep_for(microseconds(remaining_time));
    }
}

void t_prod(int count) {
    for (size_t i = 0; i < count; i++) {
        doProduce();
    }
}

void t_consume(int count) {
    for (size_t i = 0; i < count; i++) {
        doConsume();
    }
}

int main(int argc, char* argv[]) {
    loguru::init(argc, argv);
    srand(time(NULL));
    thread t1(t_prod, 10);
    thread t2(t_consume, 20);

    t1.join();
    t2.join();

    return 0;
}