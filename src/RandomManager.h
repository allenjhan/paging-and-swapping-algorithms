#ifndef RANDOMMANAGER_H
#define RANDOMMANAGER_H

class RandomManager: public MemManager {
  private:
  public:
    RandomManager(int seed) :
      MemManager(seed) {
    }

    // remove one MemPage from usedList; for random, removes a random page from list
    virtual Node<MemPage>* removeFromUsedList() {
      int toRemove = rand() % usedList.size();
      Node<MemPage>* before = 0;
      Node<MemPage>* current = usedList.getHead();
      for (int i = 0; i < toRemove; i++) {
        before = current;
        current = current->getNext();
      }

      return usedList.remove(before, current);
    }

    // remove current page from usedList and add to end of usedList; LRU only
    virtual void onlyLRU(Node<MemPage>* currentPageNode) {
    }

};

#endif
