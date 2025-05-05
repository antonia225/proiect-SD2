#pragma once
#include <iostream>
#include <random>
#include <memory>
#include <vector>
using namespace std;

mt19937 rng(random_device{}());

struct Treap
{
    int key, priority;
    shared_ptr<Treap> left, right;
    Treap(int k) : key(k), priority(rng()), left(nullptr), right(nullptr) {}
};

using ptr_Treap = shared_ptr<Treap>;

// Split treap în două treap-uri
// l conține toate elementele < key
// r conține toate elementele ≥ key
void split(ptr_Treap t, int key, ptr_Treap &l, ptr_Treap &r)
{
    if (!t)
        l = r = nullptr;

    else if (key < t->key)
    {
        split(t->left, key, l, t->left);
        r = t;
    }
    else
    {
        split(t->right, key, t->right, r);
        l = t;
    }
}

// Combină două treap-uri presupunând că ambele sunt deja sortate
// și că nu există elemente duplicate
// Merge două treap-uri: l conține < r, r conține ≥ l
ptr_Treap merge(ptr_Treap l, ptr_Treap r)
{
    if (!l || !r)
        return l ? l : r;

    if (l->priority > r->priority)
    {
        l->right = merge(l->right, r);
        return l;
    }

    else
    {
        r->left = merge(l, r->left);
        return r;
    }
}

// Verifică dacă un element există în treap
// Returnează true dacă key există în treap, false altfel
bool find(ptr_Treap t, int key)
{
    while (t)
    {
        if (t->key == key)
            return true;
        t = (key < t->key) ? t->left : t->right;
    }
    return false;
}

// Adaugă un element în treap
// Returnează treap-ul rezultat
// Dacă elementul există deja, nu se va adăuga un duplicat
// și treap-ul va rămâne neschimbat
ptr_Treap insert(ptr_Treap t, ptr_Treap item)
{
    if (!t)
        return item;

    if (item->priority > t->priority)
    {
        split(t, item->key, item->left, item->right);
        return item;
    }
    else if (item->key < t->key)
        t->left = insert(t->left, item);

    else if (item->key > t->key)
        t->right = insert(t->right, item);

    return t;
}

// Șterge un element din treap
// Returnează treap-ul rezultat
// Dacă elementul nu există, treap-ul va rămâne neschimbat
ptr_Treap erase(ptr_Treap t, int key)
{
    if (!t)
        return nullptr;

    // Dacă cheia este egală cu cheia curentă, elimină nodul
    // și combină subarborii stâng și drept
    if (t->key == key)
        return merge(t->left, t->right);

    else if (key < t->key)
        t->left = erase(t->left, key);

    else
        t->right = erase(t->right, key);

    return t;
}

// Returnează cel mai mare element din treap cu valoarea mai mică sau egală cu key
int floor(ptr_Treap t, int key)
{
    int res = -1;
    while (t)
        if (t->key <= key)
        {
            res = t->key;
            t = t->right;
        }
        else
            t = t->left;

    return res;
}

// Returnează cel mai mic element din treap cu valoarea mai mare sau egală cu key
int ceil(ptr_Treap t, int key)
{
    int res = -1;
    while (t)
        if (t->key >= key)
        {
            res = t->key;
            t = t->left;
        }
        else
            t = t->right;

    return res;
}

// Returnează toate elementele din treap care se află în intervalul [x, y]
// Adaugă elementele în vectorul out
void range(ptr_Treap t, int x, int y, vector<int> &out)
{
    if (!t)
        return;

    if (x <= t->key && t->key <= y)
        out.push_back(t->key);

    if (x < t->key)
        range(t->left, x, y, out);

    if (t->key < y)
        range(t->right, x, y, out);
}

// Reunește două treap-uri 
ptr_Treap reunion(ptr_Treap a, ptr_Treap b)
{
    if (!a || !b)
        return a ? a : b;

    if (a->priority < b->priority)
        swap(a, b);

    ptr_Treap l, r;
    split(b, a->key, l, r);

    a->left = reunion(a->left, l);
    a->right = reunion(a->right, r);
    return a;
}

// Afișează treap-ul în ordine (dreapta în sus, stânga în jos)
void print_treap(ptr_Treap t, int depth = 0)
{
    if (!t)
        return;

    print_treap(t->right, depth + 1);
    cout << string(4 * depth, ' ') << t->key << " (p=" << t->priority << ")\n";
    print_treap(t->left, depth + 1);
}
