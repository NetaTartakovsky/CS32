
#include "provided.h"
#include <string>
#include <vector>

using namespace std;

class DecrypterImpl
{
public:
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    string separators = ",;:.!()[]{}-\"#$%^& 1234567890";
    WordList w;
    Translator t;
    int count = 0;
    int numMaps = 1;
    vector<string> chosenWords;
    vector<string> output;
};

bool DecrypterImpl::load(string filename)
{
    if (w.loadWordList(filename))
        return true;
    return false;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    count++;
    if (count <= 1)
    {
        while (numMaps > 1)
        { t.popMapping(); numMaps--; }
        output.clear();
        chosenWords.clear();
    }
    
    while (count <= chosenWords.size())
    {
        chosenWords.pop_back();
    }
    
    vector<string> empty;
    Tokenizer tok(separators);
    vector<string> tokens = tok.tokenize(ciphertext);
    
    bool hasBeenChosen = false;
    int maxNumUntranslated = 0;
    string maxWordUntranslated = "";
    for (int i = 0; i < tokens.size(); i++)
    {
        int numUntranslated = 0;
        for (int j = 0; j < chosenWords.size(); j++)
        {
            if (tokens[i] == chosenWords[j])
            { hasBeenChosen = true; break; }
        }
        if (!hasBeenChosen)
        {
            string translation = t.getTranslation(tokens[i]);
            for (int k = 0; k < translation.size(); k++)
            {
                if (translation[k] == '?')
                    numUntranslated++;
            }
            if (numUntranslated > maxNumUntranslated)
            { maxNumUntranslated = numUntranslated; maxWordUntranslated = tokens[i]; }
            if (numUntranslated == maxNumUntranslated)
            {
                if (tokens[i].size() > maxWordUntranslated.size())
                    maxWordUntranslated = tokens[i];
            }
        }
        hasBeenChosen = false;
    }
    
    chosenWords.push_back(maxWordUntranslated);
    
    string translatedToken = t.getTranslation(maxWordUntranslated);
    
    vector<string> candidates = w.findCandidates(maxWordUntranslated, translatedToken);
    if (candidates.empty())
    {
        if (numMaps > 1)
        { t.popMapping(); numMaps--; }
        count--;
        return empty;
    }
    
    for (int i = candidates.size()-1; i >= 0; i--)
    {
        if (t.pushMapping(maxWordUntranslated, candidates[i]))
        {
            numMaps++;
            string fullTranslation = t.getTranslation(ciphertext);
            vector<string> translatedTokens = tok.tokenize(fullTranslation);
            
            bool containsAll = true;
            bool completelyTranslated = true;
            for (int j = 0; j < translatedTokens.size(); j++)
            {
                bool wordTranslated = true;
                for (int k = 0; k < translatedTokens[j].size(); k++)
                {
                    if (translatedTokens[j][k] == '?')
                        wordTranslated = false;
                }
                if (wordTranslated && !w.contains(translatedTokens[j]))
                { containsAll = false; }
                if (wordTranslated == false)
                    completelyTranslated = false;
            }
            
            if (!containsAll)
            {
                if (numMaps > 1)
                { t.popMapping(); numMaps--; }
            }
            else if (!completelyTranslated)
            {
                crack(ciphertext);
            }
            else if (completelyTranslated)
            {
                output.push_back(fullTranslation);
                if (numMaps > 1)
                { t.popMapping(); numMaps--; }
            }
        }
    }
    
    if (numMaps > 1)
    { t.popMapping(); numMaps--; }
    
    count--;
    return output;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}

