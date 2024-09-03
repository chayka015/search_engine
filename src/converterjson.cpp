#include "../include/converterjson.h"

using namespace std;

vector<string> ConverterJSON::GetTextDocuments() {
	nlohmann::json configJsonDic = getDicFromJsonFile("config.json");

	vector<string> docFilePathList, docTextList;

	for (const auto& it : configJsonDic["files"]) {
		ifstream docFile("../" + string(it));
		if (!docFile.is_open()) {
			cout << "Wrong Path " << "../" + string(it) <<
			" or file doesn't exist!" << endl;
			break;
		} else {
			string text;
			while (getline(docFile, text));
			docFile.close();
			docTextList.push_back(text);
		}
	}

	return docTextList;
}

int ConverterJSON::GetResponsesLimit() {
	nlohmann::json configJsonDic = getDicFromJsonFile("config.json");

	return configJsonDic["config"]["max_responses"];
}

vector<string> ConverterJSON::GetRequests() {

	nlohmann::json requestsInJsonDic;

	vector<string> requestsList;

	requestsInJsonDic = getDicFromJsonFile("requests.json");

	for (const auto& it : requestsInJsonDic["requests"]) {
		requestsList.push_back(it);
	}

	return requestsList;
}

void ConverterJSON::putAnswers(const vector<vector<pair<size_t, float>>>& _answers) {

	string answersStr = "{\n\t\"answers\":{\n\t\t";
	for (int i = 0; i < _answers.size(); ++i) {
		float count = 0;
		for (auto &it: _answers[i]) {
			if (it.second > count) {
				count += it.second;
				break;
			}
		}
		if (count == 0) {
			answersStr += "\"request00" + to_string(i) + "\":{\n\t\t\t" + R"("result":"false")";
		} else {
			answersStr += "\"request00" + to_string(i) + "\":{\n\t\t\t" + R"("result":"true",)"
				+ "\n\t\t\t\"relevance\":[\n\t\t\t\t";
			for (int j = 0; j < _answers[i].size(); ++j) {
				answersStr += "{\n\t\t\t\t\t\"docid\":" +
					to_string(_answers[i][j].first) + ",\"rank\":" +
					to_string(_answers[i][j].second) + "\n\t\t\t\t}";
				if (j < _answers[i].size() - 1)
					answersStr += ",\n\t\t\t\t";
			}
			answersStr += "\n\t\t\t]";
		}
		if (i < _answers.size() - 1) {
			answersStr += "\n\t\t},\n\t\t";
		} else
			answersStr += "\n\t\t}";
	}
	answersStr += "\n\t}\n}";

	cout << answersStr << endl;

	putDicToJsonFile("answers.json", nlohmann::json::parse(answersStr));
}