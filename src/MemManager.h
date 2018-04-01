/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MemManager.h
 * Author: ahan
 *
 * Created on September 30, 2017, 12:08 PM
 */

#ifndef MEMMANAGER_H
#define MEMMANAGER_H
#include <cstdlib>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include "MemPage.h"
#include "Process.h"
#include "LinkedList.h"
#include "Node.h"

//superclass for all trials
class MemManager {
  protected:
    int currentTime;
    MemPage* memory[100];
    LinkedList<Process> waitingProcessQueue;
    LinkedList<Process> runningProcessQueue;
    LinkedList<Process> finishedProcessQueue;
    LinkedList<MemPage> memPageQueue;
    LinkedList<MemPage> usedList;
    LinkedList<MemPage> justRanList;
    int pageReferences;
    int hits;
    int misses;
  public:
    virtual ~MemManager() {
    }

    // constructor for MemManager superclass; seed parameter is seed for random number generator
    MemManager(int seed) {
      currentTime = 0;
      pageReferences = 0;
      hits = 0;
      misses = 0;

      //allocate space on heap for each page
      for (int i = 0; i < 100; i++) {
        memory[i] = new MemPage(i);
        Node<MemPage>* memPageElement = new Node<MemPage>(memory[i]);
        memPageQueue.push(memPageElement);
      }

      //create processes and their parameters (name, number of pages, arrival time, runtime)
      srand(seed);
      for (int i = 0; i < 150; i++) {
        char firstchar = 'A';
        firstchar = (char) (firstchar + i / 10);
        char secondchar = '0';
        secondchar = (char) (secondchar + i % 10);
        std::string firstpart(1, firstchar);
        std::string secondpart(1, secondchar);
        std::stringstream ss;
        ss << firstpart << secondpart;
        std::string name = ss.str();
#ifdef DEBUG
        printf("Creating name %s\n", name.c_str());
#endif
        int dice = rand() % 4;
        int pageNumber;
        switch (dice) {
          case 0:
            pageNumber = 5;
            break;
          case 1:
            pageNumber = 11;
            break;
          case 2:
            pageNumber = 17;
            break;
          default:
            pageNumber = 31;
            break;
        }
        int arrivalTime = rand() % 60000;
        int totalRuntime = (rand() % 5) * 1000 + 1000;
        Process* process = new Process(name, pageNumber, arrivalTime,
            totalRuntime);
        Node<Process>* processElement = new Node<Process>(process);
        waitingProcessQueue.push(processElement);
      }

#ifdef DEBUG
      Node<Process>* traverse = waitingProcessQueue.getHead();
      for (int i=0; i<150; i++) {
        traverse = traverse->getNext();
        printf("traversal i=%d\n", i);
      }
#endif

#ifdef DEBUG
      std::cout<<"Linked List initialized successfully"<<std::endl;
#endif

      //perform insertion sort on process list (waitingProcessQueue) for arrival time
      for (int i = 1; i < 150; i++) {
        Node<Process>* beforeInsert = waitingProcessQueue.getHead();
        Node<Process>* insert = waitingProcessQueue.getHead()->getNext();
        for (int j = 1; j < i; j++) {
          beforeInsert = insert;
          insert = insert->getNext();
        }
        Node<Process>* beforeCheck = 0;
        Node<Process>* check = waitingProcessQueue.getHead();
        for (int j = 0; j < i; j++) {
          if (insert->getElement()->getArrivalTime()
              < check->getElement()->getArrivalTime()) {
            waitingProcessQueue.remove(beforeInsert, insert);
            waitingProcessQueue.insert(beforeCheck, insert);
            break;
          }
          beforeCheck = check;
          check = check->getNext();
        }

      }
#ifdef DEBUG
      std::cout<<"Process queue sorted successfully"<<std::endl;
#endif

    }

    // start processes that are due to be swapped in the first time, and handle the processes that
    // are currently running for this quanta; printEachRef is flag that controls whether to print
    // each page reference event
    virtual void runOneCycle(bool printEachRef) {
#ifdef DEBUG
      printf("0:%04.1f; Starting MemManager Cycle run\n", currentTime/1000.);
#endif
      while (waitingProcessQueue.getHead() != 0
          && waitingProcessQueue.getHead()->getElement()->getArrivalTime()
          <= currentTime && memPageQueue.size() >= 4) {
        Node<Process>* currentProcessNode = waitingProcessQueue.remove(0,
            waitingProcessQueue.getHead());
        runningProcessQueue.push(currentProcessNode);
        startRunningProcess(currentProcessNode->getElement()); //swap in pages for arrived processes
      }
      runProcesses(printEachRef); //handle running processes in runningProcessQueue
    }

