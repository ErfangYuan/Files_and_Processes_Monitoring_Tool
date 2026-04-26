#ifndef COMMON_H
#define COMMON_H

#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

// Importantly, we use a CDT to store composed info of every file.
// This is designed to reduce the number of IOPS for different
// table presentations (i.e. scanning all infomation once only).

typedef struct FD_Table_Info{
	pid_t PID;
	int FD;
	ino_t Inode;
	char fileName[2048];
}Info;


// To connect all infomation of every file, we use an ADT:
//                  *Dynamic Array*
// where it has less time cost for searching and traversing than Linked Lists.

typedef struct Dynamic_Array{
	int capacity; 		// for amortized O(1) insertion by doubling when it's not enough
	int size; 			// for Infomation traversing
	Info *head;  // head of all infomation
}arr;


// Helper Function: Dynamic Array Insertion:

void arrInsert(arr *array, Info *information);


// read data and load to arr from /proc
// see README for library dependencies

arr *loadInfo();

#endif
