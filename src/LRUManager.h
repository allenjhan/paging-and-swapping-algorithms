#ifndef LRUMANAGER_H
#define LRUMANAGER_H

class LRUManager: public MemManager {

  private:
  public:
    LRUManager(int seed) :
      MemManager(seed) {
    }

    // remove one MemPage from usedList; for LRU, is the head of the list
    virtual Node<MemPage>* removeFromUsedList() {
      return usedList.remove(0, usedList.getHead());
    }

    // remove current page from usedList and add to end of usedList; LRU only
    virtual void onlyLRU(Node<MemPage>* currentPageNode){
      Node<MemPage>* usedListRemoved = usedList.removeElement(currentPageNode->getElement());
      usedList.push(usedListRemoved);
    }

};

#endif
