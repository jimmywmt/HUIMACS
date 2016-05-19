/*
 * =====================================================================================
 *
 *       Filename:  ptable.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/12 09時38分31秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu 
 *   Organization:  UNLV
 *
 * =====================================================================================
 */

#include "ptable.h"
#include <algorithm>

using namespace huimacs;
using namespace std;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  PTable
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
PTable::PTable ()
{
}  /* -----  end of method PTable::PTable  (constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  PTable
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
//PTable::PTable ( const PTable &other )
//{
//}  /* -----  end of method PTable::PTable  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  ~PTable
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
PTable::~PTable ()
{
}  /* -----  end of method PTable::~PTable  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	PTable&
PTable::operator = ( const PTable &other )
{
	if (this != &other) {
	}
	return *this;
}  /* -----  end of method PTable::operator =  (assignment operator)  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  PTable :: maxVersion
 * Description:  
 *--------------------------------------------------------------------------------------
 */
unsigned PTable::maxVersion() const {
	return (unsigned)table.size();
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  PTable :: checkTable
 * Description:  
 *--------------------------------------------------------------------------------------
 */
bool PTable::checkTable ( std::vector<int>& itemset, const unsigned& version ) const {
	bool result = false;
	if (table.size() != 0u) {
		bool alreadySort       = false;
		if (version < table.size()) {
			for (unsigned i = version; i < table.size(); ++i) {
				if (table[i].size() < itemset.size() && table[i].size() != 0u) {
					if (!alreadySort) {
						sort(itemset.begin(), itemset.end());
						alreadySort = true;
					}
					if (includes(itemset.begin(), itemset.end(), table[i].begin(), table[i].end())) {
						result = true;
						break;
					}
				}
			}
		}
	}

	return result;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  PTable :: insertRecord
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void PTable::insertRecord ( std::vector<int>& itemset ) {
	list<vector<vector<int>>::iterator> lErase;
	sort(itemset.begin(), itemset.end());
	for (auto& i : table) {
		if (itemset.size() < i.size()) {
			if (includes(i.begin(), i.end(), itemset.begin(), itemset.end()))
				i.clear();
		}
	}
	table.push_back(itemset);
}
