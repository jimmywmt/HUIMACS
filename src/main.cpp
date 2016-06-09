/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/11 20時05分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu (wmt@wmt35.idv.tw)
 *   Organization:  UNLV
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "transaction.h"
#include "node.h"
#include "acs.h"
#include "cmdline.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */

using namespace std;
using namespace chrono;
using namespace huimacs;

int main ( int argc, char *argv[] )
{
	cmdline::parser a;
	time_t now = time(0);
	a.add<string>("file", 'f', "database file path", true);
	a.add<double>("threshold", 't', "high utility itemset threshold", true);
	a.add<unsigned>("max", 'm', "maximum iteration", false, 10000);
	a.add<unsigned>("size", 's', "population size", false, 20);
	a.add<double>("pheromone", 'p', "inital pheromone", false, 1.0);
	a.add<double>("rho", 'r', "rho", false, 0.4);
	a.add<double>("alpha", 'a', "alpha", false, 1.0);
	a.add<double>("beta", 'b', "beta", false, 3.0);
	a.add<double>("q0", 'q', "q0", false, 0.8);
	a.add<unsigned>("interval", 'i', "result interval", false, 100);
	a.add("write", 'w', "write file");
	a.parse_check(argc, argv);    

	ostream* output(&cout);
	if (a.exist("write")) {
		tm *ltm = localtime(&now);
		string outpath = a.get<string>("file") + " " + to_string(ltm->tm_mon + 1) + "-" + to_string(ltm->tm_mday) + "-" + to_string(ltm->tm_year + 1900) + " " + to_string(ltm->tm_hour) + " " + to_string(ltm->tm_min) + " " + to_string(ltm->tm_sec + 1) + ".log";
		ofstream* outputFile = new ofstream(outpath);
		if (outputFile->is_open()) {
			output = outputFile;
		}
	}

	const unsigned maxIteration = a.get<unsigned>("max");
	Transaction tran(a.get<double>("threshold"));
	tran.readData(a.get<string>("file"));
	Node* sNode = Node::initStartNode(tran.oneTWUsName, a.get<double>("pheromone"));
	Node::initTwoTWU(tran.oneTWUsName.size());
	Node::setLOneTWU(&tran.oneTWUs);
	Node::setCandidateIndex();
	ACS ants(a.get<unsigned>("size"), &tran, sNode, a.get<double>("pheromone"), a.get<double>("rho"), a.get<double>("alpha"), a.get<double>("beta"), a.get<double>("q0"));

	*output << "Timestamp: " << ctime(&now);
	*output << "Database: " << a.get<string>("file") << endl;
	*output << "Transaction Count: " << tran.size() << endl;
	*output << "Item Count: " << tran.itemCount() << endl;
	*output << "Threshold Ratio: " << a.get<double>("threshold") << endl;
	*output << "Ant Population: " << a.get<unsigned>("size") << endl;
	*output << "Max Generations: " << a.get<unsigned>("max") << endl;
	*output << "Initial Pheromone Density: " << a.get<double>("pheromone") << endl;
	*output << "Pseudo-Random Proportional Rule Parameter: " << a.get<double>("q0") << endl;
	*output << "Local Updating Rule Parameter: " << a.get<double>("rho") << endl << endl;
	*output << "Processing Ant Colony System Algorithm..." << endl;
	auto pStart = steady_clock::now();
	while (!sNode->finish() && ants.getNIteration() <= maxIteration) {
		ants.runIteration();
		if (output == &cout)
			*output << "\rIteration:\t" << ants.getNIteration() << " / " << maxIteration << "\t\t "<< "NUM of HUIs:\t" << ants.huis.size() << flush;
		if (ants.getNIteration() % a.get<unsigned>("interval") == 0) {
			*output << "\rIteration:\t" << ants.getNIteration() << " / " << maxIteration << "\t\t "<< "NUM of HUIs:\t" << ants.huis.size() << endl;
		}
	}
	if (sNode->finish())
		*output << endl << "Algorithm terminated on generation " << ants.getNIteration() << " (ALL HUIs discovered)" << endl;
	else
		*output << endl << "Algorithm terminated on maximum generation" << endl;
	auto pEnd = steady_clock::now();
	auto diff = pEnd - pStart;
	*output << "Time elapsed: " << chrono::duration_cast<seconds>(diff).count() << " seconds " << endl;

	auto huis = ants.huis;
	if (huis.size() > 0u) {
		*output << endl << huis.size() << " High-Utility Itemsets discovered" << endl;
		*output << "Last HUI found on generation: " << ants.getLastFind() << endl << endl;

		for (auto i : huis) {
			*output << "[ ";
			sort(i.first.begin(), i.first.end());
			for (auto j : i.first) *output << j << " ";
			*output << "]" << endl;
			*output << "Utility: " << i.second << endl;
		}
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

