#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "showtables.h"

int main(int argc, char **argv){
	
	// Case 1: positional arguments (Specific PID)
	
	int positionalArg = 1;
	bool specificProcess = false;  // no specific process chosen by default
	int processNumber = 0;  
	if (argc > positionalArg && argv[positionalArg][0] >= '0' && argv[positionalArg][0] <= '9'){
		processNumber = atoi(argv[positionalArg++]);
		if (processNumber <= 0){
			fprintf(stderr, "Invalid PID!\n");
			return 1;
		}
		specificProcess = true;
	}
	
	
	// Cases 2-9: flagged arguments: 
	// process FD table
	// system-wide FD table
    // Vnode FD table
	// composite FD table
	// summary table
	// threshold table
	// text composite output
	// binary composite output
	
	bool processTable = false;
	bool systemWideTable = false;
	bool VnodeTable = false;
	bool compositeTable = false;
	bool summaryTable = false;
	bool thresholdTable = false;
	bool compositeTxt = false;
	bool compositeBin = false;
	int threshold = 0;
	
	for (int i = positionalArg; i < argc; i++){
		if (!strcmp(argv[i], "--per-process")){
			processTable = true;
		}
		else if (!strcmp(argv[i], "--systemWide")){
			systemWideTable = true;
		}
		else if (!strcmp(argv[i], "--Vnode")){
			VnodeTable = true;
		}
		else if (!strcmp(argv[i], "--composite")){
			compositeTable = true;
		}
		else if (!strcmp(argv[i], "--summary")){
			summaryTable = true;
		}
		else if (!strcmp(argv[i], "--output_TXT")){
			compositeTxt = true;
		}
		else if (!strcmp(argv[i], "--output_binary")){
			compositeBin = true;
		}
		else if (!strncmp(argv[i], "--threshold=", 10)){
			threshold = atoi(argv[i] + 12);
			thresholdTable = true;
		}
		else{
			fprintf(stderr, "Unrecognized Option: %s\n", argv[i]);
			return 1;
		}
	}
	
	if (!compositeTable && !compositeBin && !compositeTxt && !processTable && !summaryTable && !systemWideTable && !thresholdTable && !VnodeTable){
		compositeTable = true;  // default behaviour: show the composite FD table
	}
	
	// creating the dynamic array that stores all FD tables related infomation:
	arr *array = loadInfo();
	
	
	// showing each table by the infomation array above, where the process table, system-wide table, Vnode table, 
	// and composite table are dependent on whether the positional argument is provided,
	// whereas the summary table and threshold table aren't (global behaviour),
	// and the composite text/binary output should depend on whether the composite table is shown
	
	bool specificProcessExists = false;
	if (specificProcess){
		for (int j = 0; j < array->size; j++){
			if (processNumber == array->head[j].PID){
				specificProcessExists = true;
				break;
			}
		}
		if (specificProcessExists){
			printf(">>> Target PID: %d\n\n", processNumber);  // show the specific process if it exists
		}
		else {
			fprintf(stderr, "The PID you inputted is not an existed process!\n");
			return 1;
		}
	}
	
	
	// 1. composite table module:
	showCompositeTable(specificProcess, compositeTable, compositeBin, compositeTxt, processNumber, array);
	
	// 2. process table module:
	showProcessTable(specificProcess, processTable, processNumber, array);
	
	// 3. system-wide table module:
	showSystemWideTable(specificProcess, systemWideTable, processNumber, array);
	
	// 4. Vnode table module:
	showVnodeTable(specificProcess, VnodeTable, processNumber, array);
	
	// 5. summary table module:
	showSummaryTable(summaryTable, array);
	
	// 6. threshold table module:
	showThresholdTable(thresholdTable, threshold, array);
	
	// on releasing the memory of dynamic allocation:
	free(array->head);
	free(array);
	return 0;
	
}