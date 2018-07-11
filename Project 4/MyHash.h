#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

#include <iostream>

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    struct bucket
    {
        KeyType k;
        ValueType v;
        bool used = false;
        bucket * next = nullptr;
    };
    
    bucket *m_buckets;
    double maxLF;
    int num_items = 0;
    int num_buckets;
};

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
    num_buckets = 6;
    m_buckets = new bucket[num_buckets];
    
    if (maxLoadFactor <= 0)
        maxLF = 0.5;
    else if (maxLoadFactor >= 2.0)
        maxLF = 2.0;
    else
        maxLF = maxLoadFactor;
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    delete [] m_buckets;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    delete [] m_buckets;
    num_buckets = 6;
    m_buckets = new bucket[num_buckets];
    num_items = 0;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType &key, const ValueType &value)
{
    unsigned int hash(const KeyType& k);
    
//    std::cout << key << " and " << num_items << std::endl;
    int bucketNum = hash(key) % num_buckets;
    
    if (!m_buckets[bucketNum].used)
    {   num_items++;
        m_buckets[bucketNum].k = key;
        m_buckets[bucketNum].v = value;
        m_buckets[bucketNum].used = true;   }
    
    else if (m_buckets[bucketNum].used && m_buckets[bucketNum].k == key)
    {   m_buckets[bucketNum].v = value;  }
    
    else if (m_buckets[bucketNum].used && m_buckets[bucketNum].k != key)
    {
        if (m_buckets[bucketNum].next == nullptr)
        {
            bucket *p;
            p = new bucket;
            p->k = key;
            p->v = value;
            p->used = true;
            m_buckets[bucketNum].next = p;
            num_items++;
        }
        
        else
        {
            bool added = false;
            bucket * cur;
            cur = m_buckets[bucketNum].next;
            
            while(cur->next != nullptr)
            {
                if (cur->k == key)
                { cur->v = value; added = true; break; }
                cur = cur->next;
            }
            
            if (!added)
            {
                if (cur->k == key)
                { cur->v = value; added = true; }
                else
                {
                    bucket *p;
                    p = new bucket;
                    p->k = key;
                    p->v = value;
                    p->used = true;
                    cur->next = p;
                    num_items++;
                }
            }
        }
    }
    
    if (getLoadFactor() > maxLF)
    {
        bucket *arr = new bucket[2*num_buckets];
        
        for (int i = 0; i < num_buckets; i++)
        {
            if (m_buckets[i].used == true)
            {
                bucket * cur = &m_buckets[i];
                
                while (cur != nullptr)
                {
                    int newBucketNum = hash(cur->k) % (2*num_buckets);
                
                    if (!arr[newBucketNum].used)
                    {
                        arr[newBucketNum].k = cur->k;
                        arr[newBucketNum].v = cur->v;
                        arr[newBucketNum].used = true;
                    }
                
                    else
                    {
                        bucket * c = &arr[newBucketNum];
                        while (c->next != nullptr)
                        {
                            c = c->next;
                        }
                        
                        bucket *p;
                        p = new bucket;
                        p->k = cur->k;
                        p->v = cur->v;
                        p->used = true;
                        c->next = p;
                    }
                    
                    cur = cur->next;
                }
            }
        }
        
        num_buckets *= 2;
        delete [] m_buckets;
        m_buckets = arr;
    }
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return num_items;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    double lf = static_cast<double>(num_items) / static_cast<double>(num_buckets);
    return lf;
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hash(const KeyType& k);
    ValueType *val = nullptr;
    
    int bucketNum = hash(key) % num_buckets;
    
    if (m_buckets[bucketNum].used && m_buckets[bucketNum].k == key)
        val = &m_buckets[bucketNum].v;
    else if (m_buckets[bucketNum].used && m_buckets[bucketNum].k != key)
    {
        bucket * cur = m_buckets[bucketNum].next;
        while (cur != nullptr)
        {
            if (cur->k == key)
            { val = &cur->v; break; }
            cur = cur->next;
        }
    }
    
    return val;
}


#endif // MYHASH_INCLUDED
