#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache {
   private:
    list<int> _dq;
    unordered_map<int, list<int>::iterator> _mp;
    size_t _size;

   public:
    LRUCache(size_t size) : _size(size) {}
    ~LRUCache() {}

    int get(int key) {
        int val;
        if (_mp.find(key) == _mp.end()) {
            val = -1;
        } else {
            val = *_mp[key];
            _dq.erase(_mp[key]);
            _dq.push_front(val);
            _mp[key] = _dq.begin();
        }
        return val;
    }
    void put(int key, int value) {
        if (_mp.find(key) != _mp.end()) {
            _dq.erase(_mp[key]);
        } else {
            //Pop the last element
            if (_dq.size() == _size) {
                int last = _dq.back();
                _dq.pop_back();
                _mp.erase(last);
            }
        }
        // Put the recent visited element to the front
        _dq.push_front(value);
        _mp[key] = _dq.begin();
    }
    void print() {
        cout << "[";
        for (auto it = _dq.begin(); it != _dq.end(); it++)
            cout << (*it) << " ";
        cout << "]" << endl;
    }
};

int main(int argc, char const *argv[]) {
    LRUCache cache(4);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(5, 5);
    cache.put(6, 6);
    cache.get(1);     // returns 1
    cache.get(2);     // returns -1 (not found)
    cache.put(3, 3);  // evicts key 2
    cache.put(4, 4);  // evicts key 1
    cache.get(1);     // returns -1 (not found)
    cache.get(3);     // returns 3
    cache.get(4);     // returns 4
    cache.print();
}
