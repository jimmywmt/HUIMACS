/*
 * =====================================================================================
 *
 *       Filename:  transaction.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/12 13時53分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu (wmt@wmt35.idv.tw)
 *   Organization:  UNLV
 *
 * =====================================================================================
 */


#ifndef  HUIMACS_TRANSACTION_H_INC
#define  HUIMACS_TRANSACTION_H_INC

#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <tuple>

namespace huimacs {


	/*
	 * =====================================================================================
	 *        Class:  Transaction
	 *  Description:  
	 * =====================================================================================
	 */
	class Transaction
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			Transaction () = delete;                             /* constructor      */
			Transaction ( const double& iThresholdRatio );
			Transaction ( const Transaction &other ) = delete;   /* copy constructor */
			~Transaction ();                            /* destructor       */

			/* ====================  ACCESSORS     ======================================= */
			const double& getThreshold () const;
			size_t size () const;
			size_t itemCount () const;

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

			Transaction& operator = ( const Transaction &other ); /* assignment operator */

			bool readData ( const std::string& path );
			std::tuple<unsigned, bool, bool, unsigned> calculateUtility ( std::list<unsigned>& relatedTransaction, const std::vector<int>& cItemset );

			/*-----------------------------------------------------------------------------
			 *  struct for 1-HTWUIs
			 *-----------------------------------------------------------------------------*/
			struct oneTWU {
				int    name;
				double twUtility;
			};
			std::vector<oneTWU> oneTWUs;

			std::list<int> oneTWUsName;
		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */
			struct record {
				unsigned                            utility;
				std::list<std::pair<int, unsigned>> items;	//first: name, second: item utility
			};

			std::vector<record>              data;
			std::vector<std::list<unsigned>> invertTable;
			unsigned                         totalUtility;
			double                           thresholdRatio;
			double                           threshold;

	}; /* -----  end of class Transaction  ----- */


}

#endif   /* ----- #ifndef HUIMACS_TRANSACTION_H_INC  ----- */
