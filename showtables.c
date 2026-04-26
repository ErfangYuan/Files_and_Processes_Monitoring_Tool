#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "showtables.h"

// Helper Function: Save/Write buffer to Txt/Bin output
// for flagged options of composite table only:

void saveOutput(const char *Txt, const char *Bin, bool TxtController, bool BinController, char *buffer, int bufferEnd){
	if (TxtController){
		FILE *tempFt = fopen(Txt, "w");
		if (!tempFt){
			fprintf(stderr, "Cannot open or find the file.\n");
			return;
		}
		fwrite(buffer, 1, bufferEnd, tempFt);
		fclose(tempFt);
	}
	
	if (BinController){
		FILE *tempFb = fopen(Bin, "wb");
		if (!tempFb){
			fprintf(stderr, "Cannot open or find the file.\n");
			return;
		}
		fwrite(buffer, 1, bufferEnd, tempFb);
		fclose(tempFb);
	}
}

// Helper Function: Comparing Info by PID for the sorting interface:
// qsort in C stdlib (works as the wrapper function in the fourth argument):

int compareInfo(const void *a, const void *b){
	const Info *infomationA = (const Info*) a;
	const Info *infomationB = (const Info*) b;
	
	if (infomationA->PID < infomationB->PID){
		return -1;
	}
	if (infomationA->PID > infomationB->PID){
		return 1;
	}
	return 0;
}


// Helper Function: Locate the column to print info:
// It returns the coordinate of the column with spaces appended.

int findCol(int currentCol, int targetCol, int previousElementLength, char *buffer, int *bufferEnd){
    int spaces;
    if (previousElementLength <= targetCol - currentCol - 1){
        spaces = targetCol - currentCol;
    } 
	else {
        spaces = 5;
    }

    for (int i = 0; i < spaces; i++){
        buffer[*bufferEnd] = ' ';   
        (*bufferEnd)++;             
    }

    return currentCol + spaces;    
}


// This function takes the specific PID controller specificProcess and its number (if there is),
// and a per-process table controller processTable, as well as the Info array,
// it will show the per-process table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true:

