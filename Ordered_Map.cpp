#include <iostream>
#include <vector>
using namespace std;

template <typename T1, typename T2>
struct ListNode {
    T1 key;
    T2 val;
    struct ListNode* left;
    struct ListNode* right;
    int height;
    size_t size;

     //default constructor for my struct
    ListNode(const T1& key, const T2& val) {
        this->key = key;
        this->val = val;
        this->height = 1;
        this->size = 1;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template <typename T1, typename T2>
class orderMap {
public:
    ListNode<T1, T2>* root;

     //default constructor for my class
    orderMap() {
        root = nullptr;
    }

    int getHeight(ListNode<T1, T2>* root) {
        if (root == nullptr) {
            return 0;
        }
        return root->height;
    }

    int getSize(ListNode<T1, T2>* root) {
        if (root == nullptr) {
            return 0;
        }
        return root->size;
    }

    int getBalanceFactor(ListNode<T1, T2>* root) {
        if (root == nullptr) {
            return 0;
        }
        return getHeight(root->left) - getHeight(root->right);
    }

    void updateHeight(ListNode<T1, T2>* root) {
        if (root == nullptr) {
            return;
        }
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    }

    void updateSize(ListNode<T1, T2>* root) {
        if (root == nullptr) {
            return;
        }
        root->size = 1 + getSize(root->left) + getSize(root->right);
    }

    ListNode<T1, T2>* rotateLeft(ListNode<T1, T2>* x) {
        ListNode<T1, T2>* y = x->right;
        ListNode<T1, T2>* temp = y->left;

        y->left = x;
        x->right = temp;

        updateHeight(x);
        updateSize(x);
        updateHeight(y);
        updateSize(y);

        return y;
    }

    ListNode<T1, T2>* rotateRight(ListNode<T1, T2>* y) {
        ListNode<T1, T2>* x = y->left;
        ListNode<T1, T2>* temp = x->right;

        x->right = y;
        y->left = temp;

        updateHeight(y);
        updateSize(y);
        updateHeight(x);
        updateSize(x);

        return x;
    }

    ListNode<T1, T2>* insert(ListNode<T1, T2>* root, const T1& key, const T2& val) {
        if (root == nullptr) {
            return new ListNode<T1, T2>(key, val);
        }

        if (root->key > key) {
            root->left = insert(root->left, key, val);
        } else if (root->key < key) {
            root->right = insert(root->right, key, val);
        } else {
            return root;
        }

        updateHeight(root);
        updateSize(root);

        int bf = getBalanceFactor(root);


        //means left ssbtree is more populated than right one
        if (bf > 1) {
            if (root->left->key > key) {
                return rotateRight(root);
            } else {
                root->left = rotateLeft(root->left);
                return rotateRight(root);
            }
        }


         //means right ssbtree is more populated than left one
        if (bf < -1) {
            if (key > root->right->key) {
                return rotateLeft(root);
            } else {
                root->right = rotateRight(root->right);
                return rotateLeft(root);
            }
        }

        return root;
    }

    //fiding the minimum 
    ListNode<T1, T2>* findMinimum(ListNode<T1, T2>* root) {
        while (root->left != nullptr) {
            root = root->left;
        }
        return root;
    }


    //fiding the maximum
    ListNode<T1, T2>* findMaximum(ListNode<T1, T2>* root) {
        while (root->right != nullptr) {
            root = root->right;
        }
        return root;
    }

    ListNode<T1, T2>* erase(ListNode<T1, T2>* root, const T1& key) {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->key > key) {
            root->left = erase(root->left, key);
        } else if (root->key < key) {
            root->right = erase(root->right, key);
        } else {
            if (root->left == nullptr || root->right == nullptr) {
                ListNode<T1, T2>* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                ListNode<T1, T2>* temp = findMinimum(root->right);
                root->key = temp->key;
                root->val = temp->val;
                root->right = erase(root->right, temp->key);
            }
        }

        if (root == nullptr) {
            return nullptr;
        }

        updateHeight(root);
        updateSize(root);

        int balance = getBalanceFactor(root);


        //means left ssbtree is more populated than right one
        if (balance > 1) {
            int leftBalance = getBalanceFactor(root->left);
            if (leftBalance >= 0) {
                return rotateRight(root);
            } else {
                root->left = rotateLeft(root->left);
                return rotateRight(root);
            }
        }
        
         //means right ssbtree is more populated than left one
        if (balance < -1) {
            int rightBalance = getBalanceFactor(root->right);
            if (rightBalance <= 0) {
                return rotateLeft(root);
            } else {
                root->right = rotateRight(root->right);
                return rotateLeft(root);
            }
        }

        return root;
    }

    bool contains(ListNode<T1, T2>* root, const T1& key) {
        if (root == nullptr) {
            return false;
        }

        if (key < root->key) {
            return contains(root->left, key);
        } else if (key > root->key) {
            return contains(root->right, key);
        } else {
            return true;
        }
    }

    T2& find(ListNode<T1, T2>* root, const T1& key) {
        if (root == nullptr) {
            root = insert(root, key, T2());
        }

        if (root->key > key) {
            return find(root->left, key);
        } else if (root->key < key) {
            return find(root->right, key);
        } else {
            return root->val;
        }
    }

    void clear(ListNode<T1, T2>* root) {
        if (root == nullptr) {
            return;
        }

        clear(root->left);
        clear(root->right);

        delete root;
    }

    void keys(ListNode<T1, T2>* root, vector<T1>& ans) {
        if (root == nullptr) {
            return;
        }

        keys(root->left, ans);
        ans.push_back(root->key);
        keys(root->right, ans);
    }

    ListNode<T1, T2>* lowerBound(ListNode<T1, T2>* root, const T1& key) {
        if (root == nullptr) {
            return nullptr;
        }

        if (root->key >= key) {
            ListNode<T1, T2>* left = lowerBound(root->left, key);
            if (left != nullptr) {
                return left;
            }
            return root;
        } 
    
        return lowerBound(root->right, key);
    }

    ListNode<T1, T2>* upperBound(ListNode<T1, T2>* root, const T1& key) {
        if (root == nullptr) {
            return nullptr;
        }

        if (root->key > key) {
            ListNode<T1, T2>* left = upperBound(root->left, key);
            if (left != nullptr) {
                return left;
            }
            return root;
        }

        return upperBound(root->right, key);
    }

    bool empty() {
        return root == nullptr;
    }

    int size() {
        return getSize(root);
    }

    bool contains(const T1& key) {
        return contains(root, key);
    }

    bool insert(const T1& key, const T2& value) {
        size_t initialSize = size();
        root = insert(root, key, value);
        return size() > initialSize;
    }

    bool erase(const T1& key) {
        size_t initialSize = size();
        root = erase(root, key);
        return size() < initialSize;
    }

    T2& operator[](const T1& key) {
        if (!contains(root, key)) {
            root = insert(root, key, T2());
        }
        return find(root, key);
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    vector<T1> keys() {
        vector<T1> ans;
        keys(root, ans);
        return ans;
    }

    pair<bool, T1> lower_bound(const T1& key) {
        ListNode<T1, T2>* node = lowerBound(root, key);
        if (node != nullptr) {
            return make_pair(true, node->key);
        } else {
            return make_pair(false, T1());
        }
    }

    pair<bool, T1> upper_bound(const T1& key) {
        ListNode<T1, T2>* node = upperBound(root, key);
        if (node != nullptr) {
            return make_pair(true, node->key);
        } else {
            return make_pair(false, T1());
        }
    }
};

int main() {
    orderMap<int, string> orderedMap;
    int input = -1;

    while (input != 0) {
        cin >> input;

        if (input == 1) {
            bool isEmpty = orderedMap.empty();
            cout << (isEmpty ? "true" : "false") << endl;
        } else if (input == 2) {
            int mapSize = orderedMap.size();
            cout << mapSize << endl;
        } else if (input == 3) {
            int key;
            cin >> key;
            bool containsKey = orderedMap.contains(key);
            cout << (containsKey ? "true" : "false") << endl;
        } else if (input == 4) {
            int key;
            string value;
            cin >> key >> value;
            bool inserted = orderedMap.insert(key, value);
            cout << (inserted ? "true" : "false") << endl;
        } else if (input == 5) {
            int key;
            cin >> key;
            bool erased = orderedMap.erase(key);
            cout << (erased ? "true" : "false") << endl;
        } else if (input == 6) {
            int key;
            cin >> key;
            string& value = orderedMap[key];
            cout << value << endl;
        } else if (input == 7) {
            orderedMap.clear();
        } else if (input == 8) {
            vector<int> keys = orderedMap.keys();
            for (int key : keys) {
                cout << key << endl;
            }
        } else if (input == 9) {
            int key;
            cin >> key;
            auto lb = orderedMap.lower_bound(key);
            if (lb.first) {
                cout<<"true"<<endl;
                cout << lb.second << endl;
            } else {
                cout << "false" << endl;
            }
        } else if (input == 10) {
            int key;
            cin >> key;
            auto ub = orderedMap.upper_bound(key);
            if (ub.first) {
                cout<<"true"<<endl;
                cout << ub.second << endl;
            } else {
                cout << "false" << endl;
            }
        }
    }

    return 0;
}
