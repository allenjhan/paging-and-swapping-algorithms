#ifndef MFUMANAGER_H
#define MFUMANAGER_H

class MFUManager: public MemManager {
  private:
  public:
    MFUManager(int seed) :
      MemManager(seed) {
    }

    // remove one MemPage from usedList; for MFU, removes page with the most number of hits since it was paged in
    virtual Node<MemPage>* removeFromUsedList() {
      Node<MemPage>* before = usedList.getHead();
      Node<MemPage>* current = usedList.getHead()->getNext();
      Node<MemPage>* beforeMostFrequentlyUsed = 0;
      Node<MemPage>* mostFrequentlyUsed = usedList.getHead();
      while (current != 0) {
        if (current->getElement()->getHitsSinceLastSwap()
            > mostFrequentlyUsed->getElement()->getHitsSinceLastSwap()) {
          beforeMostFrequentlyUsed = before;
          mostFrequentlyUsed = current;
        }
        before = current;
        current = current->getNext();
      }

      return usedList.remove(beforeMostFrequentlyUsed, mostFrequentlyUsed);
    }

    // remove current page from usedList and add to end of usedList; LRU only
    virtual void onlyLRU(Node<MemPage>* currentPageNode) {
    }

};

#endif
