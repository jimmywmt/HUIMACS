/*
 * =====================================================================================
 *
 *       Filename:  node.cpp
*
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/11 20時05分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu (wmt@wmt35.idv.tw)
 *   Organization:  UNLV
 *
 * =====================================================================================
 */
#include "node.h"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

using namespace huimacs;
using namespace std;

/* 
 * Initial class static variable
 */
Node*                                  Node::lStartNode     = NULL;
vector<unsigned>                       Node::twoTWU;
vector<Transaction::oneTWU>*           Node::lOneTWU;
vector<list<pair<unsigned, unsigned>>> Node::indexTable;
const size_t                           Node::sizeIndexTable = 1024u;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
//Node::Node ()
//{
//}  /* -----  end of method Node::Node  (constructor)  ----- */

Node::Node ( Node* iLParentNode, const int& iName, const list<int>& iFollowingNodes, const double& initPheromone ) {
	lParentNode          = iLParentNode;
	name                 = iName;
	followingNodes       = move(iFollowingNodes);
	pheromone            = initPheromone;
	maxCheckPTable       = 0u;
	calculate            = false;
	for (const auto& i : iFollowingNodes) {
		remainNodes.push_back(make_pair(i, (Node*)NULL));
	}
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
//Node::Node ( const Node &other )
//{
//}  /* -----  end of method Node::Node  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  ~Node
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
Node::~Node ()
{
	for (auto& i : remainNodes) {
		if (i.second != NULL)
			delete i.second;
		
	}
}  /* -----  end of method Node::~Node  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
	Node&
Node::operator = ( const Node &other )
{
	if (this != &other) {
	}
	return *this;
}  /* -----  end of method Node::operator =  (assignment operator)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: getStartNode
 * Description:  
 *--------------------------------------------------------------------------------------
 */
const Node* Node::getStartNode() {
	return Node::lStartNode;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: getName
 * Description:  
 *--------------------------------------------------------------------------------------
 */
int Node::getName () {
	return name;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: getPheromone
 * Description:  
 *--------------------------------------------------------------------------------------
 */
double Node::getPheromone () {
	return pheromone;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: positiveDeleteFolloingNode
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void  Node::positivePrue ( const vector<int> itemset, const PTable& pTable ) {
	if (maxCheckPTable < pTable.maxVersion()) {
		list<list<pair<int, Node*>>::iterator> locationRecord;
		for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
			if (i->second == NULL) {
				vector<int> tempItemset(itemset);
				tempItemset.push_back(i->first);
				if (pTable.checkTable(tempItemset, maxCheckPTable))
					locationRecord.push_back(i);
			}
		}
		maxCheckPTable = pTable.maxVersion();

		for (auto i : locationRecord)
			remainNodes.erase(i);
	}
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: localUpdate
 * Description:  
 *
 *--------------------------------------------------------------------------------------
 */
void Node::localUpdate ( const double& rho, const double& initPheromone ) {
	pheromone = pheromone * (1.0 - rho) + initPheromone * rho;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: globalUpdate
 * Description:  
 *
 *--------------------------------------------------------------------------------------
 */
void Node::globalUpdate ( const vector<int> itemset, const unsigned& utility, const double& threshold ) {
	Node* lNode = Node::lStartNode;
	bool find;

	for (auto i : itemset) {
		find = false;
		for (auto j : lNode->remainNodes) {
			Node *lFNode = j.second;
			if (lFNode != NULL) {
				if (i == lFNode->getName()) {
					find = true;
					lNode = lFNode;
					lNode->addPheromone(utility, threshold);
					break;
				}
			}
		}
		if (!find)
			break;
	}
}
 
/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: initStartNode
 * Description:  
 *--------------------------------------------------------------------------------------
 */
Node* Node::initStartNode ( const list<int>& iFollowingNodes, const double& initPheromone ) {
	Node::lStartNode = new Node(NULL, -1, iFollowingNodes, initPheromone);
	return Node::lStartNode;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: finish
 * Description:  
 *--------------------------------------------------------------------------------------
 */
bool Node::finish () {
	return (remainNodes.size() == 0u);
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: calculated
 * Description:  
 *--------------------------------------------------------------------------------------
 */
bool  Node::calculated() {
	return calculate;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: setCalculated
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::setCalculated() {
	calculate = true;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: selectNext
 * Description:  
 *--------------------------------------------------------------------------------------
 */
Node* Node::selectNext(vector<int>& cItemset, const double& alpha, const double& beta, const double& q0, const PTable& pTable, const double& initPheromone) {
	if (cItemset.size() > 1u)
		positivePrue(cItemset, pTable);

	if (finish()) {
		return this;
	}

	vector<tuple<list<pair<int, Node*>>::iterator, double, double, double>> weightTable;	//double: heuristic, double: pheromone, double: weight
	double pheromone;
	double totalWeight = 0.0;
	double maxWeight = 0.0;
	list<pair<int, Node*>>::iterator iSelectedNode;
	if (cItemset.size() == 0u) {
		for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
			unsigned index = getCandidateIndex(i->first);
			if (i->second != NULL) {
				pheromone = i->second->pheromone;
			} else {
				pheromone = initPheromone;
			}
			weightTable.push_back(make_tuple(i, (*lOneTWU)[index].twUtility, pheromone, 0.0));
		}
	} else {
		unsigned t1TWU = 0u;
		unsigned t2TWU = 0u;
		list<unsigned> no2TWU;
		for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
			if (i->second != NULL) {
				pheromone = i->second->pheromone;
			} else {
				pheromone = initPheromone;
			}
			unsigned index1 = getCandidateIndex(name);
			unsigned index2 = getCandidateIndex(i->first);
			unsigned oTWU = (*lOneTWU)[index2].twUtility;
			unsigned tTWU = twoTWU[index1 * (*lOneTWU).size() + index2 - (index1 + 2) * (index1 + 1) / 2];
			if (tTWU != 0u) {
				t1TWU += oTWU;
				t2TWU += tTWU;
				weightTable.push_back(make_tuple(i, tTWU, pheromone, 0.0));
			} else {
				no2TWU.push_back((unsigned)weightTable.size());
				weightTable.push_back(make_tuple(i, oTWU, pheromone, 0.0));
			}
		}
		if (no2TWU.size() != 0u && no2TWU.size() < remainNodes.size()) {
			double a1TWU = (double)t1TWU / (double)(remainNodes.size() - no2TWU.size());
			double a2TWU = (double)t2TWU / (double)(remainNodes.size() - no2TWU.size());
			for (auto& i : no2TWU) {
				get<1>(weightTable[i]) = (get<1>(weightTable[i]) / a1TWU) * a2TWU;
			}
		}
	}
	for (auto& i : weightTable) {
		get<3>(i) = pow(get<1>(i), alpha) * pow(get<2>(i), beta);
		totalWeight += get<3>(i);
        if (get<3>(i) > maxWeight) {
		maxWeight = get<3>(i);
		iSelectedNode = get<0>(i);
        }
	}
	default_random_engine random((unsigned)time(NULL));
	uniform_real_distribution<double> dis(0.0, 1.0);
	if (dis(random) >= q0) {
		//roulette wheel selection
		uniform_real_distribution<double> dis2(0.0, totalWeight);
		double length = dis2(random);
		auto ball = weightTable.begin();
		do {
			length -= get<3>(*ball);
			if (length <= 0.0) {
				iSelectedNode = get<0>(*ball);
			} else {
				++ball;
			}
		} while (length > 0.0);
	}
	if (iSelectedNode->second == NULL) {
		list<int> sFollowingNodes;
		auto it = find(followingNodes.begin(), followingNodes.end(), iSelectedNode->first);
		++it;
		for (auto i = it; i != followingNodes.end(); ++i) {
			sFollowingNodes.push_back(*i);
		}
		iSelectedNode->second = new Node(this, iSelectedNode->first, sFollowingNodes, initPheromone);
	}

	cItemset.push_back(iSelectedNode->first);
	return iSelectedNode->second;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: clearFollowing
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void  Node::clearRemainNodes() {
	remainNodes.clear();
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: setRelatedTransactions
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void  Node::setRelatedTransactions(const list<unsigned> transactions) {
	relatedTransactions = transactions;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: getRelatedTransactions
 * Description:  
 *--------------------------------------------------------------------------------------
 */
list<unsigned> Node::getRelatedTransactions() {
	return relatedTransactions;
}
/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: initTwoTWU
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::initTwoTWU ( const size_t& nCandidate ) {
	//due to positive prunning rule, we can set 0u for pair itemset which is never arrived
	twoTWU.resize((nCandidate * (nCandidate - 1)) / 2, 0u);
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: setLOneTWU
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::setLOneTWU ( vector<Transaction::oneTWU>* ILOneTWU ) {
	lOneTWU = ILOneTWU;
}


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: setCandidateIndex
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::setCandidateIndex () {
	indexTable.resize(sizeIndexTable);
	for (size_t i = 0u; i < lOneTWU->size(); ++i) {
		unsigned name = (*lOneTWU)[i].name;
		indexTable[name % sizeIndexTable].push_back(make_pair(name, i));
	}
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: getCandidateIndex
 * Description:  
 *--------------------------------------------------------------------------------------
 */
unsigned Node::getCandidateIndex ( const unsigned& name ) {
	unsigned lot = name % sizeIndexTable;
	for (const auto& i : indexTable[lot]) {
		if (i.first == name)
			return i.second;
	}
	return (unsigned)lOneTWU->size();
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: inputTwoTWU
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::inputTwoTWU ( const int& name1, const int& name2, const unsigned& utility ) {
	unsigned index1 = getCandidateIndex(name1);
	unsigned index2 = getCandidateIndex(name2);
	twoTWU[index1 * (*lOneTWU).size() + index2 - (index1 + 2) * (index1 + 1) / 2] = utility;
}
/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: recursivePrune
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::recurisivePrune ( Node* node ) {
	Node* lPreviousNode = node->getLParentNode();
	lPreviousNode->deleteFollowingNode(node);
	delete node;
	if (lPreviousNode->getName() != -1 && lPreviousNode->finish())
		recurisivePrune(lPreviousNode);
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: deleteFollowingNode
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::deleteFollowingNode ( Node* lNode ) {
	for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
		if (i->first == lNode->getName()) {
			remainNodes.erase(i);
			break;
		}
	}
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Node
 *      Method:  Node :: getLParentNode
 * Description:  
 *--------------------------------------------------------------------------------------
 */
Node* Node::getLParentNode () {
	return lParentNode;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PTable
 *      Method:  PTable :: addPheromone
 * Description:  
 *--------------------------------------------------------------------------------------
 */
void Node::addPheromone ( const unsigned& utility, const double& threshold ) {
	pheromone += (double)utility / threshold;
}
