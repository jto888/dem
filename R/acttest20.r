   acttest20<-function(Model,SimulationYears = 10,SimulationYearsPerPage = 10, Actions=NULL, Maint=NULL,				
	   ProgRpt=FALSE) { 				
	 				
	  				
					
		  ShowProgress=FALSE			
		  thisRNGkind="Marsaglia-Multicarry"			
					
		## validate input dataframe for Type1 Model			
		Type1_Names<-c("OpLine","EventID","FD","FP1","FP2","FP3","RD","RP1","RP2","RP3","Seed")			
		Input_Names<-names(Model)			
					
		if(length(Input_Names)!=length(Type1_Names))  {			
		stop("Error in stosim:    incorrect input dataframe length to demtest")			
		} else {			
		for(n in 1:length(Type1_Names)) {			
		if(Input_Names[n]!=Type1_Names[n]) stop("Error in stosim:   incorrect input dataframe names to demtest") }			
		}			
					
		Pages=as.integer(SimulationYears/SimulationYearsPerPage)			
		if(Pages<1) {			
		stop("Error in stosim:  SimulationYears are less than SimulationYearsPerPage")			
		}			
					
		  if(SimulationYearsPerPage>2000)  {
		## Actually accuracy might be compromised over 500 years per page
		    stop("Error in stosim:  Accuracy lost over 2000 years per page")			
		  }			
		  			
		  if(length(unique(Model[,2])) != length(Model[,2])) {			
			stop("EventID's are not unique")		
		  }			
					
		FD_DF<-NULL			
		## Parse FD to convert type to a code			
		for(x in 1:length(Model[,3]))  {			
		if (Model[x,3]=="E")  FD_DF<-rbind(FD_DF,1)			
		if (Model[x,3]=="N")  FD_DF<-rbind(FD_DF,2)			
		if (Model[x,3]=="W")  FD_DF<-rbind(FD_DF,3)			
		## if (Model[x,3]=="L")  FD_DF<-rbind(FD_DF,4)			
		if(length(FD_DF[,1])<x) stop("Error in stosim:   FD parameter not recognized")			
		} 			
		Model[,3]<-FD_DF[,1]			
	  				
		RD_DF<-NULL			
		## Parse RD to convert type to a code## enumerate the distribution types for numeric entry			
	  	for(x in 1:length(Model[,7]))  {			
		##if (Model[x,7]=="E")  RD_DF<-rbind(RD_DF,1)			
		if (Model[x,7]=="N")  RD_DF<-rbind(RD_DF,2)			
		if (Model[x,7]=="W")  RD_DF<-rbind(RD_DF,3)			
		if (Model[x,7]=="L")  RD_DF<-rbind(RD_DF,4)			
		if(length(RD_DF[,1])<x) stop("Error in stosim:   RD parameter not recognized")			
		}			
		Model[,7]<-RD_DF[,1]			
	  				
		StackedModel<-NULL			
		for(column in 1:ncol(Model))  {			
			StackedModel<-c(StackedModel, Model[,column])		
		}
		
		
		if(!is.null(Maint)) {
				#validate column names for Maint dataframe
				MaintNames<-c("MaintID", "OpLine", "Interval", "Duration", "FirstInterval")
				inputNames<-names(Maint)
			if(length(inputNames)!=length(MaintNames))  {			
				stop("Error in stosim:  incorrect input Maint dataframe to demtest")
			}
			for(n in 1:length(MaintNames)) {			
				if(inputNames[n]!=MaintNames[n]) stop("Maint object must have specified column names") 		
			}				
							
			 if(nrow(Maint)!=length(unique(Maint$MaintID))) {			
				stop("MaintID's are not unique")		
			 }	
				
			StackedMaint<-NULL
			for(column in 1:ncol(Maint))  {	
				StackedMaint<-c(StackedMaint, Maint[,column])
			}
		}
		
		
		
		if(!is.null(Actions)) {		   		
			test_actions_names<-c("OpLine", "Target", "Probability")

			input_actions_names<-names(Actions)[c(1,ncol(Actions)-1, ncol(Actions))]
			if(length(input_actions_names)!=length(test_actions_names))  {			
				stop("incorrect input Actions dataframe")
			}
			for(n in 1:length(test_actions_names)) {			
				if(input_actions_names[n]!=test_actions_names[n]) stop("incorrect input Actions dataframe names") 		
			}	

			StackedActions<-ncol(Actions)
			for(column in 1:ncol(Actions))  {	
				StackedActions<-c(StackedActions, Actions[,column])
			}
		}
			
		
		

## opline size vector to be added to StackedModel on each pass of pages		
		oplsz<-sapply(Model$OpLine, function (X)  length(Model$OpLine[Model$OpLine==X]))			
					
		##	OutputDF=NULL		
		## even proc.time has a problem on CRAN example run			
		if(ProgRpt==TRUE)  {			
			startTime<-proc.time()		
		}			
			  		
		summaryDF <- NULL			
		historyDF <- NULL			
		headspaceVec <- NULL 

			## establish the rng.kind to be carried into the Rcpp call		
			RNGkind(kind=thisRNGkind)
			
				
		## the main loop through pages providing new seed values for each page			
	for(p in 1:Pages)  {				
					
			if(p>1) { 		
			#Seed_Vec<-Seed_Vec +3 		
			Mlen<-nrow(Model)		
			SMlen<-length(StackedModel)		
			Seed_Vec<-StackedModel[(SMlen-Mlen+1):SMlen]		
			Seed_Vec<-Seed_Vec +3 		
			StackedModel[(SMlen-Mlen+1):SMlen]<-Seed_Vec		
			}		
						
			StackedModel_oplsz <- c(StackedModel, oplsz)	
			
if(is.null(Maint) && is.null(Actions)) {				
		 ## this is the old call to the unregistered C++ code in the stosim library			
			fun_out<-.Call("Basicdem", StackedModel_oplsz, SimulationYearsPerPage, PACKAGE="dem")		
}
if(!is.null(Maint) && is.null(Actions)) {
		 ## this is the old call to the unregistered C++ code in the stosim library			
			fun_out<-.Call("demwMaint", StackedModel_oplsz, StackedMaint, SimulationYearsPerPage, PACKAGE="dem")		
}					
if(!is.null(Maint) && !is.null(Actions)) {
		 ## ******************** NOTE: CHANGES TO ORDER OF ARGUEMENTS ******************			
			fun_out<-.Call("demwActionswMaint", StackedModel_oplsz, SimulationYearsPerPage, StackedActions, StackedMaint, PACKAGE="dem")		
			actionsMAT<-matrix(unlist(fun_out[[3]]),nrow=nrow(Actions), ncol=ncol(Actions), byrow=TRUE)
			
}					
		if(p == 1)  {			
			summaryDF<- as.data.frame(fun_out[[1]])		
		}else{			
		## create a new sumvector for each column then re-build the dataframe with these sumvectors			
			num_fails <- summaryDF$num_fails + as.data.frame(fun_out[[1]])$num_fails		
			repair_hrs <- summaryDF$repair_hrs + as.data.frame(fun_out[[1]])$repair_hrs		
			rand_size <- summaryDF$rand_size + as.data.frame(fun_out[[1]])$rand_size		
			fwrap <- summaryDF$fwrap + as.data.frame(fun_out[[1]])$fwrap		
			summaryDF <-cbind(summaryDF[,c(1,2)], data.frame(num_fails, repair_hrs, rand_size, fwrap))		
		}			
					
					
		history_out <- as.data.frame(fun_out[[2]])			
			Page<-rep(p,length(history_out[,1]))		
			PageCol<-data.frame(Page)		
			history_out<-cbind(PageCol,history_out)		
					
			historyDF<-rbind(historyDF, history_out)		
					
					
		headspace <- attr(fun_out[[2]], "headspace")
		if(headspace == 0) warning("insufficient headspace, simulation incomplete")
		headspaceVec <- c(headspaceVec, headspace/nrow(history_out))			
					
					
	## return to main loop through pages  				
	}				
		if(exists("actionsMAT")) {
			outlist<-list(actionsMAT, summaryDF, historyDF)	
		}else{		
			outlist<-list(headspace=headspaceVec, summaryDF, historyDF)
		}

			
	outlist				
	}				
