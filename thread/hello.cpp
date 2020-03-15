#include <sys/kdebug_signpost.h>
#include <time.h>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define LOGURU_WITH_STREAMS 1

#include "../common/loguru.cpp"

using namespace std;
using namespace std::chrono;

static const int buffer_count = 2;
static const int cmd_count = 100000;
static const int FPS_TIME = 16666;
static const int INVALID = INT_MAX;
static mutex acquire_lock;
static bool stop = false;

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

    if (idx == INVALID) {
        LOG_S(WARNING) << "All Queues are full: Sleep";
        this_thread::sleep_for(milliseconds(1));
        return;
    }

    LOG_S(INFO) << "* Filling buffer: " << idx;

    kdebug_signpost_start(1, 0, 0, 0, 1);
    time_start = steady_clock::now();
    for (size_t i = 0; i < cmd_count; i++) {
        v = rand() % 100;
        queues[idx].buffer.push_back(v);
    }
    queues[idx].isReady = true;
    time_end = steady_clock::now();
    kdebug_signpost_end(1, 0, 0, 0, 1);

    int t, remaining_time;
    t = duration_cast<microseconds>(time_end - time_start).count();

    LOG_S(INFO) << "* Product Time: " << t;
    remaining_time = FPS_TIME - t;
    if (remaining_time > 0) {
        this_thread::sleep_for(microseconds(remaining_time));
    }
}

void doConsume() {
    int sum = 0;
    int idx = acquireBuffer(true);

    if (idx == INVALID) {
        LOG_S(WARNING) << "All buffers are empty: Sleep";
        this_thread::sleep_for(milliseconds(1));
        return;
    }

    LOG_S(INFO) << "= Consume buffer: " << idx;

    kdebug_signpost_start(2, 0, 0, 0, 2);
    auto time_start = steady_clock::now();
    for (auto it : queues[idx].buffer) {
        sum += sqrt(it);
    }
    queues[idx].buffer.clear();
    queues[idx].isReady = false;
    auto time_end = steady_clock::now();
    kdebug_signpost_end(2, 0, 0, 0, 2);

    int t = duration_cast<microseconds>(time_end - time_start).count();
    LOG_S(INFO) << "= Sum: " << sum;
    LOG_S(INFO) << "= Consume Time: " << t;

    int remaining_time = FPS_TIME - t;
    if (remaining_time > 0) {
        this_thread::sleep_for(microseconds(remaining_time));
    }
}

void t_prod() {
    while (!stop) {
        doProduce();
    }
}

void t_consume() {
    while (!stop) {
        doConsume();
    }
}

int main(int argc, char* argv[]) {
    loguru::init(argc, argv);
    srand(time(NULL));
    thread t1(t_prod);
    thread t2(t_consume);
    char ch_quit;
    cin >> ch_quit;
    stop = true;

    t1.join();
    t2.join();

    return 0;
}
