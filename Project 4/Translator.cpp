#include "provided.h"
#include <string>
#include <unordered_map>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
    
private:
    unordered_map<char, char> m_translation;
    vector<unordered_map<char, char>> mapStack;
    int numPushed = 0;
    int numPopped = 0;
};

TranslatorImpl::TranslatorImpl()
{
    for (int i = 65; i < 91; i++)
    {
        m_translation[i] = '?';
    }
    
    unordered_map<char, char> temp = m_translation;
    mapStack.push_back(temp);
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    
    // Checks if they are the same length
    if (ciphertext.length() != plaintext.length())
        return false;
    
    vector<char> cLetters;
    vector<char> ptLetters;
    for (int i = 0; i < ciphertext.size(); i++)
    {
        // Checks if they only contain letters
        if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
        { return false; break; }
        
        char r = toupper(ciphertext[i]);
        string rep = "";
        rep += r;
        ciphertext.replace(i, 1, rep);
        r = toupper(plaintext[i]);
        rep = "";
        rep += r;
        plaintext.replace(i, 1, rep);
        
        // Checks if the cipher letter already maps to a different plain text letter
        if (m_translation[ciphertext[i]] != '?' && m_translation[ciphertext[i]] != plaintext[i])
        { return false; break; }
    }
    
    // Checks if a cipher letter already maps to one of the plaintext letters
    unordered_map<char, char>::iterator it = m_translation.begin();
    while (it != m_translation.end())
    {
        for (int i = 0; i < plaintext.size(); i++)
        {
            if (it->second == plaintext[i])
            {
                if (it->first != ciphertext[i])
                { return false; break; }
            }
        }
        it++;
    }
    
    for (int i = 0; i < plaintext.size(); i++)
    {
        m_translation[ciphertext[i]] = plaintext[i];
    }

    unordered_map<char, char> temp = m_translation;
    mapStack.push_back(temp);
    
    numPushed++;
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (numPushed == numPopped)
        return false;
    
    mapStack.pop_back();
    m_translation = mapStack.back();
    numPopped++;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string result = "";
    for (int i = 0; i < ciphertext.size(); i++)
    {
        if (isalpha(ciphertext[i]))
        {
            unordered_map<char, char>::const_iterator it = m_translation.begin();
            while (it != m_translation.end())
            {
                if (it->first == toupper(ciphertext[i]))
                {
                    if (isalpha(it->second))
                    {
                        if (isupper(ciphertext[i]))
                            result += it->second;
                        else
                            result += tolower(it->second);
                    }
                    else
                        result += '?';
                }
                it++;
            }
        }
        else
            result += ciphertext[i];
    }
    
    return result;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
