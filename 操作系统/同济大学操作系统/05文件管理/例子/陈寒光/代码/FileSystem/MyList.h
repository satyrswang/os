#ifndef CHEN_LIST
#define CHEN_LIST

#include <list>
namespace FS_STD{

	template < typename _T>
	class List: public std::list<_T>
	{
	public :
		List<_T>::iterator find( _T * pTarget )	;

	};

	template<typename _T>
	List<_T>::iterator
	List<_T>::
	find( _T * pTarget ){
			List<_T>::iterator iter = this->begin();
			List<_T>::iterator iter_end = this->end();
			for(; iter != iter_end; iter ++ ){
				if( *pTarget == *iter ){
					return iter;
				}
			}
			return NULL;
	}


}


#endif
