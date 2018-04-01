/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Page.h
 * Author: ahan
 *
 * Created on September 29, 2017, 3:15 PM
 */

#ifndef MEMPAGE_H
#define MEMPAGE_H

//class that provides abstraction for each page in memory
class Process;
class MemPage {
  private:
    Process* process; //the process page is assigned to
    int processPageNumber; //the ordering of the page for the process
    int memPageNumber; //the ordering of the page in memory
    int hitsSinceLastSwap; //the number of hits since last paged in
  public:
    MemPage(int id) {
      process = 0;
      processPageNumber = -1;
      memPageNumber = id;
      hitsSinceLastSwap = 0;
    }

    Process* getProcess() {
      return process;
    }

    void setProcess(Process* val) {
      process = val;
    }

    int getMemPageNumber() {
      return memPageNumber;
    }

    void setProcessPageNumber(int val) {
      processPageNumber = val;
    }

    int getProcessPageNumber() {
      return processPageNumber;
    }

    int getHitsSinceLastSwap(){
      return hitsSinceLastSwap;
    }

    void incHitsSinceLastSwap(){
      hitsSinceLastSwap++;
    }

    void zeroHitsSinceLastSwap(){
      hitsSinceLastSwap = 0;
    }
};

#endif /* MEMPAGE_H */

