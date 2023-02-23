#include "dem.h"
#include "Maintenance.h"
#include "MaintList.h"

MaintList::MaintList(SEXP maint_in) {
		using namespace Rcpp;					
						
//	int years=Rcpp::as<int>(years_in);					
						
// We know there are 5 columns in the maint dataframe				
// So 5 is hard coded here to get the number of rows						
	Rcpp::NumericVector maint_v = maint_in;					
	int df_rows = (int) maint_v.size()/5;
	
	std::vector<double> num_v(maint_v.begin(), std::next(maint_v.begin(), df_rows));							
	std::vector<double> opline_v(maint_v.begin()+df_rows, std::next(maint_v.begin()+df_rows, df_rows));	
	std::vector<double> interval_v(maint_v.begin()+2*df_rows, std::next(maint_v.begin()+2*df_rows, df_rows));		
	std::vector<double> duration_v(maint_v.begin()+3*df_rows, std::next(maint_v.begin()+3*df_rows, df_rows));		
	std::vector<double> first_interval_v(maint_v.begin()+4*df_rows, std::next(maint_v.begin()+4*df_rows, df_rows));
	
	
	std::vector<std::vector<double>>  maint =	
	{num_v, opline_v, interval_v, duration_v, first_interval_v};
	
	for(int j=0; j<df_rows; j++) {
		std::vector<double> maint_row;
		for(int i=0; i<(int) maint.size(); i++) {
			maint_row.push_back(maint[i][j]);
		}

			_maintList.push_back(std::make_shared<Maintenance>(maint_row));
	}

}	

int MaintList::getSize() {
	return _maintList.size();
}

std::shared_ptr<Maintenance> MaintList::getByIndex(int index) {
	return _maintList[index];
}

std::shared_ptr<Maintenance> MaintList::getByID(int id) {	
	std::shared_ptr<Maintenance> ret_maint;
	for(int i=0; i< (int) _maintList.size(); i++) {
		if(_maintList[i]->getNum() == id) {
			ret_maint = _maintList[i];
			break;
		}
	}
	return ret_maint;
}
