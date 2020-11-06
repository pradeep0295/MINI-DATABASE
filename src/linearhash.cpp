#include"global.h"

Bucket::Bucket(int size){
    this->avail.assign(size, true);
    this->KeyValue.assign(size,{0,0});
}
    
bool Bucket::is_empty(){
    if(this->size >0) return false;
    if(!this->overflow) return true;
    return this->overflow->is_empty();
}
void Bucket::insert(int key, int value){
    for(int i=0;i<this->avail.size();i++){
        if(this->avail[i]){
            this->size++;
            this->KeyValue[i] = {key,value};
            this->avail[i] = false;
            return;
        }
    }
    /* Propagate Insert */
    if(!this->overflow){             
        this->overflow = new Bucket(this->avail.size());
    }
    this->overflow->insert(key,value);    
}

int Bucket::remove(int key, int value){
    int d = 0;
    for(int i=0;i<this->avail.size();i++){
        if(this->avail[i]) continue;
        if(this->KeyValue[i].first == key && (value<0 || this->KeyValue[i].second == value)){
            this->size--;
            d++;
            this->avail[i] = true;
        }
    }
    if(!this->overflow) return d;
    /* Propagate remove */                  
    int ret = this->overflow->remove(key,value);       
    if(this->overflow->size ==0){
        Bucket* temp = this->overflow;
        this->overflow = this->overflow->overflow;
        delete temp;
    }
    return ret+d;
}
unordered_set <int> Bucket::get(int key){
    unordered_set<int> ret;
    for(int i=0;i<this->avail.size();i++)
        if(!this->avail[i] && this->KeyValue[i].first == key)
            ret.insert(this->KeyValue[i].second);
    if(this->overflow){ 
        auto of = this->overflow->get(key);
        ret.insert(of.begin(), of.end());
    }
    return ret;
}

void Bucket::print(){
    for(int i=0;i<this->avail.size();i++){
        if(this->avail[i] == false){
            cout<<this->KeyValue[i].first<<":"<<this->KeyValue[i].second<<" ";
        }
    }
    cout<<"|";
    if(this->overflow) this->overflow->print();
}


Linearhash::Linearhash(int bucket_size, int initial_buckets){ // min one initial bucket
    Bucket_size = bucket_size;
    this->initial_buckets = initial_buckets;
    for(int i=0;i<initial_buckets;i++)
        bucket.push_back(new Bucket(Bucket_size));
}
/* Can change it further but as database is integer only key is integer */
int Linearhash::hash(int key){
    int i = ceil(log2((int)bucket.size()));
    /* no hash used */
    int k = key;      
    int m = k % (int)pow(2, i);
    if(m>=bucket.size())
        m = k % (int)pow(2, i-1);   
    return m;
}

unordered_set<int> Linearhash::get(int x){
    return this->bucket[hash(x)]->get(x);
}

void Linearhash::migrate(Bucket*from, Bucket* to, int &j, int &n_){
    if(!from) return;
    migrate(from->overflow,to,j,n_);
    for(int i=0;i<from->avail.size();i++){
        if(from->avail[i]) continue;

        int k = hash(from->KeyValue[i].first);
        int m = k % (int)pow(2,j);

        if(m == n_){
            to->insert(from->KeyValue[i].first, from->KeyValue[i].second);
            from->size--;
            from->avail[i] = true;  
        }
        if(from->overflow && from->overflow->size ==0){
                Bucket* temp = from->overflow;
                from->overflow = from->overflow->overflow;
                delete temp;
        }
    }
}

void Linearhash::insert(int key, int value){
    this->bucket[hash(key)]->insert(key,value);
    r++;
    /* check occupancy and increase buckets */
    if((float)r/(bucket.size()*Bucket_size) > threshold){
        /* Allocate new bucket */
        int n_ = bucket.size();
        int i = ceil(log2(n_));
        int ndash = n_ % (int)pow(2,i-1);
        bucket.push_back(new Bucket(Bucket_size));

        /* Re-hash ndash bucket keys to n, if they belong to new bucket n */
        int j = ceil(log2(n_+1)); 

        migrate(bucket[ndash],bucket[n_],j,n_);
    }
}

void Linearhash::remove(int key, int value){
    int d = this->bucket[hash(key)]->remove(key,value);
    r-=d;

    /* check occupancy and increase buckets */
    while(bucket.size()>this->initial_buckets && (float)r/(bucket.size()*Bucket_size) < min_th){
        /* Allocate new bucket */
        int n_ = bucket.size()-1;

        /* Re-hash n_ bucket keys to its suffix-1(.) (or) ndash bucket*/
        int j = ceil(log2(n_));
        int ndash = n_ % (int)pow(2,j-1);

        Bucket* head = bucket[n_];
        while(head){ 
            for(int i=0;i<head->avail.size();i++){
                if(head->avail[i]) continue;
                bucket[ndash]->insert(head->KeyValue[i].first,head->KeyValue[i].second);
            }
            head =head->overflow;
        }
        head = bucket[n_];
        bucket.pop_back();
        delete head;
    }
}

void Linearhash::remove(int key){
    this->remove(key,-1);
}
void Linearhash::print(){
    int free=0;
    for(int i=0;i<bucket.size();i++){
        if(!bucket[i]->is_empty()){
            bucket[i]->print();
            cout<<endl;
        }
        else free++;
    }
    cout<<"total buckets:"<<this->bucket.size()<<" free:"<<free<<endl;
}

// int main(){
//     int key,value,x;
//     void *h = NULL;
//     h = static_cast<void*>(new Linearhash(2,2));
    
//     for(int i=0;i<10;i++){
//         for(int j=0;j<100;j++){
//             static_cast<Linearhash*>(h)->insert(j,i+j);
//         }
//     }
//     static_cast<Linearhash*>(h)->print();
//     cout<<"............."<<endl;

//     for(int i=0;i<10;i++){
//         for(int j=0;j<100;j++){
//             static_cast<Linearhash*>(h)->remove(j,i+j);
//         }
//         static_cast<Linearhash*>(h)->print();
//         cout<<"......."<<endl;
//     }
    
//     return 0;
// }
