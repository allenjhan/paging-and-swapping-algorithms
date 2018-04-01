/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ahan
 *
 * Created on September 29, 2017, 2:06 PM
 */

#include "LinkedList.h"
#include "MemManager.h"
#include "FIFOManager.h"
#include "LRUManager.h"
#include "LFUManager.h"
#include "MFUManager.h"
#include "RandomManager.h"
#include "MemPage.h"
#include "Node.h"
#include "Process.h"

/*
 * 
 */
int main(int argc, char** argv) {
  FIFOManager* fifoTrial[5];
  for (int i = 0; i < 5; i++) {
    printf("\nEnter/exit of processes for FIFO trial %d\n", i);
    fifoTrial[i] = new FIFOManager(i+1);
    fifoTrial[i]->runToCompletion();
  }

  printf("\n");

  LRUManager* lruTrial[5];
  for (int i = 0; i < 5; i++) {
    printf("\nEnter/exit of processes for LRU trial %d\n", i);
    lruTrial[i] = new LRUManager(i+1);
    lruTrial[i]->runToCompletion();
  }

  printf("\n");

  LFUManager* lfuTrial[5];
  for (int i = 0; i < 5; i++) {
    printf("\nEnter/exit of processes for LFU trial %d\n", i);
    lfuTrial[i] = new LFUManager(i+1);
    lfuTrial[i]->runToCompletion();
  }

  printf("\n");

  MFUManager* mfuTrial[5];
  for (int i = 0; i < 5; i++) {
    printf("\nEnter/exit of processes for MFU trial %d\n", i);
    mfuTrial[i] = new MFUManager(i+1);
    mfuTrial[i]->runToCompletion();
  }

  printf("\n");

  RandomManager* randomTrial[5];
  for (int i = 0; i < 5; i++) {
    printf("\nEnter/exit of processes for Random trial %d\n", i);
    randomTrial[i] = new RandomManager(i+1);
    randomTrial[i]->runToCompletion();
  }

  printf("\nGet hit/miss ratio and processes swapped in for five trials\n\n");

  for (int i = 0; i < 5; i++) {
    printf("FIFO | Trial %d | ", i + 1);
    fifoTrial[i]->printStatistics();
  }

  printf("\n");

  for (int i = 0; i < 5; i++) {
    printf("LRU | Trial %d | ", i + 1);
    lruTrial[i]->printStatistics();
  }

  printf("\n");

  for (int i = 0; i < 5; i++) {
    printf("LFU | Trial %d | ", i + 1);
    lfuTrial[i]->printStatistics();
  }

  printf("\n");

  for (int i = 0; i < 5; i++) {
    printf("MFU | Trial %d | ", i + 1);
    mfuTrial[i]->printStatistics();
  }

  printf("\n");

  for (int i = 0; i < 5; i++) {
    printf("Random | Trial %d | ", i + 1);
    randomTrial[i]->printStatistics();
  }

  printf("\n");

  printf("\nPrint 100 references for each paging algorithm.\n");

  printf("\nStarting FIFO. Print 100 page references.\n");
  FIFOManager fifotest(0);
  fifotest.run100MemoryReferences();

  printf("\nStarting LRU. Print 100 page references.\n");
  LRUManager lrutest(0);
  lrutest.run100MemoryReferences();

  printf("\nStarting LFU. Print 100 page references.\n");
  LFUManager lfutest(0);
  lfutest.run100MemoryReferences();

  printf("\nStarting MFU. Print 100 page references.\n");
  MFUManager mfutest(0);
  mfutest.run100MemoryReferences();

  printf("\nStarting Random. Print 100 page references.\n");
  RandomManager randomtest(0);
  randomtest.run100MemoryReferences();

  return 0;
}