void showProcessTable(bool specificProcess, bool processTable, int processNumber, arr *array){
	if (!array || !array->size || !array->head || !processTable){
        return;
    }	
	
	int currentCol = 1;
	int previousElementLength = 0;
	char buffer[1048576]; // large buffer with 2^20 positions
	int bufferEnd = 0;
	int index = 0;
	
	printf("\n        PID       FD\n");
	printf("        ============\n");
	
	if (specificProcess){
		for (int i = 0; i < array->size; i++){
			if (array->head[i].PID == processNumber){
				
				// printing index:
				previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				index++;
				
				// printing PID:
				currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%d", processNumber);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				
				// printing FD:
				currentCol = findCol(currentCol, 17, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%d\n", array->head[i].FD);
				bufferEnd += previousElementLength;
				
				// re-initializing for next row:
				currentCol = 1;
				previousElementLength = 0;
			}
		}
	}
	
	else {
		for (int j = 0; j < array->size; j++){
			
			// printing index:
			previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			index++;

			// printing PID:
			currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[j].PID);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;

			// printing FD:
			currentCol = findCol(currentCol, 17, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%d\n", array->head[j].FD);
			bufferEnd += previousElementLength;
			
			// re-initializing for next row:
			currentCol = 1;
			previousElementLength = 0;
		}
	}
	
	fwrite(buffer, 1, bufferEnd, stdout);  // printing on STDOUT
	printf("        ============\n");
}

// This function takes the specific PID controller specificProcess and its number (if there is),
// and a system-wide table controller systemWideTable, as well as the Info array,
// it will show the system-wide table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true:
	
void showSystemWideTable(bool specificProcess, bool systemWideTable, int processNumber, arr *array){
	if (!array || !array->size || !array->head || !systemWideTable){
        return;
    }	
	
	int currentCol = 1;
	int previousElementLength = 0;
	char buffer[1048576]; // large buffer with 2^20 positions
	int bufferEnd = 0;
	int index = 0;
	
	printf("\n        PID       FD    Filename\n");
	printf("        ========================================\n");
	
	if (specificProcess){
		for (int i = 0; i < array->size; i++){
			if (array->head[i].PID == processNumber){
				
				// printing index:
				previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				index++;
				
				// printing PID:
				currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%d", processNumber);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				
				// printing FD:
				currentCol = findCol(currentCol, 17, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[i].FD);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				
				// printing File Name:
				currentCol = findCol(currentCol, 25, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%s\n", array->head[i].fileName);
				bufferEnd += previousElementLength;
				
				// re-initializing for next row:
				currentCol = 1;
				previousElementLength = 0;
			}
		}
	}
	
	else {
		for (int j = 0; j < array->size; j++){
						
			// printing index:
			previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			index++;

			// printing PID:
			currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[j].PID);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;

			// printing FD:
			currentCol = findCol(currentCol, 17, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[j].FD);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			
			// printing File Name:
			currentCol = findCol(currentCol, 25, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%s\n", array->head[j].fileName);
			bufferEnd += previousElementLength;
			
			// re-initializing for next row:
			currentCol = 1;
			previousElementLength = 0;
		}
	}
	
	fwrite(buffer, 1, bufferEnd, stdout);  // printing on STDOUT
	printf("        ========================================\n");
}

// This function takes the specific PID controller specificProcess and its number (if there is),
// and a Vnode table controller VnodeTable, as well as the Info array,
// it will show the Vnode table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true:
	
void showVnodeTable(bool specificProcess, bool VnodeTable, int processNumber, arr *array){
	if (!array || !array->size || !array->head || !VnodeTable){
        return;
    }
	
	int currentCol = 1;
	int previousElementLength = 0;
	char buffer[1048576]; // large buffer with 2^20 positions
	int bufferEnd = 0;
	int index = 0;
	
	printf("\n        FD            Inode\n");

	printf("        ========================================\n");
	
	if (specificProcess){
		for (int i = 0; i < array->size; i++){
			if (array->head[i].PID == processNumber){
				
				// printing index:
				previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				index++;

				// printing FD:
				currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[i].FD);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;

				// printing Inode:
				currentCol = findCol(currentCol, 23, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%lu\n", array->head[i].Inode);
				bufferEnd += previousElementLength;
				
				// re-initializing for next row:
				currentCol = 1;
				previousElementLength = 0;
			}
		}
	}
	
	else {
		for (int j = 0; j < array->size; j++){
			
			// printing index:
			previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			index++;
			
			// printing FD:
			currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[j].FD);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			
			// printing Inode:
			currentCol = findCol(currentCol, 23, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%lu\n", array->head[j].Inode);
			bufferEnd += previousElementLength;
			
			// re-initializing for next row:
			currentCol = 1;
			previousElementLength = 0;
		}
	}
	
	fwrite(buffer, 1, bufferEnd, stdout);  // printing on STDOUT
	printf("        ========================================\n");	
	
}

// This function takes the specific PID controller specificProcess and its number (if there is),
// and a composite table controller compositeTable, two related flagged arguments controller:
// compositeBin and compositeTxt, as well as the Info array,
// it will show the composite table for all processes if the specific PID controller is false,
// show the relative specific process table only if the controller is true,
// if either flagged arguments (for output) controller is true,
// it will also create/save the Txt/Bin version of the composite table in "compositeTable.txt" 
// and "compositeTable.bin" respectively:
	
void showCompositeTable(bool specificProcess, bool compositeTable, bool compositeBin, bool compositeTxt, int processNumber, arr *array){
	if (!compositeTable){
		if (compositeBin || compositeTxt){
			fprintf(stderr, "You cannot output compositeTable.txt or compositeTable.bin without flagging --composite !\n");
		}
		return;
	}
	
	if (!array || !array->size || !array->head){
        return;
    }	
	
	int currentCol = 1;
	int previousElementLength = 0;
	char buffer[1048576]; // large buffer with 2^20 positions
	int bufferEnd = 0;
	int index = 0;
	
	int tempLength = sprintf(buffer + bufferEnd, "\n        PID       FD    Filename       Inode\n        ========================================\n");
	bufferEnd += tempLength;
	
	if (specificProcess){
		for (int i = 0; i < array->size; i++){
			if (array->head[i].PID == processNumber){
				
				// printing index:
				previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				index++;

				// printing PID:
				currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%d", processNumber);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				
				// printing FD:
				currentCol = findCol(currentCol, 17, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[i].FD);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				
				// printing File Name:
				currentCol = findCol(currentCol, 25, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%s", array->head[i].fileName);
				currentCol += previousElementLength;
				bufferEnd += previousElementLength;
				
				// printing Inode:
				currentCol = findCol(currentCol, 40, previousElementLength, buffer, &bufferEnd);
				previousElementLength = sprintf(buffer + bufferEnd, "%lu\n", array->head[i].Inode);
				bufferEnd += previousElementLength;
				
				// re-initializing for next row:
				currentCol = 1;
				previousElementLength = 0;
			}
		}
	}
	
	else {
		for (int j = 0; j < array->size; j++){
			
			// printing index:
			previousElementLength = sprintf(buffer + bufferEnd, "%d", index);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			index++;

			// printing PID:
			currentCol = findCol(currentCol, 9, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[j].PID);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;

			// printing FD:
			currentCol = findCol(currentCol, 17, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%d", array->head[j].FD);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			
			// printing File Name:
			currentCol = findCol(currentCol, 25, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%s", array->head[j].fileName);
			currentCol += previousElementLength;
			bufferEnd += previousElementLength;
			
			// printing Inode:
			currentCol = findCol(currentCol, 40, previousElementLength, buffer, &bufferEnd);
			previousElementLength = sprintf(buffer + bufferEnd, "%lu\n", array->head[j].Inode);
			bufferEnd += previousElementLength;
			
			// re-initializing for next row:
			currentCol = 1;
			previousElementLength = 0;
		}
	}
	
	
	tempLength = sprintf(buffer + bufferEnd, "        ========================================\n");
	bufferEnd += tempLength;
	
	fwrite(buffer, 1, bufferEnd, stdout);  // printing on STDOUT
	saveOutput("compositeTable.txt", "compositeTable.bin", compositeTxt, compositeBin, buffer, bufferEnd);
	
}
	
// This function takes the summary table controller summaryTable, 
// as well as the Info array, it will print the counting infomation
// about how many files in each process with the format:
// PID(number of files), ...,

void showSummaryTable(bool summaryTable, arr *array){
	if (!array || !array->size || !array->head || !summaryTable){
        return;
    }
	
	// sorting the Info array, so that Info with the same PID can form blocks,
	// by using qsort in stdlib,
	// which will be available for counting the number of files:
	qsort(array->head, array->size, sizeof(Info), &compareInfo);
	
	printf("\n         Summary Table\n");
	printf("         =============\n");
	
	int count = 0;
	int currentPID = array->head[0].PID;
	
	for (int i = 0; i < array->size; i++){
		if (array->head[i].PID != currentPID){
			printf("%d(%d), ", currentPID, count); // printing the second last process
			currentPID = array->head[i].PID;
			count = 1;
		}
		else {
			count++;
		}
	}
	
	printf("%d(%d)\n", currentPID, count); // printing the last process
}

// This function takes the threshold table controller thresholdTable, 
// as well as the Info array, it will print the counting infomation
// about how many files in each process iff the number of files (i.e. FD numbers) 
// in that PID > threshold, with the format: PID(number of files), ...,
	
void showThresholdTable(bool thresholdTable, int threshold, arr *array){
	if (!array || !array->size || !array->head || !thresholdTable){
        return;
    }
	
	// sorting the Info array, so that Info with the same PID can form blocks,
	// by using qsort in stdlib,
	// which will be available for counting the number of files:
	qsort(array->head, array->size, sizeof(Info), &compareInfo);
	
	printf("\n## Offending processes -- #FD threshold=%d\n", threshold);

	int count = 0;
	int currentPID = array->head[0].PID;
	
	for (int i = 0; i < array->size; i++){
        if (array->head[i].PID != currentPID){
            // determine whether the number of files in the current process should be printed by comparing with the threshold:
            if (count > threshold){
                printf("%d(%d), ", currentPID, count);
            }

            currentPID = array->head[i].PID;
            count = 1;
        } 
        else {
            count++;
        }
    }

    // handle the last process
    if (count > threshold){
        printf("%d(%d)\n", currentPID, count);
    } 
	else {
		printf("\n");
    }
	
}