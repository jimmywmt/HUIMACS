/*
 * =====================================================================================
 *
 *       Filename:  transaction.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/12 14時53分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu (wmt@wmt35.idv.tw)
 *   Organization:  UNLV
 *
 * =====================================================================================
 */

#include "transaction.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace huimacs;
using namespace std;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  Transaction
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
//Transaction::Transaction ()
//{
//}  /* -----  end of method Transaction::Transaction  (constructor)  ----- */

Transaction::Transaction ( const double& iThresholdRatio ) {
	totalUtility = 0;
	thresholdRatio = iThresholdRatio;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  Transaction
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
//Transaction::Transaction ( const Transaction &other )
//{
//}  /* -----  end of method Transaction::Transaction  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  ~Transaction
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
Transaction::~Transaction ()
{
}  /* -----  end of method Transaction::~Transaction  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	Transaction&
Transaction::operator = ( const Transaction &other )
{
	if (this != &other) {
	}
	return *this;
}  /* -----  end of method Transaction::operator =  (assignment operator)  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  Transaction :: getThreshold
 * Description:  
 *--------------------------------------------------------------------------------------
 */
const double& Transaction::getThreshold () {
	return threshold;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  Transaction :: size
 * Description:  
 *--------------------------------------------------------------------------------------
 */
size_t Transaction::size () const {
	return data.size();
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  Transaction :: itemCount
 * Description:  
 *--------------------------------------------------------------------------------------
 */
size_t Transaction::itemCount () const {
	if (invertTable.front().size() == 0u)
		return invertTable.size() - 1u;
	else
		return invertTable.size();
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  Transaction :: readData
 * Description:  
 *--------------------------------------------------------------------------------------
 */
bool Transaction::readData ( const std::string& path ) {
	ifstream file(path);
	if (!file.is_open())
		return false;

	while (!file.eof()) {
		string line;
		getline(file, line);
		if (line.length() != 0u) {
			size_t firstSemicolon = line.find(":");
			size_t secondSemicolon = line.find_last_of(":");
			struct record aRecord;
			stringstream ssName, ssTotalUtility, ssUtility;

			ssName << line.substr(0u, firstSemicolon);
			ssTotalUtility << line.substr(firstSemicolon + 1, secondSemicolon - firstSemicolon - 1);
			ssUtility << line.substr(secondSemicolon + 1, line.length() - secondSemicolon - 1);

			ssTotalUtility >> aRecord.utility;
			totalUtility += aRecord.utility;
			while (!ssName.eof()) {
				unsigned name;
				unsigned utility;
				ssName >> name;
				ssUtility >> utility;
				aRecord.items.push_back(make_pair(name, utility));
			}
			data.push_back(aRecord);
		}
	}

	//calculate the threshold
	threshold = (double)totalUtility * thresholdRatio;

	//fill invert table
	for (unsigned i = 0u; i < data.size(); ++i) {
		struct record aRecord = data[i];
		for (const auto& j : aRecord.items) {
			if (invertTable.size() < j.first + 1u)
				invertTable.resize(j.first + 1u);
			invertTable[j.first].push_back(i);
		}
	}

	//build 1-HTWUIs
	for (unsigned i = 0u; i < invertTable.size(); ++i) {
		struct oneTWU twu;
		twu.name = i;
		twu.twUtility = 0;
		for (const auto& j : invertTable[i]) {
			twu.twUtility += data[j].utility;
		}
		if (twu.twUtility > threshold)
			oneTWUs.push_back(twu);
	}
	sort(oneTWUs.begin(), oneTWUs.end(), [] ( const oneTWU& i, const oneTWU& j ) { return i.twUtility < j.twUtility; });
	for (const auto& i : oneTWUs) {
		oneTWUsName.push_back(i.name);
	}

	return true;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Transaction
 *      Method:  Transaction :: calculateUtility
 * Description:  
 *--------------------------------------------------------------------------------------
 */
tuple<unsigned, bool, bool, unsigned> Transaction::calculateUtility ( list<unsigned>& relatedTransaction, const vector<int>& cItemset ) {
	unsigned name = cItemset.back();
	unsigned utility = 0u;
	unsigned twUtility = 0u;
	bool isHUI = false;
	bool passThreshold = true;
	const auto& itemRelatedTransaction = invertTable[name];
	vector<int> itemset(cItemset);
	if (cItemset.size() == 1u) {
		for (const auto& i : itemRelatedTransaction) {
			relatedTransaction.push_back(i);
		}
	} else {
		list<unsigned> newRelatedTransaction;
		auto index1 = relatedTransaction.begin();
		auto index2 = itemRelatedTransaction.begin();
		while (index1 != relatedTransaction.end() && index2 != itemRelatedTransaction.end())
		{
			if (*index1 < *index2) ++index1;
			else if (*index2 < *index1) ++index2;
			else {
				newRelatedTransaction.push_back(*index1);
				++index1; ++index2;
			}
		}
		relatedTransaction = move(newRelatedTransaction);
	}
	sort(itemset.begin(), itemset.end());
	for (const auto& i : relatedTransaction) {
		twUtility += data[i].utility;
		const auto& items = data[i].items;
		auto index1 = itemset.begin();
		auto index2 = items.begin();
		while (index1 != itemset.end()) {
			if (index2->first < *index1) ++index2;
			else if (index2->first == *index1) {
				utility += index2->second;
				++index1; ++index2;
			}
		}
	}
	if (utility >= threshold) isHUI = true;
	else if (twUtility < threshold) passThreshold = false;

	return make_tuple(utility, isHUI, passThreshold, twUtility);
}
