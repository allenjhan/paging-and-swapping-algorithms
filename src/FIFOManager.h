/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FIFOManager.h
 * Author: ahan
 *
 * Created on September 30, 2017, 8:59 PM
 */

#ifndef FIFOMANAGER_H
#define FIFOMANAGER_H

#include "MemPage.h"
#include <iostream>
#include <cstdio>

// This class runs the paging algorithm for FIFO.
class FIFOManager : public MemManager{

  private:

  public:
    FIFOManager(int seed):MemManager(seed){
    }

    // remove one MemPage from usedList; for FIFO, is head of list
    virtual Node<MemPage>* removeFromUsedList(){
      return usedList.remove(0, usedList.getHead());
    }

    // remove current page from usedList and add to end of usedList; LRU only
    virtual void onlyLRU(Node<MemPage>* currentPageNode){
    }

};


#endif /* FIFOMANAGER_H */

