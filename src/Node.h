/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageNode.h
 * Author: ahan
 *
 * Created on September 29, 2017, 3:12 PM
 */

#ifndef NODE_H
#define NODE_H

//class for linked list node
template<class E>
class Node {

  private:
    E* element;  // the element of node
    Node<E>* next; // the next element

  public:
    Node(E* item) {
      element = item;
      next = 0;
    }

    void setNext(Node<E>* next) {
      this->next = next;
    }

    Node<E>* getNext() const {
      return next;
    }

    void setElement(E* element) {
      this->element = element;
    }

    E* getElement() const {
      return element;
    }

    // return copy of node
    Node<E>* copy() {
      Node<E>* item = new Node<E>(element);
      item->setNext(next);
      return item;
    }
};

#endif /* NODE_H */

