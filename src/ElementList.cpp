#include "dem.h"
#include "Element.h"
#include "ElementList.h"

ElementList::ElementList(SEXP model_in, SEXP years_in) {
		using namespace Rcpp;					
						
	int years=Rcpp::as<int>(years_in);					
						
// We know there are 11 columns in the model dataframe	Plus 1 for oplsz					
// So 12 is hard coded here to get the number of rows						
	Rcpp::NumericVector model_v = model_in;					
	int df_rows = (int) model_v.size()/12;					
						
	std::vector<double> opline_v(model_v.begin(), std::next(model_v.begin(), df_rows));	
	std::vector<double> id_v(model_v.begin()+df_rows, std::next(model_v.begin()+df_rows, df_rows));
	std::vector<double> fd_v(model_v.begin()+2*df_rows, std::next(model_v.begin()+2*df_rows, df_rows));
	std::vector<double> fp1_v(model_v.begin()+3*df_rows, std::next(model_v.begin()+3*df_rows, df_rows));
	std::vector<double> fp2_v(model_v.begin()+4*df_rows, std::next(model_v.begin()+4*df_rows, df_rows));
	std::vector<double> fp3_v(model_v.begin()+5*df_rows, std::next(model_v.begin()+5*df_rows, df_rows));
	std::vector<double> rd_v(model_v.begin()+6*df_rows, std::next(model_v.begin()+6*df_rows, df_rows));
	std::vector<double> rp1_v(model_v.begin()+7*df_rows, std::next(model_v.begin()+7*df_rows, df_rows));
	std::vector<double> rp2_v(model_v.begin()+8*df_rows, std::next(model_v.begin()+8*df_rows, df_rows));
	std::vector<double> rp3_v(model_v.begin()+9*df_rows, std::next(model_v.begin()+9*df_rows, df_rows));
	std::vector<double> seed_v(model_v.begin()+10*df_rows, std::next(model_v.begin()+10*df_rows, df_rows));
	std::vector<double> oplsz_v(model_v.begin()+10*df_rows, std::next(model_v.begin()+11*df_rows, df_rows));
	
	
	std::vector<std::vector<double>>  model =	
	{opline_v, id_v, fd_v, fp1_v, fp2_v, fp3_v, rd_v, rp1_v, rp2_v, rp3_v, seed_v, oplsz_v};
	
	for(int j=0; j<df_rows; j++) {
		std::vector<double> model_row;
		for(int i=0; i<(int) model.size(); i++) {
			model_row.push_back(model[i][j]);
		}
			//_elementList.push_back(new Element(model_row,years)); //compiler error
			//_elementList.push_back(std::shared_ptr<Element>(new Element(model_row,years)));		
			//std::make_shared makes one memory allocation for both the object and data structure
			//required for reference counting i.e. new operator will called only once.
			_elementList.push_back(std::make_shared<Element>(model_row,years));
	}

}	

int ElementList::getSize() {
	return _elementList.size();
}

std::shared_ptr<Element> ElementList::getByIndex(int index) {
	return _elementList[index];
}

std::shared_ptr<Element> ElementList::getByID(int id) {	
	std::shared_ptr<Element> ret_el;
	for(int i=0; i< (int) _elementList.size(); i++) {
		if(_elementList[i]->getID() == id) {
			ret_el = _elementList[i];
			break;
		}
	}
	return ret_el;
}
