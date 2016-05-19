/*
 * =====================================================================================
 *
 *       Filename:  acs.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/12 23時40分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu
 *   Organization:  UNLV
 *
 * =====================================================================================
 */


#ifndef  HUIMACS_ACS_H_INC
#define  HUIMACS_ACS_H_INC

#include "transaction.h"
#include "node.h"
#include "ptable.h"

namespace huimacs {

	/*
	 * =====================================================================================
	 *        Class:  ACS
	 *  Description:  
	 * =====================================================================================
	 */
	class ACS
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			ACS () = delete;                    /* constructor      */
			ACS ( const unsigned& iSize, Transaction* iLDataset, Node* iLSNode, double iInitialPheromone, double iRho, const double& iAlpha, const double& iBeta, const double& iQ0 );
			ACS ( const ACS &other ) = delete;  /* copy constructor */
			~ACS ();                            /* destructor       */

			/* ====================  ACCESSORS     ======================================= */
			unsigned getNiteration();
			unsigned getLastFind();

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

			ACS& operator = ( const ACS &other ); /* assignment operator */

			void runIteration();

			std::vector<std::pair<std::vector<int>, unsigned>> huis;

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			unsigned     nIteration;
			unsigned     populationSize;
			double       initPheromone;
			double       rho;
			double       alpha;
			double       beta;
			double       q0;
			Transaction* lDataset;
			PTable       pTable;
			Node*        lSNode;
			unsigned     lastFind;
			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class ACS  ----- */


}

#endif   /* ----- #ifndef HUIMACS_ACS_H_INC  ----- */

