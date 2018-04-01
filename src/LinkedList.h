/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LinkedList.h
 * Author: ahan
 *
 * Created on September 29, 2017, 3:29 PM
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <iostream>
#include <cstdio>
#include "Node.h"

// class for linked lists
template<class E>
class LinkedList {

  private:
    Node<E>* head; // the head of the linked list
    Node<E>* tail; // the tail of the linked list
    int count; // how many items are in linked list

  public:
    LinkedList() {
      head = 0;
      tail = 0;
      count = 0;
    }

    Node<E>* getHead() {
      return head;
    }

    void setHead(Node<E>* element) {
      head = element;
    }

    Node<E>* getTail() {
      return tail;
    }

    void setTail(Node<E>* element) {
      tail = element;
    }

    int size() {
      return count;
    }

    // remove node from linked list, given node that is before and node itself
    Node<E>* remove(Node<E>* before, Node<E>* element) {
      if (before == 0)
        head = element->getNext();
      else {
        before->setNext(element->getNext());
      }

      if (element == tail)
        tail = before;
      element->setNext(0);

      count--;
      return element;
    }

    // remove head of list
    Node<E>* pop() {
      return remove(0, head);
    }

    //insert node, given node before and current node
    void insert(Node<E>* before, Node<E>* element) {
      Node<E>* nextElement;
      if (before != 0) {
        nextElement = before->getNext();
        before->setNext(element);
      } else {
        nextElement = head;
        head = element;
      }

      element->setNext(nextElement);
      if (tail == before) {
        tail = element;
        element->setNext(0);
      }

      count++;
    }

    // insert element to tail of list
    void push(Node<E>* element) {
      insert(tail, element);
    }

    // remove element from list
    Node<E>* removeElement(E* element) {
      Node<E>* before = 0;
      Node<E>* current = head;
      Node<E>* toRemove = 0;
      while (current != 0) {
        if (current->getElement() == element) {
          toRemove = remove(before, current);
          break;
        }
        before = current;
        current = current->getNext();
      }
      return toRemove;
    }
};

#endif /* LINKEDLIST_H */

