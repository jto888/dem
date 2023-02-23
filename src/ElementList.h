#ifndef _elementlist_H
#define _elementlist_H

#include "dem.h"
#include "Element.h"

class ElementList {		
	private:	
		std::vector<std::shared_ptr<class Element>> _elementList;
		
		
		
	public:	
		ElementList(SEXP model_in, SEXP years_in);
		int getSize();
		std::shared_ptr<Element> getByIndex(int index);
		std::shared_ptr<Element> getByID(int id);
		
};		

#endif
