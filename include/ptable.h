/*
 * =====================================================================================
 *
 *       Filename:  ptable.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/05/11 22時40分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jimmy Ming-Tai Wu (wmt@wmt35.idv.tw)
 *   Organization:  UNLV
 *
 * =====================================================================================
 */

#ifndef  HUIMACS_PTABLE_INC
#define  HUIMACS_PTABLE_INC

#include <cstdlib>
#include <vector>
#include <list>

namespace huimacs {


	/*
	 * =====================================================================================
	 *        Class:  PTable
	 *  Description:  
	 * =====================================================================================
	 */
	class PTable
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			PTable  ();				/* constructor      */
			PTable  ( const PTable &other ) = delete;	/* copy constructor */
			~PTable ();				/* destructor       */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

			PTable& operator = ( const PTable &other ); /* assignment operator */

			unsigned maxVersion () const;
			bool checkTable ( std::vector<int>& itemset, const unsigned& version ) const;
			void insertRecord ( std::vector<int>& itemset );

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */
			std::vector<std::vector<int>> table;

	}; /* -----  end of class PTable  ----- */


}

#endif   /* ----- #ifndef HUIMACS_PTABLE_INC  ----- */

