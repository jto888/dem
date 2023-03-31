   demtest22<-function(Model,OpLineNames=NULL,SimulationYears = 10,SimulationYearsPerPage = 10, Maint=NULL,				
	   Control=list(headspace_margin=.15), ProgRpt=FALSE) { 				
	 				
	  	HeadspaceMargin <- Control$headspace_margin			
					
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
			StackedMaint<-NULL
			for(column in 1:ncol(Maint))  {	
				StackedMaint<-c(StackedMaint, Maint[,column])
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
			suppressWarnings(RNGkind(kind=thisRNGkind))
			
				
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
			
if(is.null(Maint)) {				
		 ## this is the old call to the unregistered C++ code in the stosim library			
			fun_out<-.Call("Basicdem", StackedModel_oplsz, SimulationYearsPerPage, HeadspaceMargin, PACKAGE="dem")		
}else{
		 ## this is the old call to the unregistered C++ code in the stosim library			
			fun_out<-.Call("demwMaint", StackedModel_oplsz, StackedMaint, SimulationYearsPerPage, HeadspaceMargin, PACKAGE="dem")		
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
			detail_out<-as.data.frame(fun_out[[3]])
		if(!is.null(OpLineNames)) {			
			names(detail_out)<-OpLineNames[,2]
			if(nrow(OpLineNames)!=ncol(detail_out)) {	
				warning("Provided OpLineNames do not match OpLines")
			}
		}	
			history_out<-cbind(PageCol,history_out, detail_out)		
					
			historyDF<-rbind(historyDF, history_out)		
					
					
		headspace <- attr(fun_out[[2]], "headspace")
		if(headspace < 0.01) warning("insufficient headspace, simulation incomplete")
		headspaceVec <- c(headspaceVec, headspace/(nrow(history_out)+headspace))			
					
					
	## return to main loop through pages  				
	}				
					
		outlist<-list(HeadspacePerAllocation=headspaceVec, summaryDF, historyDF)	

			
	outlist				
	}				
