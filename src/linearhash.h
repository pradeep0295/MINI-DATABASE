#include<iostream>
#include<unordered_set>
#include<vector>
#include<math.h>
using namespace std;

/**
 * @brief 
 * 2.0. Linear hashing with bucket(vector) containing KeyValue pairs.
 * initial no. of buckets are 2.
 * is_present is not implemented.
 */

class Bucket{
public:
    vector<pair<int,int>> KeyValue;
    Bucket *overflow = NULL;
    int size = 0;
    vector<bool> avail;

    Bucket(int size);
    bool is_empty();
    void insert(int key, int value);
    int remove(int key, int value);
    unordered_set <int> get(int key);
    void print();
};

class Linearhash{
    int Bucket_size = 0; 
    int r=0;
    float threshold = 0.75;
    float min_th    = 0.25;

    vector<Bucket*> bucket;  

    public:
    Linearhash(int bucket_size, int initial_buckets);
    
    int hash(int key);
    unordered_set<int> get(int x);
    void migrate(Bucket*from, Bucket* to, int &j, int &n_);
    void insert(int key, int value);
    void remove(int key, int value);
    void remove(int key);
    void print();
};