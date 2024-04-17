#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

struct Entry 
{
    size_t doc_id, count;

    // Required for tests:
    bool operator ==(const Entry& other) const 
    {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex 
{
    public:

    InvertedIndex() = default;

    void updateDocumentBase(const std::vector<std::string>& input_docs);

    std::vector<Entry> getWordCount(const std::string& word);

    size_t getWordCountInDoc(const std::string& word, const size_t doc_id) const; // ADDITIONAL METHOD

    private:

    void indexTheFile(const std::string& fileContent, size_t docId);

    std::map<int, std::string> document_list; // map of documents (unique id and file name) for search in
    static std::mutex mutexIndexMap; // access to index map
    std::map<std::string, std::vector<Entry>> frequencyDictionary; // frequency dictionary for all files
    bool indexingIsOngoing;
};