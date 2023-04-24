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

OpLineList::OpLineList(std::unique_ptr<ElementList>& EL, SEXP dep_in) {		
		using namespace Rcpp;
		
	std::vector<int> oplines;	
	for(int i=0; i< EL->getSize(); i++) {	
		oplines.push_back(EL->getByIndex(i)->getOplineNum());
	}	
	// preparing a sorted, unique vector of oplines by number (as recorded in input dataframe)	
	std::vector<int> opline_v=oplines;	
	std::sort(opline_v.begin(), opline_v.end());	
	std::vector<int>::iterator it;	
	it = std::unique(opline_v.begin(), opline_v.end());	
	opline_v.resize(distance(opline_v.begin(),it));	
	// build the oplines vector by constructing unique OpLine objects in a push_back	
	for(int i=0; i < (int) opline_v.size(); i++) {	
		_oplines.push_back(std::make_shared<OpLine>(EL, opline_v[i]));
	}	
		
	// read in the dependency vector	
	Rcpp::IntegerVector deps_v = dep_in;	
	int df_cols = deps_v[0];	
	int df_rows = (int) deps_v.size()/df_cols;	
	deps_v.erase(deps_v.begin());	
			
	// reconstruct the dataframe as a vector of NumericVectors		
	//std::vector<Rcpp::IntegerVector>  dep_rows;		
	for(int i=0; i<df_rows; i++) {		
		Rcpp::IntegerVector dep_row(df_cols);	
		for(int j=0; j<df_cols; j++) {	
			dep_row[j] = deps_v[i+j*df_rows];
		}	
		dep_rows.push_back(dep_row);	
	}		
	
	// place dependency data in each applicable OpLine		
	for(int i=0; i<df_rows; i++) {		
		int opline = dep_rows[i][0];	
		if(df_rows==2 || dep_rows[i][1]==0) {	
			//OLL->getByNum(opline)->addDirectDependent(dep_rows[i][ncol-1]);/
			getByNum(opline)->addDirectDependent(dep_rows[i][df_cols-1]);
		}else{	
			Rcpp::IntegerVector co_enabled_row = dep_rows[i];
			co_enabled_row.erase(co_enabled_row.begin());
			//OLL->getByNum(opline)->addCoEnabledDependent(co_enabled_row);
			getByNum(opline)->addCoEnabledDependent(co_enabled_row);
		}	
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


	Rcpp::IntegerVector OpLineList::getDepRow(int row_num) {
		return dep_rows[row_num];
	}
	
