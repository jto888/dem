#include "dem.h"
#include "OpLine.h"
#include "OpLineList.h"

	OpLineList::OpLineList(std::unique_ptr<ElementList>& EL) {
			using namespace Rcpp;	
			
		std::vector<int> oplines;
		for(int i=0; i< EL->getSize(); i++) {
			oplines.push_back(EL->getByIndex(i)->getOplineNum());
		}
		// preparing a sorted, unique vector of oplines	by number (as recorded in input dataframe)
		std::vector<int> opline_v=oplines;
		std::sort(opline_v.begin(), opline_v.end());
		std::vector<int>::iterator it;
		it = std::unique(opline_v.begin(), opline_v.end());
		opline_v.resize(distance(opline_v.begin(),it));
		// build the oplines vector by constructing unique OpLine objects in a push_back
		for(int i=0; i < (int) opline_v.size(); i++) {
			_oplines.push_back(std::make_shared<OpLine>(EL, opline_v[i]));	
		}
	}
	
	int OpLineList::getSize() {
		return _oplines.size();
	}
	
	std::shared_ptr<OpLine> OpLineList::getByIndex(int index) {
		return _oplines[index];	
	}
	
	std::shared_ptr<OpLine>  OpLineList::getByNum(int num) {
// Ideally this should be done with a std iterator
// if num is not valid the first OpLine will be returned without warning
		int index=0;
		for(int i=0; i<(int) _oplines.size(); i++)  {
			if(_oplines[i]->getNum() == num)  {
				index=i;
				break;
			}
		}
		return _oplines[index];	
	}


	
	
