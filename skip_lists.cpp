#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <cstdlib>
#include <random>
#include <ctime>
using namespace std;

ofstream g("output_skip_list.csv");

template<typename T>
struct Node {
    T value;
    int level;
    vector<Node*> next;

    Node(T value, int level = 0)
        : value(value), level(level), next(level+1, nullptr) {}
};

template <typename T>
struct SkipList {
    Node<T>* head;
    Node<T>* tail;
    short int max_level;
    mt19937 rng;
    uniform_int_distribution<int> random;

    SkipList() {
        head = new Node<T>(numeric_limits<T>::lowest());
        tail=new Node<T>(numeric_limits<T>::max()); 
        head->next[0]=tail;
        max_level=0;
    }
    
    short int random_level() {
        mt19937 rng(random_device{}());
        uniform_int_distribution<int> random(0,1);
        short int level = 0;
        while (random(rng) % 2 && level < max_level + 1) {
            level++;
        }
        return level;
    }

    void insert(T value) {
        if (search(value)!=nullptr)
        {
            g<<"No allowed duplicates,";
            return ;
        }
        Node<T>* update[max_level+2];
        Node<T>* current = head;

        short int node_level = random_level();
        if (node_level > max_level) 
        {
            max_level = node_level;
            head->next.resize(node_level + 1, tail);
            tail->next.resize(node_level + 1, nullptr);
        }
        for (short int i = max_level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->value <= value) {
                current = current->next[i];
            }
            update[i] = current;
        }

        Node<T>* new_node = new Node<T>(value, node_level);

        for (short int i = 0; i <= node_level; i++) {
            new_node->next[i] = update[i]->next[i];
            update[i]->next[i] = new_node;
        }
    }

    void delete_value(T value) {
        Node<T>* update[max_level + 1];
        Node<T>* current = head;

        for (short int i = max_level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->value < value) {
                current = current->next[i];
            }
            update[i] = current;
        }
        current=current->next[0];
        if (current != nullptr && current->value == value) {
            for (short int i = 0; i <= max_level; i++) {
                if (update[i]->next[i] != current) break;
                update[i]->next[i] = current->next[i];
            }
            delete current;
        }
    }

    Node<T>* search(T value) {
        Node<T>* current = head;
        
        for (short int i = max_level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->value <= value)
            {
                if (current->next[i]->value == value)
                    return current->next[i];
                current = current->next[i];
            }
        }
        return nullptr;  
    }
    T lowest_greater_than (T x)
    {
        Node<T>* current = head;
        for (short int i = max_level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->value <= x)
            {
                if (current->next[i]->value == x)
                    if (current->next[0]!=tail)
                        return current->next[0]->value;
                current = current->next[i];
            }
        }
        if(current->next[0]->value>x && current->next[0] != tail)
            return current->next[0]->value;
        g<<"Value does not exist,";
        return;  
    }
    T largest_lower_by (T x)
    {
        Node<T>* current = head;
        for (short int i = max_level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->value < x)
            {
                if (current->next[i]->value == x)
                    if (current!=head)
                        return current->value;
                current = current->next[i];
            }
        }
        if(current->next[0]->value>x && current != head)
            return current->value;
        g<<"Value does not exist,";
        return;  
    }
    void Interval(T x, T y)
    {
        Node<T>* current = head;
        for (short int i = max_level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->value < x)
                current = current->next[i];
            }
        while (current->value<=y)
        {
            g<<current->value<<" ";
            current = current->next[0];
        }
            
    }
    void search_delete_max_element()
    {
        Node<T>* current = head;
        Node<T>* update[max_level + 1];
        for (short int i = max_level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->next[i] != nullptr && current->next[i]->next[i] !=tail)
                current = current->next[i];
            update[i] = current;
        }
        if(current != head)
            {
                current=current->next[0];
                g<<current->value;
                for (short int i = 0; i <= max_level; i++) {
                    update[i]->next[i] = tail;
                }
                delete current;
            }
        else{
            g<<"Value does not exist,";
        }
        return;
    }
    T union(SkipList<T>* sl1,SkipList<T> sl2)
    {
        SkipList<T> union1;
        Node<T>* current1=sl1.head;
        Node<T>* current2=sl2.head;
        Node<T>* current3=union1->head;

    }
    void print_list() {
        for (int i=0;i<=max_level;i++)
        {
            cout<<"Level "<<i;
            Node<T>* current = head;
            while (current != nullptr) {
                cout << current->value << " ";
                current = current->next[i];
            }
            cout << endl;
        }
    }

};

int main() {
    g<<"Insert,Delete,Search,Lowest_Greater_Than,Largest_Lower_By, Interval";
    return 0;
}
