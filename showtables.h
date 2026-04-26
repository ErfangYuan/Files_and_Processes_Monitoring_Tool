#ifndef SHOWTABLES_H
#define SHOWTABLES_H

#include "common.h"

// Helper Function: Comparing Info by PID for the sorting interface:
// qsort in C stdlib (works as the wrapper function in the fourth argument):

int compareInfo(const void *a, const void *b);


// Helper Function: Locate the column to print info:
// It returns the coordinate of the column with spaces appended.

int findCol(int currentCol, int targetCol, int previousElementLength, char *buffer, int *bufferEnd);


// This function takes the specific PID controller specificProcess and its number (if there is),
// and a per-process table controller processTable, as well as the Info array,
// it will show the per-process table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true:

void showProcessTable(bool specificProcess, bool processTable, int processNumber, arr *array);


// This function takes the specific PID controller specificProcess and its number (if there is),
// and a system-wide table controller systemWideTable, as well as the Info array,
// it will show the system-wide table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true:
	
void showSystemWideTable(bool specificProcess, bool systemWideTable, int processNumber, arr *array);


// This function takes the specific PID controller specificProcess and its number (if there is),
// and a Vnode table controller VnodeTable, as well as the Info array,
// it will show the Vnode table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true:
	
void showVnodeTable(bool specificProcess, bool VnodeTable, int processNumber, arr *array);


// This function takes the specific PID controller specificProcess and its number (if there is),
// and a composite table controller compositeTable, two related flagged arguments controller:
// compositeBin and compositeTxt, as well as the Info array,
// it will show the composite table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true,
// if either flagged arguments (for output) controller is true,
// it will also create/save the Txt/Bin version of the composite table in "compositeTable.txt" 
// and "compositeTable.bin" respectively:
	
void showCompositeTable(bool specificProcess, bool compositeTable, bool compositeBin, bool compositeTxt, int processNumber, arr *array);


// This function takes the summary table controller summaryTable, 
// as well as the Info array, it will print the counting infomation
// about how many files in each process with the format:
// PID(number of files), ...,

void showSummaryTable(bool summaryTable, arr *array);


// This function takes the threshold table controller thresholdTable, 
// as well as the Info array, it will print the counting infomation
// about how many files in each process iff the number of files (i.e. FD numbers) 
// in that PID > threshold, with the format: PID(number of files), ...,
	
void showThresholdTable(bool thresholdTable, int threshold, arr *array);


#endif