    // remove current page from usedList and add to end of usedList; LRU only
    virtual void onlyLRU(Node<MemPage>* currentPageNode)=0;

    // run all processes in runningProcessQueue; page out if miss and no free pages; recycle
    // pages if process done; printEachRef is flag that controls whether to print each page
    // reference event
    virtual void runProcesses(bool printEachRef) {
      addRanPagesBackToQueue();

      //loop through runningProcessQueue
      Node<Process>* beforeCurrentProcessNode = 0;
      Node<Process>* currentProcessNode = runningProcessQueue.getHead();
      while (currentProcessNode != 0) {
        int pageToAccess = currentProcessNode->getElement()->getPageToAccess();
        LinkedList<MemPage>* pages = currentProcessNode->getElement()->getPages();

        //loop through page list for the current process, until page is found
        Node<MemPage>* currentPageNode = pages->getHead();
        bool gotHit = false;
        while (currentPageNode != 0) {
          if (currentPageNode->getElement()->getProcessPageNumber()
              == pageToAccess) {
            currentProcessNode->getElement()->incHits();
            currentPageNode->getElement()->incHitsSinceLastSwap();
            gotHit = true;
            hits++;

            onlyLRU(currentPageNode);

            break;
          }
          currentPageNode = currentPageNode->getNext();
        }

        // if page was not found, then handle miss: get free page, or if no free pages,
        // kick currently used page
        bool pageKicked = false;
        std::string processNameKicked;
        int pageKickedOut;
        if (gotHit == false) {
          misses++;
          currentProcessNode->getElement()->incMisses();
          Node<MemPage>* toSet;
          if (memPageQueue.size() > 0) {
            toSet = memPageQueue.pop();
            Node<MemPage>* copy = toSet->copy();
            usedList.push(copy);
          } else {
            pageKicked = true;
            toSet = pageOut();
            processNameKicked =
                toSet->getElement()->getProcess()->getProcessName();
            pageKickedOut = toSet->getElement()->getProcessPageNumber();
          }
          toSet->getElement()->zeroHitsSinceLastSwap();
          toSet->getElement()->incHitsSinceLastSwap();
          toSet->getElement()->setProcess(currentProcessNode->getElement());
          toSet->getElement()->setProcessPageNumber(pageToAccess);
          toSet->getElement()->getProcess()->getPages()->push(toSet);
        }

        //print page reference if flag is activated
        if (printEachRef == true) {
          printf("0:%04.1f; Ran %s page %d; ", currentTime / 1000.,
              currentProcessNode->getElement()->getProcessName().c_str(),
              pageToAccess);

          if (gotHit == true)
            printf("Hit\n");
          else if (pageKicked == false) {
            printf("Miss; used free page\n");
          } else {
            printf("Miss; %s Page %d kicked\n", processNameKicked.c_str(),
                pageKickedOut);
          }
        }

        //handle process termination if process is done running
        currentProcessNode->getElement()->incRuntime();
        if (currentProcessNode->getElement()->isDone() == true) {
          Node<Process>* next = currentProcessNode->getNext();
          runningProcessQueue.remove(beforeCurrentProcessNode,
              currentProcessNode);
          finishedProcessQueue.push(currentProcessNode);
          LinkedList<MemPage>* memPages =
              currentProcessNode->getElement()->getPages();
          Node<MemPage>* toRecycle;
          while (memPages->getHead() != 0) {
            toRecycle = memPages->pop();
            toRecycle->getElement()->setProcess(0);
            toRecycle->getElement()->setProcessPageNumber(-1);
            memPageQueue.push(toRecycle);
            usedList.removeElement(toRecycle->getElement());
          }
          printf("0:%04.1f; %s; exit; %d pages; duration %.1f sec\n",
              currentTime / 1000.,
              currentProcessNode->getElement()->getProcessName().c_str(),
              currentProcessNode->getElement()->getPageNumber(),
              currentProcessNode->getElement()->getTotalRuntime() / 1000.);
          printMemoryMap();
          currentProcessNode = next;
        } else {
          beforeCurrentProcessNode = currentProcessNode;
          currentProcessNode = currentProcessNode->getNext();
        }
      }
    }

