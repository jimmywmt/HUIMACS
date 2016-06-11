/*
 * =====================================================================================
 *
 *       Filename:  node.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/11 20時09分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu (wmt@wmt35.idv.tw)
 *   Organization:  UNLV
 *
 * =====================================================================================
 */

#ifndef  HUIMACS_NODE_INC
#define  HUIMACS_NODE_INC

#include "ptable.h"
#include "transaction.h"
#include <cstdlib>
#include <list>
#include <vector>

namespace huimacs {


	/*
	 * =====================================================================================
	 *        Class:  Node
	 *  Description:  
	 * =====================================================================================
	 */
	class Node
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			Node  () = delete;				/* constructor      */
			Node  ( Node* iLParentNode, const int& iName, const std::list<int>& iFollowingNodes, const double& initPheromone );
			Node  ( const Node &other ) = delete;		/* copy constructor */
			~Node ();				/* destructor       */

			/* ====================  ACCESSORS     ======================================= */
			static const Node* getStartNode();

			int    getName () const;
			double getPheromone () const;
			void   setRelatedTransactions ( const std::list<unsigned> transactions );
			std::list<unsigned> getRelatedTransactions ();

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			Node& operator = ( const Node &other ); /* assignment operator */

			static void     recurisivePrune ( Node* node );
			static void     globalUpdate ( const std::vector<int> itemset, const unsigned& utility, const double& threshold );
			static Node*    initStartNode ( const std::list<int>& iFollowingNodes, const double& initPheromone );
			static void     deleteStartNode ();
			static void     initTwoTWU ( const size_t& nCandidate );
			static void     setLOneTWU ( std::vector<Transaction::oneTWU>* ILOneTWU );
			static void     setCandidateIndex ();
			static unsigned getCandidateIndex ( const unsigned& name );
			static void     inputTwoTWU ( const int& name1, const int& name2, const unsigned& utility );

			void  positivePrue ( const std::vector<int> itemset, const PTable& pTable );
			void  localUpdate ( const double& rho, const double& initPheromone );
			bool  finish ();
			bool  calculated ();
			void  setCalculated ();
			Node* selectNext ( std::vector<int>& cItemset, const double& alpha, const double& beta, const double& q0, const PTable& pTable, const double& initPheromone );
			void  clearRemainNodes ();


		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */
			static Node*                                                 lStartNode;
			static std::vector<unsigned>                                 twoTWU;
			static std::vector<Transaction::oneTWU>*                     lOneTWU;
			static std::vector<std::list<std::pair<unsigned, unsigned>>> indexTable;
			const static size_t                                          sizeIndexTable;

			int                              name;
			unsigned                         maxCheckPTable;
			double                           pheromone;
			Node*                            lParentNode;
			std::list<int>                   followingNodes;
			std::list<std::pair<int, Node*>> remainNodes;
			std::list<unsigned>              relatedTransactions;
			bool                             calculate;

			void  deleteFollowingNode ( Node* lNode );
			Node* getLParentNode ();
			void  addPheromone ( const unsigned& utility, const double& threshold );
	}; /* -----  end of class Node  ----- */


}

#endif   /* ----- #ifndef HUIMACS_NODE_INC  ----- */

