//
//  Map.cpp
//  CS32 Project 2
//
//  Created by Neta Tartakovsky on 1/28/18.
//  Copyright © 2018 Neta Tartakovsky. All rights reserved.
//

#include "Map.h"
using namespace std;

Map::Map()         // Create an empty map (i.e., one with no key/value pairs)
{
    m_size = 0;
    head = nullptr;
    tail = nullptr;
}

Map::Map(const Map& m)   // Copy constructor - makes a copy of the object that is passed by reference
{
    if (m.head == nullptr)
    {
        head = nullptr;
        tail = nullptr;
    }
    
    else
    {
        Node *n = new Node;
        n->m_key = m.head->m_key;
        n->m_value = m.head->m_value;
        n->prev = nullptr;
        n->next = nullptr;
        head = n;
        tail = n;
        
        Node *p;
        p = m.head->next;
        while (p != nullptr)
        {
            Node *j;
            j = head;
            while (j->next != nullptr)
                j = j->next;
            
            Node *k = new Node;
            k->m_key = p->m_key;
            k->m_value = p->m_value;
            k->next = nullptr;
            k->prev = j;
            j->next = k;
            tail = k;
            
            if (head == nullptr)
                head = k;
            
            p = p->next;
        }
    }
    
    m_size = m.m_size;
}

Map&Map:: operator= (const Map &that)   // Assignment operator
{
    if (this != &that)
    {
        Node *p;
        p = head;
        while (p != nullptr)
        {
            Node *n = p->next;
            delete p;
            p = n;
        }
        
        if (that.head == nullptr)
        {
            head = nullptr;
            tail = nullptr;
        }
        
        else
        {
            Node *n = new Node;
            n->m_key = that.head->m_key;
            n->m_value = that.head->m_value;
            n->prev = nullptr;
            n->next = nullptr;
            head = n;
            tail = n;
            
            Node *v;
            v = that.head->next;
            while (v != nullptr)
            {
                Node *j;
                j = head;
                while (j->next != nullptr)
                    j = j->next;
                
                Node *k = new Node;
                k->m_key = v->m_key;
                k->m_value = v->m_value;
                k->next = nullptr;
                k->prev = j;
                j->next = k;
                tail = k;
                
                if (head == nullptr)
                    head = k;
                
                v = v->next;
            }
        }
        
        m_size = that.m_size;
    }
    
    return *this;
}

Map::~Map()              // Delete the map
{
    Node *p;
    p = head;
    while (p != nullptr)
    {
        Node *n = p->next;
        delete p;
        p = n;
    }
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
    if (size() == 0)
        return true;
    return false;
}

int Map::size() const    // Return the number of key/value pairs in the map.
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, then insert and return true.
{
    Node *p;
    p = head;
    
    while (p != nullptr)
    {
        if (p->m_key == key)
            return false;
        p = p->next;
    }

    Node *n = new Node;
    n->m_key = key;
    n->m_value = value;
    n->next = head;
    n->prev = nullptr;
    head = n;
    
    
    if (tail == nullptr)
        tail = n;
    
    m_size++;
    
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
    Node *p;
    p = head;
    
    while (p != nullptr)
    {
        if (p->m_key == key)
        {
            p->m_value = value;
            return true;
        }
        p = p->next;
    }
    
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
{
    bool updateBool = update(key, value);
    
    if (!updateBool)
        insert(key, value);
    
    return true;
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
    if (head == nullptr)
        return false;
    
    if (head->m_key == key)
    {
        if (m_size == 1)
            tail = nullptr;
        Node *killMe = head;
        head = killMe->next;
        delete killMe;
        if (head != nullptr)
            head->prev = nullptr;
        m_size--;
        return true;
    }
    
    Node *p;
    p = head;
    while (p != nullptr)
    {
        if (p->next != nullptr && p->next->m_key == key)
            break;
        p = p->next;
    }
    
    if (p != nullptr)
    {
        if (tail == p->next)
            tail = p;
        Node *killMe = p->next;
        p->next = killMe->next;
        if (killMe->next != nullptr)
            killMe->next->prev = p;
        delete killMe;
        m_size--;
        return true;
    }
    
    return false;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
    Node *p;
    p = head;
    while (p != nullptr)
    {
        if (p->m_key == key)
            return true;
        p = p->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value)
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
    Node *p;
    p = head;
    while (p != nullptr)
    {
        if (p->m_key == key)
        {
            value = p->m_value;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
    if (i < 0 || i >= size())
        return false;

    Node *p;
    p = head;
    int k = 0;
    
    while (k < i)
    {
        p = p->next;
        k++;
    }
    
    key = p->m_key;
    value = p->m_value;
    return true;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
    Map copy(other);
    
    other = *this;
    
    *this = copy;
}

bool combine(const Map& m1, const Map& m2, Map& result)
// Combines two maps and stores the result in a third map
{
    while (!result.empty())
    {
        int count = 0;
        KeyType destroy;
        ValueType val;
        result.get(count, destroy, val);
        result.erase(destroy);
    }
    
    bool returnVal = true;
    
    for (int i = 0; i < m1.size(); i++)
    {
        KeyType key;
        ValueType value;
        m1.get(i, key, value);
        
        bool diffVal = true;
        
        for (int k = 0; k < m2.size(); k++)
        {
            KeyType key2;
            ValueType value2;
            m2.get(k, key2, value2);
            
            if (key == key2)
                if (value != value2)
                    diffVal = false;
        }
        
        if (diffVal)
            result.insert(key, value);
        else
            returnVal = false;
    }
    
    for (int j = 0; j < m2.size(); j++)
    {
        KeyType key;
        ValueType value;
        m2.get(j, key, value);
        
        if (!result.contains(key))
        {
            if (!m1.contains(key))
            {
                result.insert(key, value);
            }
        }
    }
    
    return returnVal;
}

void subtract(const Map& m1, const Map& m2, Map& result)
// Subtracts values stored in m2 from m1 and stores the results in a third map
{
    while (!result.empty())
    {
        int count = 0;
        KeyType destroy;
        ValueType val;
        result.get(count, destroy, val);
        result.erase(destroy);
    }
    
    for (int i = 0; i < m1.size(); i++)
    {
        KeyType key;
        ValueType value;
        m1.get(i, key, value);
        
        bool sameKey = false;
        
        for (int k = 0; k < m2.size(); k++)
        {
            KeyType key2;
            ValueType value2;
            m2.get(k, key2, value2);
            
            if (key == key2)
                sameKey = true;
        }
        
        if (!sameKey)
            result.insert(key, value);
    }
}