    //take pages that were removed from usedList on last quanta and add back to usedList
    virtual void addRanPagesBackToQueue() {
      Node<MemPage>* current;
      while (justRanList.getHead() != 0) {
        current = justRanList.pop();

        usedList.push(current);
      }
    }

    // take away a MemPage from a currently running process and return it as output of function; also,
    // remove version of selected page from usedList and add it to the back of justRanList
    virtual Node<MemPage>* pageOut() {
#ifdef DEBUG
      Node<MemPage>* traverse = usedList.getHead();
      int i = 0;
      printf("Start mempage traversal\n");
      while(traverse->getNext() != 0) {
        traverse = traverse->getNext();
        printf("mempage usedList traversal i=%d\n", i);
        i++;
      }
      printf("done traversing\n");
#endif

      // remove one page from usedList, and add to justRanList
      Node<MemPage>* removed = removeFromUsedList();
      justRanList.push(removed);

      // find the page requested and remove it from its currently using process
      LinkedList<MemPage>* processPages =
          removed->getElement()->getProcess()->getPages();
      int processPageNumber = removed->getElement()->getProcessPageNumber();
      Node<MemPage>* toPage = 0;
      Node<MemPage>* currentPageNode = processPages->getHead();
      Node<MemPage>* before = 0;
      while (currentPageNode != 0) {
        if (processPageNumber
            == currentPageNode->getElement()->getProcessPageNumber()) {
          toPage = processPages->remove(before, currentPageNode);
          break;
        }
        before = currentPageNode;
        currentPageNode = currentPageNode->getNext();
      }

      if (toPage == 0)
        printf("Error. Could not find page while trying to page out.\n");

      return toPage;
    }

    // remove one MemPage from usedList; abstract here in superclass, overriden in all subclasses
    virtual Node<MemPage>* removeFromUsedList()=0;

    // allocate pages for the process received as parameter to function
    virtual void startRunningProcess(Process* process) {
#ifdef DEBUG
      printf("0:%04.1f; starting process %s\n", currentTime/1000., process->getProcessName().c_str());
#endif

      // assign 1 free page to the process to be swapped in, and add copy of this
      // page to usedList
      process->setSwappedInOnce(true);
      Node<MemPage>* removedPageNode = memPageQueue.remove(0,
          memPageQueue.getHead());
      removedPageNode->getElement()->setProcess(process);
      removedPageNode->getElement()->setProcessPageNumber(0);
      process->getPages()->push(removedPageNode);

      Node<MemPage>* copy = removedPageNode->copy();
      usedList.push(copy);
      printf("0:%04.1f; %s; enter; %d pages; duration %.1f sec\n",
          currentTime / 1000., process->getProcessName().c_str(),
          process->getPageNumber(), process->getTotalRuntime() / 1000.);
      printMemoryMap();
#ifdef DEBUG
      printf("0:%04.1f; process %s started\n", currentTime/1000., process->getProcessName().c_str());
#endif
    }

    //run paging algorithm for one minute
    virtual void runToCompletion() {
      currentTime = 0;
      while (currentTime < 60100) {
        runOneCycle(false);
        currentTime += 100;
      }
    }

    //run paging algorithm for 100 page references
    virtual void run100MemoryReferences() {
      currentTime = 0;
      while (hits + misses < 100) {
        runOneCycle(true);
        currentTime += 100;
      }
    }

    // print hit/miss ratio and the number of processes swapped in during run
    virtual void printStatistics() {
      double ratio = ((double) hits) / misses;
      int swappedIn = runningProcessQueue.size() + finishedProcessQueue.size();
      printf("hit/miss %f | processes swapped in %d\n", ratio, swappedIn);
    }

    // print memory map
    virtual void printMemoryMap() {
      for (int i = 0; i < 100; i++) {
        if (memory[i]->getProcess() != 0)
          printf("%s", memory[i]->getProcess()->getProcessName().c_str());
        else
          printf(". ");
        if (i % 20 == 19)
          printf("\n");
      }
    }

};

#endif /* MEMMANAGER_H */

