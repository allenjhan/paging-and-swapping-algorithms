#ifndef LFUMANAGER_H
#define LFUMANAGER_H

class LFUManager : public MemManager{
  private:
  public:
    LFUManager(int seed) : MemManager(seed) {
    }

    // remove one MemPage from usedList; for LFU, is the page with smallest number of hits since it was paged in
    virtual Node<MemPage>* removeFromUsedList() {
      Node<MemPage>* before = usedList.getHead();
      Node<MemPage>* current = usedList.getHead()->getNext();
      Node<MemPage>* beforeLeastFrequentlyUsed = 0;
      Node<MemPage>* leastFrequentlyUsed = usedList.getHead();
      while(current!=0){
        if(current->getElement()->getHitsSinceLastSwap()<leastFrequentlyUsed->getElement()->getHitsSinceLastSwap()){
          beforeLeastFrequentlyUsed = before;
          leastFrequentlyUsed = current;
        }
        before = current;
        current = current->getNext();
      }

      return usedList.remove(beforeLeastFrequentlyUsed, leastFrequentlyUsed);
    }

    // remove current page from usedList and add to end of usedList; LRU only
    virtual void onlyLRU(Node<MemPage>* currentPageNode){
    }
};

#endif
