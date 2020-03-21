#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class Node {
   public:
    int key;
    int value;

   public:
    Node(int k, int v) : key(k), value(v){};
    ~Node(){};
};

class LRUCache {
   private:
    list<Node> _dq;
    unordered_map<int, list<Node>::iterator> _mp;
    size_t _size;

   public:
    LRUCache(size_t size) : _size(size) {}
    ~LRUCache() {}

    int get(int key) {
        int val;
        if (_mp.find(key) == _mp.end()) {
            val = -1;
        } else {
            val = (*_mp[key]).value;
            _dq.erase(_mp[key]);
            _dq.emplace_front(key, val);
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
                Node last = _dq.back();
                _dq.pop_back();
                _mp.erase(last.key);
            }
        }
        // Put the recent visited element to the front
        _dq.emplace_front(key, value);
        _mp[key] = _dq.begin();
    }
    void print() {
        cout << "[";
        for (auto& it : _dq)
            cout << (it.value) << " ";
        cout << "]" << endl;
    }
};

int main(int argc, char const* argv[]) {
    // LRUCache cache(4);
    // cache.put(1, 1);
    // cache.put(2, 2);
    // cache.put(5, 5);
    // cache.put(6, 6);
    // cache.get(1);     // returns 1
    // cache.get(2);     // returns -1 (not found)
    // cache.put(3, 3);  // evicts key 2
    // cache.put(4, 4);  // evicts key 1
    // cache.get(1);     // returns -1 (not found)
    // cache.get(3);     // returns 3
    // cache.get(4);     // returns 4
    // cache.print();

    LRUCache cache(1);
    cache.put(2, 1);
    cache.get(2);
    cache.put(3, 2);
    cache.get(2);
    cache.get(3);
    cache.print();
}
