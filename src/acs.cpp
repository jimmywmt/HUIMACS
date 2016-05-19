/*
 * =====================================================================================
 *
 *       Filename:  acs.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/13 09時49分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu
 *   Organization:  UNLV
 *
 * =====================================================================================
 */

#include "acs.h"

using namespace huimacs;
using namespace std;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  ACS
 *      Method:  ACS
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
//ACS::ACS ()
//{
//}  /* -----  end of method ACS::ACS  (constructor)  ----- */
//
ACS::ACS ( const unsigned& iSize, Transaction* iLDataset, Node* iLSNode, double iInitialPheromone, double iRho, const double& iAlpha, const double& iBeta, const double& iQ0 ) {
	nIteration     = 1u;
	populationSize = iSize;
	lDataset       = iLDataset;
	lSNode         = iLSNode;
	initPheromone  = iInitialPheromone;
	rho            = iRho;
	alpha          = iAlpha;
	beta           = iBeta;
	q0             = iQ0;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  ACS
 *      Method:  ACS
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
//ACS::ACS ( const ACS &other )
//{
//}  /* -----  end of method ACS::ACS  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  ACS
 *      Method:  ~ACS
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
ACS::~ACS ()
{
}  /* -----  end of method ACS::~ACS  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  ACS
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	ACS&
ACS::operator = ( const ACS &other )
{
	if ( this != &other ) {
	}
	return *this;
}  /* -----  end of method ACS::operator =  (assignment operator)  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  ACS
 *      Method:  ACS :: getNiteration
 * Description:  
 *--------------------------------------------------------------------------------------
 */
unsigned ACS::getNiteration() {
	return nIteration;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  ACS
 *      Method:  ACS :: getLastFind
 * Description:  
 *--------------------------------------------------------------------------------------
 */
unsigned ACS::getLastFind() {
	return lastFind;
}
/*
 *--------------------------------------------------------------------------------------
 *       Class:  ACS
 *      Method:  ACS :: runIteration
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void ACS::runIteration() {
	Node* lCNode;
	vector<int>    cItemset;
	vector<int>*   lBestItemset = NULL;
	unsigned       bestUtility  = 0u;
	list<unsigned> relatedTransactions;

	for (unsigned i = 0u; i < populationSize; ++i) {
		lCNode = lSNode;
		cItemset.clear();
		relatedTransactions.clear();
		while (!lCNode->finish()) {
			lCNode = lCNode->selectNext(cItemset, alpha, beta, q0, pTable, initPheromone);
			lCNode->localUpdate(rho, initPheromone);
			if (!lCNode->calculated()) {
				auto result = lDataset->calculateUtility(relatedTransactions, cItemset);
				lCNode->setRelatedTransactions(relatedTransactions);
				if (get<1>(result)) {
					huis.push_back(make_pair(cItemset, get<0>(result)));
					if (bestUtility < get<0>(result)) {
						bestUtility = get<0>(result);
						if (lBestItemset != NULL) delete lBestItemset;
						lBestItemset = new vector<int>(cItemset);
					}
				} else if (!get<2>(result)) {
					pTable.insertRecord(cItemset);
					lCNode->clearRemainNodes();
				}
				if (cItemset.size() == 2u) Node::inputTwoTWU(cItemset.front(), cItemset.back(), get<3>(result));
				lCNode->setCalculated();
			} else {
				relatedTransactions = lCNode->getRelatedTransactions();
			}
		}
		if (lCNode->getName() == -1) break;
		Node::recurisivePrune(lCNode);
	}
	if (lBestItemset != NULL) {
		Node::globalUpdate (*lBestItemset, bestUtility, lDataset->getThreshold());
		delete lBestItemset;
		lastFind = nIteration;
	}
	++nIteration;
}
