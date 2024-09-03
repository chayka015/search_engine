#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#pragma once

#include "converterjson.h"
#include "invertedIndex.h"

using namespace std;

class SearchServer {
	public:
	explicit SearchServer(InvertedIndex& _idx) : idx(_idx) {};

	SearchServer(const SearchServer& other) { idx = other.idx; };

	SearchServer& operator=(const SearchServer& other) {
		if (this == &other)
			return *this;
		idx = other.idx;
		return *this;
	};

	~SearchServer() = default;

	vector<vector<pair<size_t, float>>> search(const vector<string>& queriesInput);

private:
	InvertedIndex idx;
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H
