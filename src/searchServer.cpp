#include "../include/searchServer.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <thread>

using namespace std;

bool isEqualFloat(float x, float y) {
	return (fabs( x - y ) <=
	numeric_limits<float>::epsilon() * fmax(fabs(x), fabs(y)));
}

vector<vector<pair<size_t, float>>> SearchServer::search(
	const vector<string>& _queriesInput) {
	ConverterJSON converterJson;
	vector<vector<pair<size_t, float>>> relativeIndexVecAllQueries, topRelativeIndexes;

	if (!_queriesInput.empty()) {
		for (const auto &query: _queriesInput) {
			vector<pair<size_t, float>> relativeIndexVec;
			vector<pair<string, vector<Entry>>> freqVec;

			vector<string> queryWords = idx.convertTextToUniqWords(query);
			vector<thread> threadVec;
			for (auto &wordIt: queryWords) {
				threadVec.push_back(thread([&, wordIt]{
					vector<Entry> sortedIndexVec;
					sortedIndexVec = idx.GetWordCount(wordIt);

					sort(sortedIndexVec.begin(),
							  sortedIndexVec.end(),
							  [](auto &left, auto &right) {
								  return left.count < right.count;
							  });
					freqVec.emplace_back(wordIt, sortedIndexVec);
				}));
			}
			for (auto& thrd : threadVec) if (thrd.joinable()) thrd.join();
			size_t relevanceAbsMax = 0;
			vector<pair<size_t, size_t>> relevanceAbsVec;
			for (int id = 0; id < idx.GetDocsSize(); ++id) {
				size_t relevanceAbsolute = 0;
				for (auto& d: freqVec) {
					for (auto& e: d.second) {
						if (e.docId == id) {
							relevanceAbsolute += e.count;
							if (relevanceAbsolute > relevanceAbsMax)
								relevanceAbsMax = relevanceAbsolute;
							break;
						}
					}
				}
				if (relevanceAbsolute > 0)
					relevanceAbsVec.emplace_back(id, relevanceAbsolute);
			}

			for (auto &it: relevanceAbsVec) {
				relativeIndexVec.emplace_back(it.first, float(it.second) / float(relevanceAbsMax));
			}
			relativeIndexVecAllQueries.emplace_back(relativeIndexVec);

			for (auto &idxVec: relativeIndexVecAllQueries) {
				for (int i = 0; i < idxVec.size(); ++i) {
					for (int j = 0; j < idxVec.size() - 1; ++j) {
						if (isless(idxVec[j].second, idxVec[j + 1].second)) {
							swap(idxVec[j], idxVec[j + 1]);
						}
					}
				}
			}
		}

		converterJson.putAnswers(relativeIndexVecAllQueries);

		for (auto &itVec: relativeIndexVecAllQueries) {
			vector<pair<size_t, float>> tempVec;
			int respLimit = converterJson.GetResponsesLimit();
			for (int j = 0; (j < itVec.size() && j < respLimit); ++j) {
				if (!isEqualFloat(itVec[j].second, 0))
					tempVec.emplace_back(itVec[j]);
				else
					tempVec.emplace_back();
			}
			topRelativeIndexes.push_back(tempVec);
		}
	} else {
		cout << "_queriesInput file is EMPTY!" << endl;
	}
	return topRelativeIndexes;
}
