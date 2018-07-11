
#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const string& s) const;
private:
    vector<char> m_separators;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i = 0; i < separators.size(); i++)
    {
        m_separators.push_back(separators[i]);
    }
}

vector<string> TokenizerImpl::tokenize(const string& s) const
{
    vector<string> tokens;
    string add = "";
    
    for (int i = 0; i < s.size(); i++)
    {
        bool isSeparator = false;
        for (int j = 0; j < m_separators.size(); j++)
        {
            if (s[i] == m_separators[j])
                isSeparator = true;
        }
        
        if (isSeparator)
        {
            if (add != "")
            {
                tokens.push_back(add);
                add = "";
            }
        }
        else
            add += s[i];
        
        if (i == s.size()-1 && add != "")
            tokens.push_back(add);
    }
    
    return tokens;  // This compiles, but may not be correct
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const string& s) const
{
    return m_impl->tokenize(s);
}
