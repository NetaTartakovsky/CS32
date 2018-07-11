#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <fstream>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
    
private:
    MyHash<string, list<string>> m_words;
};

bool WordListImpl::loadWordList(string filename)
{
    m_words.reset();
    ifstream infile(filename);
    
    if (!infile)
        return false;
    
    for(;;)
    {
        string s;
        getline(infile, s);
        
        bool validWord = true;
        if (s.size() == 0)
            validWord = false;
        for (size_t i = 0; i < s.size(); i++)
        {
            if (!isalpha(s[i]) && s[i] != '\'')
                validWord = false;
            else if (isalpha(s[i]))
            {
                string rep = "";
                rep += tolower(s[i]);
                s.replace(i, 1, rep);
            }
        }
        
        if (validWord)
        {
            vector<char> letterPos;
            string format = "";
            for (int i = 0; i < s.size(); i++)
            {
                char pos;
                bool containsLetter = false;
                int j = 0;
                
                for (j = 0; j < letterPos.size(); j++)
                {
                    if (letterPos[j] == s[i])
                    {
                        containsLetter = true;
                        break;
                    }
                }
                
                if (!containsLetter)
                {
                    letterPos.push_back(s[i]);
                    pos = 'A' + letterPos.size()-1;
                    format += pos;
                }
                else
                {
                    pos = 'A' + j;
                    format += pos;
                }
            }
            
            list<string> val;
            
            if (m_words.find(format) == nullptr)
            {
                val.push_front(s);
            }
            
            else
            {
                val = *m_words.find(format);
                val.push_front(s);
            }
            
            m_words.associate(format, val);
        }
        
        if (infile.eof())
            break;
    }
    
    return true;
}

bool WordListImpl::contains(string word) const
{
    // Makes all the alphabetical letters in the given word lowercase
    for (size_t i = 0; i < word.size(); i++)
    {
        if (isalpha(word[i]))
        {
            char r = tolower(word[i]);
            string rep = "";
            rep += r;
            word.replace(i, 1, rep);
        }
    }
    
    // Gets the letter pattern of the word given
    vector<char> letterPos;
    string format = "";
    for (size_t i = 0; i < word.size(); i++)
    {
        char pos;
        bool containsLetter = false;
        int j = 0;
        
        for (j = 0; j < letterPos.size(); j++)
        {
            if (letterPos[j] == word[i])
            {
                containsLetter = true;
                break;
            }
        }
        
        if (!containsLetter)
        {
            letterPos.push_back(word[i]);
            pos = 'A' + letterPos.size()-1;
            format += pos;
        }
        else
        {
            pos = 'A' + j;
            format += pos;
        }
    }
    
    if (m_words.find(format) != nullptr)
    {   const list<string> val = *m_words.find(format);
        list<string>::const_iterator it = val.begin();
        while (it != val.end())
        {
            if (*it == word)
                return true;
            it++;
        }
    }
    
    return false;
}
                                                                                                                                                                                            
vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> candidates;
    vector<string> empty;

    // Gets letter pattern of cipherWord
    vector<char> letterPos;
    string format = "";
    for (size_t i = 0; i < cipherWord.size(); i++)
    {
        char pos;
        bool containsLetter = false;
        int j = 0;

        for (j = 0; j < letterPos.size(); j++)
        {
            if (letterPos[j] == tolower(cipherWord[i]))
            {
                containsLetter = true;
                break;
            }
        }

        if (!containsLetter)
        {
            letterPos.push_back(tolower(cipherWord[i]));
            pos = 'A' + letterPos.size()-1;
            format += pos;
        }
        else
        {
            pos = 'A' + j;
            format += pos;
        }
    }

    if (m_words.find(format) != nullptr)
    {
        const list<string> val = *m_words.find(format);
        list<string>::const_iterator it = val.begin();
        while (it != val.end())
        {
            string w = *it;
            bool matches = true;
            for (size_t i = 0; i < currTranslation.size(); i++)
            {
                if (isalpha(currTranslation[i]))
                {
                    if (!isalpha(cipherWord[i]))
                    { return empty; }
                    else if (w[i] != tolower(currTranslation[i]))
                    { matches = false; break; }
                }
                else if (currTranslation[i] == '?')
                {
                    if (!isalpha(cipherWord[i]))
                    { return empty; }
                    else if (!isalpha(w[i]))
                    { matches = false; break; }
                }
                else if (currTranslation[i] == '\'')
                {
                    if (cipherWord[i] != '\'')
                    { return empty; }
                    else if (w[i] != '\'')
                    { matches = false; break; }
                }
            }
            
            if (matches)
            {
                candidates.push_back(w);
            }
            
            it++;
        }
    }
    
    return candidates;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}


