#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "common.h"

// Helper Function: Dynamic Array Insertion:

void arrInsert(arr *array, Info *information){

	// dealing with empty cases
    if(!array || !information){
        return;
	}

    // initialize empty arr
    if(!array->capacity){
        array->capacity = 32; // assigning relatively large capacity = 32 (2^5) to avoid early doubling
        array->size = 0;
        array->head = malloc(sizeof(Info) * array->capacity); // dynamic allocation
		if(!array->head){
			fprintf(stderr, "Memory allocation failed\n"); // Failing allocating initially
			return;
		}
    }

    // doubling full arr
    if(array->size >= array->capacity){
        array->capacity *= 2;
        Info *newArr = realloc(array->head, sizeof(Info) * array->capacity); // attempt doubling
        if(!newArr){
            return; // failed doubling
		}
        array->head = newArr;
    }

    // performing insertion and update the position for next insertion
    array->head[array->size] = *information;
    array->size++;
}


// read data and load to arr from /proc
// see README for library dependencies

arr *loadInfo(){

    DIR *procDirectory;
    struct dirent *entry;

    arr *array = malloc(sizeof(arr));
    array->capacity = 0;
    array->size = 0;
    array->head = NULL;

    procDirectory = opendir("/proc");

	// failed opening /proc:
    if(!procDirectory){
        return array;
	}

    while((entry = readdir(procDirectory))){

		// 1. filtering non-PID directories:
        if(entry->d_name[0] < '0' || entry->d_name[0] > '9'){
            continue;
		}
        pid_t PID = atoi(entry->d_name);
        char fdPath[512];
		
		// on creating fdPath:
        sprintf(fdPath, "/proc/%s/fd", entry->d_name);
        DIR *fdDirectory = opendir(fdPath);

		// 2. filtering unaccessible FD directories:
        if(!fdDirectory){
            continue;
		}
        struct dirent *fdEntry;
        while((fdEntry = readdir(fdDirectory))){

			// 3. filtering current directories and parent directories:
            if(fdEntry->d_name[0] == '.'){
                continue;
			}
            int FD = atoi(fdEntry->d_name);
            char linkPath[1024];
            char fileName[2048];

			// on creating linkPath:
            sprintf(linkPath, "%s/%s", fdPath, fdEntry->d_name);
			
			// on collecting file name
            ssize_t len = readlink(linkPath, fileName, sizeof(fileName) - 1);

			// 4. filtering failures when reading file names:
            if(len == -1){
                continue;
			}

			// add end-of-string delimiter explicitly to avoid UDB:
            fileName[len] = '\0';

            struct stat status;
			
			// 5. filtering unaccessible inodes:
            if(stat(linkPath, &status) == -1){
                continue;
			}

            Info infomation;
            infomation.PID = PID;
            infomation.FD = FD;
            infomation.Inode = status.st_ino;
            strncpy(infomation.fileName, fileName, sizeof(infomation.fileName));

			// on creating all FD information arr:
            arrInsert(array, &infomation);
        }
        closedir(fdDirectory);
    }

    closedir(procDirectory);
    return array;
}

