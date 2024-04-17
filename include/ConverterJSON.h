#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"

class ConverterJSON
{
    public:

    static ConverterJSON* getInstance();

    std::vector<std::string> getTextDocuments();
    int getResponsesLimit() const;
    std::vector<std::string> getRequests();

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

    void readConfigFile(std::string path = "config.json");

    void readRequestFile(std::string path = "requests.json");

    int getMaxResponses() const;

    private:
    ConverterJSON() = default; // private constructor for singleton realization
    static ConverterJSON* instance;
    const std::string ANSWERS_FILE_PATH = "answers.json";
    std::string applicationName;
    std::string applicationVersion;
    int maxResponses  {5};
    std::vector<std::string> resourcesPaths;
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;
};
