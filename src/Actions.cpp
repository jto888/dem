#include "Actions.h"

Actions::Actions(std::shared_ptr<OpLineList>& OLL, SEXP actions_in)  {
	Rcpp::NumericVector actions_v = actions_in;
	int df_cols = actions_v[0];
					
	int df_rows = (int) actions_v.size()/df_cols;
	actions_v.erase(actions_v.begin());	
	

// this works with no need to wrap rot_actions on return
// and elements can be accessed in rot_actions[ ] [ ] notation.		
	std::vector<Rcpp::NumericVector> rot_actions;	
	for(int v=0; v<df_cols; v++) {
		rot_actions.push_back(Rcpp::wrap(actions_v.begin()+v*df_rows, std::next(actions_v.begin()+v*df_rows, df_rows)));	
	}
	
	// action_rows is now a member
	//std::vector<std::vector<double>> action_rows;
	
	for(int j=0; j<df_rows; j++) {
		std::vector<double> action_row;
		for(int i=0; i<(int) rot_actions.size(); i++) {
			action_row.push_back(rot_actions[i][j]);
		}
		action_rows.push_back(action_row);
		
		if(df_cols == 3) {
			int opline = (int) action_row[0];
			int target = (int) action_row[1];
			double probability = action_row[2];
			
			OLL->getByNum(opline)->addDirectImpact(target,probability);			
		}
					
	}
}

std::vector<std::vector<double>> Actions::getActionRows(){
	return action_rows;
}
