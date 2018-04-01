/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: ahan
 *
 * Created on September 30, 2017, 11:08 AM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <cstdlib>
#include <string>
#include "LinkedList.h"
#include "MemPage.h"

// class for process
class Process {

  private:
    std::string processName; // name of process
    LinkedList<MemPage>* pages; //page list of in-memory pages for process
    int pageNumber; // total number of pages process has
    int arrivalTime; // arrival time of process
    int totalRuntime; // total runtime of process
    int runtime; //the amount of time the process has run for
    int previousPageAccessed; //last accessed page; -1 if no previous page
    bool swappedInOnce; // true if process has been swapped in once
    int pageHits; // total number of page hits
    int pageMisses; // total number of page misses

  public:
    Process(std::string name, int pageNo, int arrival, double duration) {
      processName = name;
      pageNumber = pageNo;
      arrivalTime = arrival;
      totalRuntime = duration;
      runtime = 0;
      previousPageAccessed = -1;
      swappedInOnce = false;
      pageHits = 0;
      pageMisses = 0;

      pages = new LinkedList<MemPage>();
    }

    // get the next page to access based on space locality
    int getPageToAccess() {
      int nextPage;
      if (previousPageAccessed == -1) {
        nextPage = 0;
      } else {
        int dice = rand() % 10;
        if (dice < 7) {
          int delta;
          if (previousPageAccessed == 0){
            delta = rand() % 2;
          } else if (previousPageAccessed == pageNumber - 1){
            delta = -1 + rand() % 2;
          } else {
            delta = -1 + rand() % 3;
          }
          nextPage = previousPageAccessed + delta;
        } else {
          int* deltas;
          int length;
          if (previousPageAccessed == 0){
            deltas = new int(2);
            deltas[0] = -1;
            deltas[1] = 0;
            length = 2;
          } else if (previousPageAccessed == pageNumber - 1){
            deltas = new int(2);
            deltas[0] = 0;
            deltas[1] = 1;
            length = 2;
          } else {
            deltas = new int(3);
            deltas[0] = -1;
            deltas[1] = 0;
            deltas[1] = 1;
            length = 3;
          }
          bool retry = false;
          do{
            nextPage = rand() % pageNumber;
            for(int i=0; i<length; i++){
              if(nextPage == deltas[i]+previousPageAccessed){
                retry = true;
                break;
              } else{
                retry = false;
              }
            }
          } while(retry);
        }
      }
      previousPageAccessed = nextPage;
      return nextPage;
    }

    bool getSwappedInOnce() {
      return swappedInOnce;
    }

    void setSwappedInOnce(bool val) {
      swappedInOnce = val;
    }

    int getArrivalTime() {
      return arrivalTime;
    }

    int getPageNumber() {
      return pageNumber;
    }

    int getTotalRuntime(){
      return totalRuntime;
    }

    void incRuntime(){
      runtime += 100;
    }

    // returns if the process is done or not
    bool isDone(){
      if(runtime>=totalRuntime)
        return true;
      else
        return false;
    }

    LinkedList<MemPage>* getPages() {
      return pages;
    }

    void incHits() {
      pageHits++;
    }

    void incMisses() {
      pageMisses++;
    }

    std::string getProcessName() {
      return processName;
    }
};

#endif /* PROCESS_H */

