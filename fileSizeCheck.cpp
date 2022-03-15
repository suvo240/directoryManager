#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CAPACITY  (0x00989680)

//Threshold is set to 80% of the capacity
uint64_t Curr_Mem_Thresholed = (CAPACITY) * 0.8; 
std::string directorypath = "/media/sf_SHARED_UBUNTU/SuvoFolder";

size_t DirctorySize(const char *pathname);

//Deletes oldest files if isMemoryAboveThreshold is true untill isMemoryAboveThreshold is false
void DirectoryManager(const char *dir, int maxNumberOfFiles){

  uint64_t Curr_Mem_Usage = (uint64_t)DirctorySize(directorypath.c_str());
  std::cout<<"DirectoryManager : Current Directory Size is : "<<(int)Curr_Mem_Usage<<std::endl;


  while(Curr_Mem_Usage > Curr_Mem_Thresholed){
    DIR *dp;
    struct dirent *entry, *oldestFile=NULL;
    struct stat statbuf;
    int numberOfEntries=0;
    time_t t_oldest;

    time(&t_oldest);
    if((dp = opendir(dir)) != NULL) {
       if(chdir(dir)==0){
       while((entry = readdir(dp)) != NULL) {
          lstat(entry->d_name, &statbuf);
          if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
             continue;
             numberOfEntries++;
          if(difftime(statbuf.st_mtime, t_oldest) < 0){
                t_oldest = statbuf.st_mtime;
             oldestFile = entry;
                }
            }
        }
    }

    if(numberOfEntries >= maxNumberOfFiles){

      	std::string oldestFileName(oldestFile->d_name);
        std::cout<<"DirectoryManager : Removing file : "<<oldestFileName<<std::endl;
        //Removing Oldest unnecessary file
        remove(oldestFile->d_name);
    }
    closedir(dp);
    Curr_Mem_Usage=(uint64_t)DirctorySize(directorypath.c_str());
    std::cout<<"DirectoryManager under while loop: Current Directory Size is : "<<(int)Curr_Mem_Usage<<std::endl;
  }

}

//Returns total directory size of directory
size_t DirctorySize(const char *pathname){
  if (pathname == NULL) {
	std::cout<<"Error: pathname is NULL "<<std::endl;
    }

    struct stat stats;

    if (lstat(pathname, &stats) == 0) {
      if (S_ISREG(stats.st_mode)){
        return stats.st_size;
      }
    } else {
	      std::cout<<"Error: lstat"<<std::endl;
    }

    DIR* dir = opendir(pathname);

    if (dir == NULL) {
	    std::cout<<"Error: directory name is NULL"<<std::endl;
      return 0;
    }

    struct dirent *dirEntry;
    size_t totalSize = 0;

    for (dirEntry = readdir(dir); dirEntry != NULL; dirEntry =   readdir(dir)) {
      long pathLength = sizeof(char) * (strlen(pathname) + strlen(dirEntry->d_name) + 2);
      char* name = (char*)malloc(pathLength);
      strcpy(name, pathname);
      strcpy(name + strlen(pathname), "/");
      strcpy(name + strlen(pathname) + 1, dirEntry->d_name);

        int status = lstat(name, &stats);
        if (status == 0) {
          totalSize += stats.st_size;
        } else {
	     std::cout<<"Error: lstat"<<std::endl;
        }
      free(name);
    }

    closedir(dir);

    return totalSize - (8192);

}

//Checks if current memory usage is above the threshold for a particular directory
bool checkMemoryThreshold() {

    uint64_t Curr_Mem_Usage = (uint64_t)DirctorySize(directorypath.c_str());
    std::cout<<"checkMemoryThreshold : Current Directory Size is : "<<(int)Curr_Mem_Usage<<std::endl;
    bool mMemoryUsageAboveThreshold_flag=false;

    if (Curr_Mem_Usage > Curr_Mem_Thresholed) {
      mMemoryUsageAboveThreshold_flag = true;
    } else {
      mMemoryUsageAboveThreshold_flag = false;
    }
    return mMemoryUsageAboveThreshold_flag;
}



int main(){
  bool isMemoryAboveThreshold = checkMemoryThreshold();
  if(isMemoryAboveThreshold == true) {
    std::cout<<"mUsageAboveThreshold_flag true"<<std::endl;
    (void)DirectoryManager(directorypath.c_str(),1);
  } else {
     std::cout<<"mUsageAboveThreshold_flag false"<<std::endl;
  }
}