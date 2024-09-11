#include <iostream>
#include <vector>
using namespace std;

template <typename T1, typename T2>
struct ListNode {
    T1 key;
    T2 val;
    ListNode* next;
    ListNode(const T1& k, const T2& v) : key(k), val(v), next(nullptr) {}
};

template <typename T1, typename T2>
class unorderedMap {
    static const size_t tableSize = 1000;
    ListNode<T1, T2>* hashedTable[tableSize];

public:
    unorderedMap() {
        for (int i = 0; i < tableSize; i++) {
            hashedTable[i] = nullptr;
        }
    }

    ~unorderedMap() {
        clear();
    }

    // Hash function
    size_t hashFunction(const T1& key) {
        size_t a = 31;
        size_t b = 11;
        size_t p = 4294967311;
        return ((a * key + b) % p) % tableSize;
    }

    // Insertion
    bool insert(const T1& key, const T2& val) {
        size_t index = hashFunction(key);

        if (hashedTable[index] == nullptr) {
            hashedTable[index] = new ListNode<T1, T2>(key, val);
            return true;
        }

        ListNode<T1, T2>* temp = hashedTable[index];

        while (temp->next != nullptr) {
            if (temp->key == key) {
                return false;
            }
            temp = temp->next;
        }
        if (temp->key == key) {
            return false;
        }
        temp->next = new ListNode<T1, T2>(key, val);
        return true;
    }

    // Deletion
    bool erase(const T1& key) {
        size_t index = hashFunction(key);

        if (hashedTable[index] == nullptr) {
            return false;
        }

        if (hashedTable[index]->key == key) {
            ListNode<T1, T2>* toDelete = hashedTable[index];
            hashedTable[index] = hashedTable[index]->next;
            delete toDelete;
            return true;
        }

        ListNode<T1, T2>* temp = hashedTable[index];
        while (temp->next != nullptr) {
            if (temp->next->key == key) {
                ListNode<T1, T2>* toDelete = temp->next;
                temp->next = temp->next->next;
                delete toDelete;
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Checking if key exists
    bool contains(const T1& key) {
        size_t index = hashFunction(key);
        if (hashedTable[index] == nullptr) {
            return false;
        }

        if (hashedTable[index]->key == key) {
            return true;
        }

        ListNode<T1, T2>* temp = hashedTable[index];
        while (temp != nullptr) {
            if (temp->key == key) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Operator overloading for key access
    T2& operator[](const T1& key) {
        size_t index = hashFunction(key);

        if (hashedTable[index] == nullptr) {
            hashedTable[index] = new ListNode<T1, T2>(key, T2());
            return hashedTable[index]->val;
        }

        ListNode<T1, T2>* temp = hashedTable[index];
        while (temp->next != nullptr) {
            if (temp->key == key) {
                return temp->val;
            }
            temp = temp->next;
        }
        if (temp->key == key) {
            return temp->val;
        }
        
        // not there so insert with default value
        temp->next = new ListNode<T1, T2>(key, T2());
        return temp->next->val;
    }

    // Clearing the map and free memory
    void clear() {
        for (size_t i = 0; i < tableSize; i++) {
            ListNode<T1, T2>* temp = hashedTable[i];
            while (temp != nullptr) {
                ListNode<T1, T2>* node = temp;
                temp = temp->next;
                delete node;
            }
            hashedTable[i] = nullptr;
        }
    }

    // Calculating the size of the map
    size_t size() {
        size_t ans = 0;
        for (size_t i = 0; i < tableSize; i++) {
            ListNode<T1, T2>* temp = hashedTable[i];
            while (temp != nullptr) {
                ans++;
                temp = temp->next;
            }
        }
        return ans;
    }

    // Checking if the map is empty
    bool empty() {
        return size() == 0;
    }

    // Getting a vector of all keys in the map
    vector<T1> keys() {
        vector<T1> ans;
        for (size_t i = 0; i < tableSize; i++) {
            ListNode<T1, T2>* temp = hashedTable[i];
            while (temp != nullptr) {
                ans.push_back(temp->key);
                temp = temp->next;
            }
        }
        return ans;
    }

    // Print the hash table
    void print() {
        for (size_t i = 0; i < tableSize; i++) {
            ListNode<T1, T2>* temp = hashedTable[i];
            while (temp != nullptr) {
                cout << "Index " << i << ": Key = " << temp->key << ", Value = " << temp->val << endl;
                temp = temp->next;
            }
        }
    }
};

int main() {
    unorderedMap<int, int> myMap; 
    int q;
    cin >> q;

    while (q--) {
        int command;
        cin >> command;

        if (command == 1) {
            int key;
            int value;
            cin >> key >> value;
            if (myMap.contains(key)) {
                cout << "false" << endl;
            } else {
                bool inserted = myMap.insert(key, value);
                cout << (inserted ? "true" : "false") << endl;
            }
        } else if (command == 2) {
            int key;
            cin >> key;
            bool erased = myMap.erase(key);
            cout << (erased ? "true" : "false") << endl;
        } else if (command == 3) {
            int key;
            cin >> key;
            bool exists = myMap.contains(key);
            cout << (exists ? "true" : "false") << endl;
        } else if (command == 4) {
            int key;
            cin >> key;
            int& value = myMap[key];
            cout << value << endl;
        } else if (command == 5) {
            myMap.clear();
        } else if (command == 6) {
            size_t mapSize = myMap.size(); 
            cout << mapSize << endl;
        } else if (command == 7) {
            bool isMapEmpty = myMap.empty();
            cout << (isMapEmpty ? "true" : "false") << endl;
        } else if (command == 8) {
            vector<int> keys = myMap.keys();
            if (!keys.empty()) { 
                for (int key : keys) {
                    cout << key << " ";
                }
                
            }
            cout << endl;
        } 
    }

    return 0;
}
