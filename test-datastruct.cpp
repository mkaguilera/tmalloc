// test-datastruct.cpp
//
// Test functions for datastruct.h and datastructmt.h

/*
  Copyright (c) 2015 VMware, Inc
  All rights reserved. 

  Written by Marcos K. Aguilera

  MIT License

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the ""Software""), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/


#include <stdio.h>
#include <list>
#include <map>
#include <stack>
#include <set>

using namespace std;

#define _NO_TMALLOC
#include "os.h"
#include "datastruct.h"
#include "datastructmt.h"

#include "prng.h"


#define TestSimpleLinkList_NOPSK 10000

void TestSimpleLinkList(){
  SimpleLinkList<int> one;
  list<int> two;
  Prng rnd(1);
  int i;
  int op;
  int val;
  int val1, val2;
  SimpleLinkListItem<int> *oneptr;
  list<int>::iterator twoptr;
  list<int>::reverse_iterator tworptr;
  
  printf("Testing SimpleLinkList\n");
  for (i=0; i < TestSimpleLinkList_NOPSK*1000; ++i){
    op = rnd.next() % 9;
    val = (int) rnd.next();
    switch(op){
      case 0: // test pushTail
        one.pushTail(val);
        two.push_back(val);
        break;
      case 1: // test pushHead
        one.pushHead(val);
        two.push_front(val);
        break;
      case 2: // test popTail
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          val1 = one.popTail();
          val2 = two.back();
          two.pop_back();
          assert(val1==val2);
        }
        break;
      case 3: // test popHead
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          val1 = one.popHead();
          val2 = two.front();
          two.pop_front();
          assert(val1==val2);
        }
        break;
      case 4: // test empty
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        break;
      case 5: // test forward iteration
        for (oneptr = one.getFirst(), twoptr = two.begin();
             oneptr != one.getLast();
             oneptr = one.getNext(oneptr), ++twoptr){
          val1 = one.peek(oneptr);
          val2 = *twoptr;
          assert(val1==val2);
        }
        assert(twoptr == two.end());
        break;
      case 6: // test backward iteration
        for (oneptr = one.rGetFirst(), tworptr = two.rbegin();
             oneptr != one.rGetLast();
             oneptr = one.rGetNext(oneptr), ++tworptr){
          val1 = one.peek(oneptr);
          val2 = *tworptr;
          assert(val1==val2);
        }
        assert(tworptr == two.rend());
        break;
      case 7: // test clear
        one.clear();
        two.clear();
        assert(one.empty());
        break;
      case 8:
        val1 = one.getNitems();
        val2 = (int) two.size();
        assert(val1==val2);
        break;
      default:
        assert(0);
        break;
    }
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}


#define TestLinkList_NOPSK 10000
struct TestLinkListNode {
  int val;
  TestLinkListNode *next, *prev;
};

void TestLinkList(){
  LinkList<TestLinkListNode> one;
  TestLinkListNode *oneptr, *oneptr2;
  list<int> two;
  Prng rnd(1);
  int i, j;
  int op;
  int val;
  int val1, val2;
  list<int>::iterator twoptr;
  list<int>::reverse_iterator tworptr;
  
  printf("Testing LinkList\n");
  for (i=0; i < TestLinkList_NOPSK*1000; ++i){
    op = rnd.next() % 11;
    val = (int) rnd.next();
    switch(op){
      case 0: // test pushTail
        oneptr = new TestLinkListNode();
        oneptr->val = val;
        one.pushTail(oneptr);
        two.push_back(val);
        break;
      case 1: // test pushHead
        oneptr = new TestLinkListNode();
        oneptr->val = val;
        one.pushHead(oneptr);
        two.push_front(val);
        break;
      case 2: // test popTail
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          oneptr = one.popTail();
          val1 = oneptr->val;
          val2 = two.back();
          two.pop_back();
          assert(val1==val2);
        }
        break;
      case 3: // test popHead
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          oneptr = one.popHead();
          val1 = oneptr->val;
          val2 = two.front();
          two.pop_front();
          assert(val1==val2);
        }
        break;
      case 4: // test empty
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        break;
      case 5: // test forward iterator
        for (oneptr = one.getFirst(), twoptr = two.begin();
             oneptr != one.getLast();
             oneptr = one.getNext(oneptr), ++twoptr){
          val1 = oneptr->val;;
          val2 = *twoptr;
          assert(val1==val2);
        }
        assert(twoptr == two.end());
        break;
      case 6: // test backward iterator
        for (oneptr = one.rGetFirst(), tworptr = two.rbegin();
             oneptr != one.rGetLast();
             oneptr = one.rGetNext(oneptr), ++tworptr){
          val1 = oneptr->val;;
          val2 = *tworptr;
          assert(val1==val2);
        }
        assert(tworptr == two.rend());
        break;
      case 7: // test clear
        one.clear(true);
        two.clear();
        assert(one.empty());
        break;
      case 8: // test number of items
        val1 = one.getNitems();
        val2 = (int) two.size();
        assert(val1==val2);
        break;
      case 9: // test addBefore
        val1 = one.getNitems();
        val2 = (int)two.size();
        assert(val1==val2);
        
        // pick random number in 0..val2
        val1 = val2 = (int) rnd.next() % (val2+1);
        
        // advance val1 elements
        for (oneptr = one.getFirst(), twoptr = two.begin(), j=0;
             j < val1;
             oneptr = one.getNext(oneptr), ++twoptr, ++j){
          ;
        }
        oneptr2 = new TestLinkListNode();
        oneptr2->val = val;
        one.addBefore(oneptr2, oneptr);
        two.insert(twoptr, val);
        break;
      case 10: // test addAfter
        val1 = one.getNitems();
        val2 = (int)two.size();
        assert(val1==val2);
        
        if (val1 == 0) break; // cannot add after empty list
        // pick random number in 0..val2-1
        val1 = val2 = (int) rnd.next() % val2;
        
        // advance val1 elements
        for (oneptr = one.getFirst(), twoptr = two.begin(), j=0;
             j < val1;
             oneptr = one.getNext(oneptr), ++twoptr, ++j){
          ;
        }
        oneptr2 = new TestLinkListNode();
        oneptr2->val = val;
        one.addAfter(oneptr2, oneptr);
        ++twoptr;
        two.insert(twoptr, val);
        break;
      default:
        assert(0);
        break;
    }
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}


#define TestSLinkList_NOPSK 10000
struct TestSLinkListNode {
  int val;
  TestSLinkListNode *next, *prev;
};

void TestSLinkList(){
  SLinkList<TestSLinkListNode> one;
  TestSLinkListNode *oneptr;
  list<int> two;
  Prng rnd(1);
  int i;
  int op;
  int val;
  int val1, val2;
  list<int>::iterator twoptr;
  list<int>::reverse_iterator tworptr;
  
  printf("Testing SLinkList\n");
  for (i=0; i < TestSLinkList_NOPSK*1000; ++i){
    op = rnd.next() % 9;
    val = (int) rnd.next();
    switch(op){
      case 0: // test pushTail
        oneptr = new TestSLinkListNode();
        oneptr->val = val;
        one.pushTail(oneptr);
        two.push_back(val);
        break;
      case 1: // test pushHead
        oneptr = new TestSLinkListNode();
        oneptr->val = val;
        one.pushHead(oneptr);
        two.push_front(val);
        break;
      case 2: // test peekTail
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          val1 = one.peekTail()->val;
          val2 = two.back();
          assert(val1==val2);
        }
        break;
      case 3: // test peekHead
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          val1 = one.peekHead()->val;
          val2 = two.front();
          assert(val1==val2);
        }
        break;
      case 4: // test popHead
      case 5:
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          oneptr = one.popHead();
          val1 = oneptr->val;
          val2 = two.front();
          two.pop_front();
          assert(val1==val2);
        }
        break;
      case 6: // test empty
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        break;
      case 7: // test forward iterator
        for (oneptr = one.getFirst(), twoptr = two.begin();
             oneptr != one.getLast();
             oneptr = one.getNext(oneptr), ++twoptr){
          val1 = oneptr->val;;
          val2 = *twoptr;
          assert(val1==val2);
        }
        assert(twoptr == two.end());
        break;
      case 8: // test number of items
        val1 = one.getNitems();
        val2 = (int) two.size();
        assert(val1==val2);
        break;
      default:
        assert(0);
        break;
    }
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

// These are used for both TestSortedLinkList() and TestSortedLinkListBK()
#define TestSortedLinkList_NOPSK 10000
struct TestSortedLinkListNode {
  TestSortedLinkListNode(){}
  TestSortedLinkListNode(int k, int v){ key=k; value=v; }
  
  int key;
  int value;
  TestSortedLinkListNode *snext, *sprev;
  int GetKey(){ return key; }
  int *GetKeyPtr(){ return &key; }
  static int CompareKey(int k1, int k2){
    if (k1<k2) return -1;
    if (k1>k2) return +1;
    return 0;
  }
  static int CompareKey(int *k1, int *k2){
    if (*k1<*k2) return -1;
    if (*k1>*k2) return +1;
    return 0;
  }
};

void TestSortedLinkList(){
  SortedLinkList<int,TestSortedLinkListNode> one;
  TestSortedLinkListNode *oneptr;
  multimap<int,int> two;
  Prng rnd(1);
  int i;
  int op;
  int key;
  int val1, val2;
  multimap<int,int>::iterator twoptr;
  multimap<int,int>::reverse_iterator tworptr;
  
  printf("Testing SortedLinkList\n");
  for (i=0; i < TestSortedLinkList_NOPSK*1000; ++i){
    op = rnd.next() % 7;
    key = (int) rnd.next() % 5;
    switch(op){
      case 0: // test insert
        oneptr = new TestSortedLinkListNode();
        oneptr->key = key;
        oneptr->value = key + 10;
        one.insert(oneptr);
        two.insert(pair<int,int>(key, key+10));
        break;
      case 1: // test lookup
        // test exact lookup
        oneptr = one.lookup(key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        
        // test non-exact lookup
        oneptr = one.lookup(key,0);
        twoptr = two.lower_bound(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(twoptr!=two.end());
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        break;
      case 2: // test remove
        oneptr = one.lookup(key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(twoptr!=two.end());
          one.remove(oneptr);
          two.erase(twoptr);
        }
        break;
      case 3: // test popTail
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          oneptr = one.popTail();
          val1 = oneptr->value;
          twoptr = two.end();
          --twoptr;
          val2 = twoptr->second;
          two.erase(twoptr);
          assert(val1==val2);
        }
        break;
      case 4: // test popHead
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          oneptr = one.popHead();
          val1 = oneptr->value;
          twoptr = two.begin();
          val2 = twoptr->second;
          two.erase(twoptr);
          assert(val1==val2);
        }
        break;
      case 5: // test empty
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        break;
      case 6: // test forward iterator
        for (oneptr = one.getFirst(), twoptr = two.begin();
             oneptr != one.getLast();
             oneptr = one.getNext(oneptr), ++twoptr){
          val1 = oneptr->value;
          val2 = twoptr->second;
          assert(val1==val2);
        }
        assert(twoptr == two.end());
        break;
      default:
        assert(0);
        break;
    }
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

void TestSortedLinkListManual(){
  SortedLinkList<int,TestSortedLinkListNode> one;
  TestSortedLinkListNode *oneptr;
  int i;
  
  printf("Testing SortedLinkList manually\n");
  // insert a bunch of keys
  oneptr = new TestSortedLinkListNode(1,11);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(3,13);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(5,15);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(7,17);
  one.insert(oneptr);
  
  // test exact lookups
  oneptr = one.lookup(0); assert(!oneptr);
  oneptr = one.lookup(1); assert(oneptr->key == 1);
  oneptr = one.lookup(2); assert(!oneptr);
  oneptr = one.lookup(6); assert(!oneptr);
  oneptr = one.lookup(7); assert(oneptr->key == 7);
  oneptr = one.lookup(8); assert(!oneptr);
  
  // test non-exact lookups
  oneptr = one.lookup(0,0); assert(oneptr->key == 1);
  oneptr = one.lookup(1,0); assert(oneptr->key == 1);
  oneptr = one.lookup(2,0); assert(oneptr->key == 3);
  oneptr = one.lookup(6,0); assert(oneptr->key == 7);
  oneptr = one.lookup(7,0); assert(oneptr->key == 7);
  oneptr = one.lookup(8,0); assert(!oneptr);

  // remove key 1
  oneptr = one.lookup(1); assert(oneptr);
  one.remove(oneptr);
  oneptr = one.lookup(1); assert(!oneptr);
  oneptr = one.lookup(1,0); assert(oneptr->key==3);

  // remove key 5
  oneptr = one.lookup(5); assert(oneptr);
  one.remove(oneptr);
  oneptr = one.lookup(5); assert(!oneptr);
  oneptr = one.lookup(5,0); assert(oneptr->key==7);

  // remove key 7
  oneptr = one.lookup(7); assert(oneptr);
  one.remove(oneptr);
  oneptr = one.lookup(7); assert(!oneptr);
  oneptr = one.lookup(7,0); assert(!oneptr);
  
  // test popTail
  assert(!one.empty());
  oneptr = one.popTail(); assert(oneptr->key == 3);
  assert(one.empty());

  oneptr = new TestSortedLinkListNode(1,11);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(3,13);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(5,15);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(7,17);
  one.insert(oneptr);
  
  // test popTail
  assert(!one.empty());
  oneptr = one.popTail(); assert(oneptr->key == 7);
  
  // test popHead
  assert(!one.empty());
  oneptr = one.popHead(); assert(oneptr->key == 1);

  for (oneptr = one.getFirst(), i=0;
       oneptr != one.getLast();
       oneptr = one.getNext(oneptr), ++i){
    switch(i){
      case 0:
        assert(oneptr->key == 3);
        break;
      case 1:
        assert(oneptr->key == 5);
        break;
      default:
        assert(0);
    }
  }
  assert(i==2);
  printf("  ok\n");
}

void TestSortedLinkListBK(){
  SortedLinkListBK<int,TestSortedLinkListNode> one;
  TestSortedLinkListNode *oneptr;
  multimap<int,int> two;
  Prng rnd(1);
  int i;
  int op;
  int key;
  int val1, val2;
  multimap<int,int>::iterator twoptr;
  multimap<int,int>::reverse_iterator tworptr;
  
  printf("Testing SortedLinkListBK\n");
  for (i=0; i < TestSortedLinkList_NOPSK*1000; ++i){
    op = rnd.next() % 7;
    key = (int) rnd.next() % 5;
    switch(op){
      case 0: // test insert
        oneptr = new TestSortedLinkListNode();
        oneptr->key = key;
        oneptr->value = key + 10;
        one.insert(oneptr);
        two.insert(pair<int,int>(key, key+10));
        break;
      case 1: // test lookup
        // test exact lookup
        oneptr = one.lookup(&key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        
        // test non-exact lookup
        oneptr = one.lookup(&key,0);
        twoptr = two.lower_bound(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(twoptr!=two.end());
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        break;
      case 2: // test remove
        oneptr = one.lookup(&key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(twoptr!=two.end());
          one.remove(oneptr);
          two.erase(twoptr);
        }
        break;
      case 3: // test popTail
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          oneptr = one.popTail();
          val1 = oneptr->value;
          twoptr = two.end();
          --twoptr;
          val2 = twoptr->second;
          two.erase(twoptr);
          assert(val1==val2);
        }
        break;
      case 4: // test popHead
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        if (!val1){
          oneptr = one.popHead();
          val1 = oneptr->value;
          twoptr = two.begin();
          val2 = twoptr->second;
          two.erase(twoptr);
          assert(val1==val2);
        }
        break;
      case 5: // test empty
        val1 = one.empty() ? 1 : 0;
        val2 = two.empty() ? 1 : 0;
        assert(val1==val2);
        break;
      case 6: // test forward iterator
        for (oneptr = one.getFirst(), twoptr = two.begin();
             oneptr != one.getLast();
             oneptr = one.getNext(oneptr), ++twoptr){
          val1 = oneptr->value;
          val2 = twoptr->second;
          assert(val1==val2);
        }
        assert(twoptr == two.end());
        break;
      default:
        assert(0);
        break;
    }
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

void TestSortedLinkListBKManual(){
  SortedLinkListBK<int,TestSortedLinkListNode> one;
  TestSortedLinkListNode *oneptr;
  int i;
  
  printf("Testing SortedLinkListBK manually\n");
  // insert a bunch of keys
  oneptr = new TestSortedLinkListNode(1,11);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(3,13);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(5,15);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(7,17);
  one.insert(oneptr);
  
  // test exact lookups
  oneptr = one.lookup((i=0,&i)); assert(!oneptr);
  oneptr = one.lookup((i=1,&i)); assert(oneptr->key == 1);
  oneptr = one.lookup((i=2,&i)); assert(!oneptr);
  oneptr = one.lookup((i=6,&i)); assert(!oneptr);
  oneptr = one.lookup((i=7,&i)); assert(oneptr->key == 7);
  oneptr = one.lookup((i=8,&i)); assert(!oneptr);
  
  // test non-exact lookups
  oneptr = one.lookup((i=0,&i),0); assert(oneptr->key == 1);
  oneptr = one.lookup((i=1,&i),0); assert(oneptr->key == 1);
  oneptr = one.lookup((i=2,&i),0); assert(oneptr->key == 3);
  oneptr = one.lookup((i=6,&i),0); assert(oneptr->key == 7);
  oneptr = one.lookup((i=7,&i),0); assert(oneptr->key == 7);
  oneptr = one.lookup((i=8,&i),0); assert(!oneptr);
  
  // remove key 1
  oneptr = one.lookup((i=1,&i)); assert(oneptr);
  one.remove(oneptr);
  oneptr = one.lookup((i=1,&i)); assert(!oneptr);
  oneptr = one.lookup((i=1,&i),0); assert(oneptr->key==3);
  
  // remove key 5
  oneptr = one.lookup((i=5,&i)); assert(oneptr);
  one.remove(oneptr);
  oneptr = one.lookup((i=5,&i)); assert(!oneptr);
  oneptr = one.lookup((i=5,&i),0); assert(oneptr->key==7);
  
  // remove key 7
  oneptr = one.lookup((i=7,&i)); assert(oneptr);
  one.remove(oneptr);
  oneptr = one.lookup((i=7,&i)); assert(!oneptr);
  oneptr = one.lookup((i=7,&i),0); assert(!oneptr);
  
  // test popTail
  assert(!one.empty());
  oneptr = one.popTail(); assert(oneptr->key == 3);
  assert(one.empty());
  
  oneptr = new TestSortedLinkListNode(1,11);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(3,13);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(5,15);
  one.insert(oneptr);
  oneptr = new TestSortedLinkListNode(7,17);
  one.insert(oneptr);
  
  // test popTail
  assert(!one.empty());
  oneptr = one.popTail(); assert(oneptr->key == 7);
  
  // test popHead
  assert(!one.empty());
  oneptr = one.popHead(); assert(oneptr->key == 1);
  
  for (oneptr = one.getFirst(), i=0;
       oneptr != one.getLast();
       oneptr = one.getNext(oneptr), ++i){
    switch(i){
      case 0:
        assert(oneptr->key == 3);
        break;
      case 1:
        assert(oneptr->key == 5);
        break;
      default:
        assert(0);
    }
  }
  assert(i==2);
  printf("  ok\n");
}

struct SkipListKey {
  SkipListKey(){ key = -9999; }
  SkipListKey(int k){ key = k; }
  int key;
  static int cmp(SkipListKey &left, SkipListKey &right){
    if (left.key<right.key) return -1;
    if (left.key>right.key) return +1;
    return 0;
  }
  static int cmp(SkipListKey *left, SkipListKey *right){
    if (left->key<right->key) return -1;
    if (left->key>right->key) return +1;
    return 0;
  }
};

#define TestSkipList_NOPSK 10000

void TestSkipList(){
  SkipList<SkipListKey,int> one, onecopy;
  SkipListNode<SkipListKey,int> *oneptr;
  multimap<int,int> two;
  
  // Tested here:
  //  copy
  //  copy constructor
  //  assignment
  //  clear
  //  lookup
  //  belongs
  //  lookupInsert
  //  insertOrReplace
  //  lookupRemove
  //  insert
  //  getFirst
  //  getLast
  //  getNext
  // Not tested:
  //  keyInInterval
  //  delRange

  Prng rnd(1);
  int i;
  int op;
  int intkey;
  SkipListKey key;
  int val1, val2;
  int *retvalue;
  int res;
  multimap<int,int>::iterator twoptr;
  multimap<int,int>::reverse_iterator tworptr;
  
  printf("Testing SkipList\n");
  for (i=0; i < TestSkipList_NOPSK*1000; ++i){
    op = rnd.next() % 7;
    intkey = (int) rnd.next() % 5;
    key.key = intkey;
    switch(op){
      case 0: // test insert
        one.insert(key, intkey+10);
        two.insert(pair<int,int>(intkey, intkey+10));
        break;
      case 1: // test lookup and belongs
        res = one.lookup(key, retvalue);
        twoptr = two.find(intkey);
        if (res==0){ // found
          assert(twoptr != two.end());
          assert(*retvalue == twoptr->second);
        } else {
          assert(twoptr == two.end());
        }
        
        res = (int) one.belongs(key);
        if (res) assert(twoptr != two.end());
        else assert(twoptr == two.end());
        
        break;
      case 2: // test clear
        one.clear(0,0);
        two.clear();
        break;
      case 3: // test lookupRemove
        res = one.lookupRemove(key, 0, val1);
        twoptr = two.find(intkey);
        if (res){ // not found
          assert(twoptr == two.end());
        } else { // found
          assert(twoptr->first == intkey);
          assert(twoptr->second == val1);
          two.erase(twoptr);
        }
        break;
      case 4: // test lookupInsert
        res = one.lookupInsert(key, retvalue);
        twoptr = two.find(intkey);
        if (res){ // not found, so inserted it
          assert(twoptr == two.end());
          two.insert(pair<int,int>(intkey, intkey+10));
          *retvalue = intkey+10;
        } else { // found
          assert(twoptr != two.end());
          assert(twoptr->first == intkey);
          assert(*retvalue == twoptr->second);
        }
        break;
      case 5: // test insertOrReplace
        res = one.insertOrReplace(key, intkey+10, 0, 0);
        twoptr = two.find(intkey);
        if (res){ // inserted
          assert(twoptr == two.end());
          two.insert(pair<int,int>(intkey, intkey+10));
        } else { // replaced
          assert(twoptr != two.end());
          assert(twoptr->first == intkey);
          twoptr->second = intkey+10;
        }
        break;
      case 6: // test copy
        onecopy = one;
        one = onecopy;
        onecopy.clear(0,0);
        break;
      default:
        assert(0);
        break;
    }
    // check skiplist against reference
    for (oneptr = one.getFirst(), twoptr = two.begin();
         oneptr != one.getLast();
         oneptr = one.getNext(oneptr), ++twoptr){
      val1 = oneptr->key.key;
      val2 = twoptr->first;
      assert(val1==val2);
      val1 = oneptr->value;
      val2 = twoptr->second;
      assert(val1==val2);
    }
    assert(twoptr == two.end());
  
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

void freeSkipListKey(SkipListKey *key){
  delete key;
}

void TestSkipListBK(){
  SkipListBK<SkipListKey,int> one, onecopy;
  SkipListNodeBK<SkipListKey,int> *oneptr;
  multimap<int,int> two;
  
  // Tested here:
  //  copy
  //  clear
  //  lookup
  //  belongs
  //  lookupInsert
  //  insertOrReplace
  //  lookupRemove
  //  insert
  //  getFirst
  //  getLast
  //  getNext
  // Not tested:
  //  keyInInterval
  //  delRange
  
  Prng rnd(1);
  int i;
  int op;
  int intkey;
  SkipListKey key;
  SkipListKey *keyptr;
  int val1, val2;
  int *retvalue;
  int res;
  multimap<int,int>::iterator twoptr;
  multimap<int,int>::reverse_iterator tworptr;
  
  printf("Testing SkipListBK\n");
  for (i=0; i < TestSkipList_NOPSK*1000; ++i){
    op = rnd.next() % 7;
    intkey = (int) rnd.next() % 5;
    key.key = intkey;
    switch(op){
      case 0: // test insert
        keyptr = new SkipListKey();
        *keyptr = key;
        one.insert(keyptr, intkey+10);
        two.insert(pair<int,int>(intkey, intkey+10));
        break;
      case 1: // test lookup and belongs
        res = one.lookup(&key, retvalue);
        twoptr = two.find(intkey);
        if (res==0){ // found
          assert(twoptr != two.end());
          assert(*retvalue == twoptr->second);
        } else {
          assert(twoptr == two.end());
        }
        
        res = (int) one.belongs(&key);
        if (res) assert(twoptr != two.end());
        else assert(twoptr == two.end());
        
        break;
      case 2: // test clear
        one.clear(freeSkipListKey,0);
        two.clear();
        break;
      case 3: // test lookupRemove
        res = one.lookupRemove(&key, freeSkipListKey, val1);
        twoptr = two.find(intkey);
        if (res){ // not found
          assert(twoptr == two.end());
        } else { // found
          assert(twoptr->first == intkey);
          assert(twoptr->second == val1);
          two.erase(twoptr);
        }
        break;
      case 4: // test lookupInsert
        keyptr = new SkipListKey();
        *keyptr = key;
        res = one.lookupInsert(keyptr, retvalue);
        twoptr = two.find(intkey);
        if (res){ // not found, so inserted it
          assert(twoptr == two.end());
          two.insert(pair<int,int>(intkey, intkey+10));
          *retvalue = intkey+10;
        } else { // found
          delete keyptr;
          assert(twoptr != two.end());
          assert(twoptr->first == intkey);
          assert(*retvalue == twoptr->second);
        }
        break;
      case 5: // test insertOrReplace
        keyptr = new SkipListKey();
        *keyptr = key;
        res = one.insertOrReplace(keyptr, intkey+10, freeSkipListKey, 0);
        twoptr = two.find(intkey);
        if (res){ // inserted
          assert(twoptr == two.end());
          two.insert(pair<int,int>(intkey, intkey+10));
        } else { // replaced
          assert(twoptr != two.end());
          assert(twoptr->first == intkey);
          twoptr->second = intkey+10;
        }
        break;
      case 6: // test copy
        onecopy.copy(one, 0, freeSkipListKey, 0);
        one.clear(freeSkipListKey, 0);
        one.copy(onecopy, 0, freeSkipListKey, 0);
        onecopy.clear(freeSkipListKey,0);
        break;
      default:
        assert(0);
        break;
    }
    // check skiplist against reference
    for (oneptr = one.getFirst(), twoptr = two.begin();
         oneptr != one.getLast();
         oneptr = one.getNext(oneptr), ++twoptr){
      val1 = oneptr->key->key;
      val2 = twoptr->first;
      assert(val1==val2);
      val1 = oneptr->value;
      val2 = twoptr->second;
      assert(val1==val2);
    }
    assert(twoptr == two.end());
    
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

struct SkipListRangeTest {
  int typ;         // type of interval (0-8)
  int k1;          // beginning of range interval
  int k2;          // end of range interval
  int found;       // whether key should be found
  int keyfound;    // what key should be found
  int absolutekey; // whether key to be found is relative to offset or absolute (0=relative, 1=absolute)
};
SkipListRangeTest SkipListcommonRtests[] = {
  {0, 1, 5, 0, 0, 0}, {0, 0, 5, 0, 0, 0}, {0, 1,10, 0, 0, 0}, {0, 0,10, 0, 0, 0}, {0,-1, 5, 1, 0, 0}, {0,-1,11, 1, 0, 0}, {0, 5,11, 1, 10, 0},
  {1, 1, 5, 0, 0, 0}, {1, 0, 5, 0, 0, 0}, {1, 1,10, 1,10, 0}, {1, 0,10, 1,10, 0}, {1,-1, 5, 1, 0, 0}, {1,-1,11, 1, 0, 0}, {1, 5,11, 1, 10, 0},
  {2, 1, 5, 1,10, 0}, {2, 0, 5, 1,10, 0}, {2, 1,10, 1,10, 0}, {2, 0,10, 1,10, 0}, {2,-1, 5, 1, 0, 0}, {2,-1,11, 1, 0, 0}, {2, 5,11, 1, 10, 0},
  {3, 1, 5, 0, 0, 0}, {3, 0, 5, 1, 0, 0}, {3, 1,10, 0, 0, 0}, {3, 0,10, 1, 0, 0}, {3,-1, 5, 1, 0, 0}, {3,-1,11, 1, 0, 0}, {3, 5,11, 1, 10, 0},
  {4, 1, 5, 0, 0, 0}, {4, 0, 5, 1, 0, 0}, {4, 1,10, 1,10, 0}, {4, 0,10, 1, 0, 0}, {4,-1, 5, 1, 0, 0}, {4,-1,11, 1, 0, 0}, {4, 5,11, 1, 10, 0},
  {5, 1, 5, 1,10, 0}, {5, 0, 5, 1, 0, 0}, {5, 1,10, 1,10, 0}, {5, 0,10, 1, 0, 0}, {5,-1, 5, 1, 0, 0}, {5,-1,11, 1, 0, 0}, {5, 5,11, 1, 10, 0},
  {6, 1, 5, 1, 0, 1}, {6, 0, 5, 1, 0, 1}, {6, 1,10, 1, 0, 1}, {6, 0,10, 1, 0, 1}, {6,-1, 5, 1, 0, 1}, {6,-1,11, 1, 0, 1}, {6, 5,11, 1,  0, 1},
  {7, 1, 5, 1, 0, 1}, {7, 0, 5, 1, 0, 1}, {7, 1,10, 1, 0, 1}, {7, 0,10, 1, 0, 1}, {7,-1, 5, 1, 0, 1}, {7,-1,11, 1, 0, 1}, {7, 5,11, 1,  0, 1},
  {8, 1, 5, 1, 0, 1}, {8, 0, 5, 1, 0, 1}, {8, 1,10, 1, 0, 1}, {8, 0,10, 1, 0, 1}, {8,-1, 5, 1, 0, 1}, {8,-1,11, 1, 0, 1}, {8, 5,11, 1,  0, 1},
};
int SkipListcommonOffsets[] = {0, 10, 500, 600, 980};

struct SkipListSpecificRtest {
  int offset;
  SkipListRangeTest rt[63];
};

SkipListSpecificRtest SkipListspecificRtests[] = {
  {-10, {
    {0, 1, 5, 0, 0, 0}, {0, 0, 5, 0, 0, 0}, {0, 1,10, 0, 0, 0}, {0, 0,10, 0, 0, 0}, {0,-1, 5, 0, 0, 0}, {0,-1,11, 1,10, 0}, {0, 5,11, 1,10, 0},
    {1, 1, 5, 0, 0, 0}, {1, 0, 5, 0, 0, 0}, {1, 1,10, 1,10, 0}, {1, 0,10, 1,10, 0}, {1,-1, 5, 0, 0, 0}, {1,-1,11, 1,10, 0}, {1, 5,11, 1,10, 0},
    {2, 1, 5, 1, 0, 1}, {2, 0, 5, 1, 0, 1}, {2, 1,10, 1, 0, 1}, {2, 0,10, 1, 0, 1}, {2,-1, 5, 1, 0, 1}, {2,-1,11, 1, 0, 1}, {2, 5,11, 1, 0, 1},
    {3, 1, 5, 0, 0, 0}, {3, 0, 5, 0, 0, 0}, {3, 1,10, 0, 0, 0}, {3, 0,10, 0, 0, 0}, {3,-1, 5, 0, 0, 0}, {3,-1,11, 1,10, 0}, {3, 5,11, 1,10, 0},
    {4, 1, 5, 0, 0, 0}, {4, 0, 5, 0, 0, 0}, {4, 1,10, 1,10, 0}, {4, 0,10, 1,10, 0}, {4,-1, 5, 0, 0, 0}, {4,-1,11, 1,10, 0}, {4, 5,11, 1,10, 0},
    {5, 1, 5, 1, 0, 1}, {5, 0, 5, 1, 0, 1}, {5, 1,10, 1, 0, 1}, {5, 0,10, 1, 0, 1}, {5,-1, 5, 1, 0, 1}, {5,-1,11, 1, 0, 1}, {5, 5,11, 1, 0, 1},
    {6, 1, 5, 0, 0, 0}, {6, 0, 5, 0, 0, 0}, {6, 1,10, 0, 0, 0}, {6, 0,10, 0, 0, 0}, {6,-1, 5, 0, 0, 0}, {6,-1,11, 1, 0, 1}, {6, 5,11, 1, 0, 1},
    {7, 1, 5, 0, 0, 0}, {7, 0, 5, 0, 0, 0}, {7, 1,10, 1, 0, 1}, {7, 0,10, 1, 0, 1}, {7,-1, 5, 0, 0, 0}, {7,-1,11, 1, 0, 1}, {7, 5,11, 1, 0, 1},
    {8, 1, 5, 1, 0, 1}, {8, 0, 5, 1, 0, 1}, {8, 1,10, 1, 0, 1}, {8, 0,10, 1, 0, 1}, {8,-1, 5, 1, 0, 1}, {8,-1,11, 1, 0, 1}, {8, 5,11, 1, 0, 1}}
  },{-20, {
    {0, 1, 5, 0, 0, 0}, {0, 0, 5, 0, 0, 0}, {0, 1,10, 0, 0, 0}, {0, 0,10, 0, 0, 0}, {0,-1, 5, 0, 0, 0}, {0,-1,11, 0, 0, 0}, {0, 5,11, 0, 0, 0},
    {1, 1, 5, 0, 0, 0}, {1, 0, 5, 0, 0, 0}, {1, 1,10, 0, 0, 0}, {1, 0,10, 0, 0, 0}, {1,-1, 5, 0, 0, 0}, {1,-1,11, 0, 0, 0}, {1, 5,11, 0, 0, 0},
    {2, 1, 5, 1, 0, 1}, {2, 0, 5, 1, 0, 1}, {2, 1,10, 1, 0, 1}, {2, 0,10, 1, 0, 1}, {2,-1, 5, 1, 0, 1}, {2,-1,11, 1, 0, 1}, {2, 5,11, 1, 0, 1},
    {3, 1, 5, 0, 0, 0}, {3, 0, 5, 0, 0, 0}, {3, 1,10, 0, 0, 0}, {3, 0,10, 0, 0, 0}, {3,-1, 5, 0, 0, 0}, {3,-1,11, 0, 0, 0}, {3, 5,11, 0, 0, 0},
    {4, 1, 5, 0, 0, 0}, {4, 0, 5, 0, 0, 0}, {4, 1,10, 0, 0, 0}, {4, 0,10, 0, 0, 0}, {4,-1, 5, 0, 0, 0}, {4,-1,11, 0, 0, 0}, {4, 5,11, 0, 0, 0},
    {5, 1, 5, 1, 0, 1}, {5, 0, 5, 1, 0, 1}, {5, 1,10, 1, 0, 1}, {5, 0,10, 1, 0, 1}, {5,-1, 5, 1, 0, 1}, {5,-1,11, 1, 0, 1}, {5, 5,11, 1, 0, 1},
    {6, 1, 5, 0, 0, 0}, {6, 0, 5, 0, 0, 0}, {6, 1,10, 0, 0, 0}, {6, 0,10, 0, 0, 0}, {6,-1, 5, 0, 0, 0}, {6,-1,11, 0, 0, 0}, {6, 5,11, 0, 0, 0},
    {7, 1, 5, 0, 0, 0}, {7, 0, 5, 0, 0, 0}, {7, 1,10, 0, 0, 0}, {7, 0,10, 0, 0, 0}, {7,-1, 5, 0, 0, 0}, {7,-1,11, 0, 0, 0}, {7, 5,11, 0, 0, 0},
    {8, 1, 5, 1, 0, 1}, {8, 0, 5, 1, 0, 1}, {8, 1,10, 1, 0, 1}, {8, 0,10, 1, 0, 1}, {8,-1, 5, 1, 0, 1}, {8,-1,11, 1, 0, 1}, {8, 5,11, 1, 0, 1}}
  },{990, {
    {0, 1, 5, 0, 0, 0}, {0, 0, 5, 0, 0, 0}, {0, 1,10, 0, 0, 0}, {0, 0,10, 0, 0, 0}, {0,-1, 5, 1, 0, 0}, {0,-1,11, 1, 0, 0}, {0, 5,11, 0,  0, 0},
    {1, 1, 5, 0, 0, 0}, {1, 0, 5, 0, 0, 0}, {1, 1,10, 0, 0, 0}, {1, 0,10, 0, 0, 0}, {1,-1, 5, 1, 0, 0}, {1,-1,11, 1, 0, 0}, {1, 5,11, 0,  0, 0},
    {2, 1, 5, 0, 0, 0}, {2, 0, 5, 0, 0, 0}, {2, 1,10, 0, 0, 0}, {2, 0,10, 0, 0, 0}, {2,-1, 5, 1, 0, 0}, {2,-1,11, 1, 0, 0}, {2, 5,11, 0,  0, 0},
    {3, 1, 5, 0, 0, 0}, {3, 0, 5, 1, 0, 0}, {3, 1,10, 0, 0, 0}, {3, 0,10, 1, 0, 0}, {3,-1, 5, 1, 0, 0}, {3,-1,11, 1, 0, 0}, {3, 5,11, 0,  0, 0},
    {4, 1, 5, 0, 0, 0}, {4, 0, 5, 1, 0, 0}, {4, 1,10, 0, 0, 0}, {4, 0,10, 1, 0, 0}, {4,-1, 5, 1, 0, 0}, {4,-1,11, 1, 0, 0}, {4, 5,11, 0,  0, 0},
    {5, 1, 5, 0, 0, 0}, {5, 0, 5, 1, 0, 0}, {5, 1,10, 0, 0, 0}, {5, 0,10, 1, 0, 0}, {5,-1, 5, 1, 0, 0}, {5,-1,11, 1, 0, 0}, {5, 5,11, 0,  0, 0},
    {6, 1, 5, 1, 0, 1}, {6, 0, 5, 1, 0, 1}, {6, 1,10, 1, 0, 1}, {6, 0,10, 1, 0, 1}, {6,-1, 5, 1, 0, 1}, {6,-1,11, 1, 0, 1}, {6, 5,11, 1,  0, 1},
    {7, 1, 5, 1, 0, 1}, {7, 0, 5, 1, 0, 1}, {7, 1,10, 1, 0, 1}, {7, 0,10, 1, 0, 1}, {7,-1, 5, 1, 0, 1}, {7,-1,11, 1, 0, 1}, {7, 5,11, 1,  0, 1},
    {8, 1, 5, 1, 0, 1}, {8, 0, 5, 1, 0, 1}, {8, 1,10, 1, 0, 1}, {8, 0,10, 1, 0, 1}, {8,-1, 5, 1, 0, 1}, {8,-1,11, 1, 0, 1}, {8, 5,11, 1,  0, 1}}
  },{1000, {
    {0, 1, 5, 0, 0, 0}, {0, 0, 5, 0, 0, 0}, {0, 1,10, 0, 0, 0}, {0, 0,10, 0, 0, 0}, {0,-1, 5, 0, 0, 0}, {0,-1,11, 0, 0, 0}, {0, 5,11, 0, 0, 0},
    {1, 1, 5, 0, 0, 0}, {1, 0, 5, 0, 0, 0}, {1, 1,10, 0, 0, 0}, {1, 0,10, 0, 0, 0}, {1,-1, 5, 0, 0, 0}, {1,-1,11, 0, 0, 0}, {1, 5,11, 0, 0, 0},
    {2, 1, 5, 0, 0, 0}, {2, 0, 5, 0, 0, 0}, {2, 1,10, 0, 0, 0}, {2, 0,10, 0, 0, 0}, {2,-1, 5, 0, 0, 0}, {2,-1,11, 0, 0, 0}, {2, 5,11, 0, 0, 0},
    {3, 1, 5, 0, 0, 0}, {3, 0, 5, 0, 0, 0}, {3, 1,10, 0, 0, 0}, {3, 0,10, 0, 0, 0}, {3,-1, 5, 0, 0, 0}, {3,-1,11, 0, 0, 0}, {3, 5,11, 0, 0, 0},
    {4, 1, 5, 0, 0, 0}, {4, 0, 5, 0, 0, 0}, {4, 1,10, 0, 0, 0}, {4, 0,10, 0, 0, 0}, {4,-1, 5, 0, 0, 0}, {4,-1,11, 0, 0, 0}, {4, 5,11, 0, 0, 0},
    {5, 1, 5, 0, 0, 0}, {5, 0, 5, 0, 0, 0}, {5, 1,10, 0, 0, 0}, {5, 0,10, 0, 0, 0}, {5,-1, 5, 0, 0, 0}, {5,-1,11, 0, 0, 0}, {5, 5,11, 0, 0, 0},
    {6, 1, 5, 1, 0, 1}, {6, 0, 5, 1, 0, 1}, {6, 1,10, 1, 0, 1}, {6, 0,10, 1, 0, 1}, {6,-1, 5, 1, 0, 1}, {6,-1,11, 1, 0, 1}, {6, 5,11, 1, 0, 1},
    {7, 1, 5, 1, 0, 1}, {7, 0, 5, 1, 0, 1}, {7, 1,10, 1, 0, 1}, {7, 0,10, 1, 0, 1}, {7,-1, 5, 1, 0, 1}, {7,-1,11, 1, 0, 1}, {7, 5,11, 1, 0, 1},
    {8, 1, 5, 1, 0, 1}, {8, 0, 5, 1, 0, 1}, {8, 1,10, 1, 0, 1}, {8, 0,10, 1, 0, 1}, {8,-1, 5, 1, 0, 1}, {8,-1,11, 1, 0, 1}, {8, 5,11, 1, 0, 1}}
  },{1500, {
    {0, 1, 5, 0, 0, 0}, {0, 0, 5, 0, 0, 0}, {0, 1,10, 0, 0, 0}, {0, 0,10, 0, 0, 0}, {0,-1, 5, 0, 0, 0}, {0,-1,11, 0, 0, 0}, {0, 5,11, 0, 0, 0},
    {1, 1, 5, 0, 0, 0}, {1, 0, 5, 0, 0, 0}, {1, 1,10, 0, 0, 0}, {1, 0,10, 0, 0, 0}, {1,-1, 5, 0, 0, 0}, {1,-1,11, 0, 0, 0}, {1, 5,11, 0, 0, 0},
    {2, 1, 5, 0, 0, 0}, {2, 0, 5, 0, 0, 0}, {2, 1,10, 0, 0, 0}, {2, 0,10, 0, 0, 0}, {2,-1, 5, 0, 0, 0}, {2,-1,11, 0, 0, 0}, {2, 5,11, 0, 0, 0},
    {3, 1, 5, 0, 0, 0}, {3, 0, 5, 0, 0, 0}, {3, 1,10, 0, 0, 0}, {3, 0,10, 0, 0, 0}, {3,-1, 5, 0, 0, 0}, {3,-1,11, 0, 0, 0}, {3, 5,11, 0, 0, 0},
    {4, 1, 5, 0, 0, 0}, {4, 0, 5, 0, 0, 0}, {4, 1,10, 0, 0, 0}, {4, 0,10, 0, 0, 0}, {4,-1, 5, 0, 0, 0}, {4,-1,11, 0, 0, 0}, {4, 5,11, 0, 0, 0},
    {5, 1, 5, 0, 0, 0}, {5, 0, 5, 0, 0, 0}, {5, 1,10, 0, 0, 0}, {5, 0,10, 0, 0, 0}, {5,-1, 5, 0, 0, 0}, {5,-1,11, 0, 0, 0}, {5, 5,11, 0, 0, 0},
    {6, 1, 5, 1, 0, 1}, {6, 0, 5, 1, 0, 1}, {6, 1,10, 1, 0, 1}, {6, 0,10, 1, 0, 1}, {6,-1, 5, 1, 0, 1}, {6,-1,11, 1, 0, 1}, {6, 5,11, 1, 0, 1},
    {7, 1, 5, 1, 0, 1}, {7, 0, 5, 1, 0, 1}, {7, 1,10, 1, 0, 1}, {7, 0,10, 1, 0, 1}, {7,-1, 5, 1, 0, 1}, {7,-1,11, 1, 0, 1}, {7, 5,11, 1, 0, 1},
    {8, 1, 5, 1, 0, 1}, {8, 0, 5, 1, 0, 1}, {8, 1,10, 1, 0, 1}, {8, 0,10, 1, 0, 1}, {8,-1, 5, 1, 0, 1}, {8,-1,11, 1, 0, 1}, {8, 5,11, 1, 0, 1}}
  }};
struct SkipListDelRangeTest {
  int k1;   // beginning of interval
  int typ1; // interval type
  int k2;   // end of interval
  int typ2; // interval type
  int beg;  // beginning of removed range
  int end;  // end of removed range
} SkipListdelRangeTest[] = {
  { 0, 0, 10, 0, 1, 9},
  { 0, 1, 10, 0, 0, 9},
  { 0, 2, 10, 0, 0, 9},
  { 0, 0, 10, 1, 1, 10},
  { 0, 1, 10, 1, 0, 10},
  { 0, 2, 10, 1, 0, 10},
  { 0, 0, 10, 2, 1, 99},
  { 0, 1, 10, 2, 0, 99},
  { 0, 2, 10, 2, 0, 99},
  
  {10, 0, 20, 0, 11, 19},
  {10, 1, 20, 0, 10, 19},
  {10, 2, 20, 0,  0, 19},
  {10, 0, 20, 1, 11, 20},
  {10, 1, 20, 1, 10, 20},
  {10, 2, 20, 1,  0, 20},
  {10, 0, 20, 2, 11, 99},
  {10, 1, 20, 2, 10, 99},
  {10, 2, 20, 2,  0, 99},
  
  {-5, 0, 5, 0,  0, 4},
  {-5, 1, 5, 0,  0, 4},
  {-5, 2, 5, 0,  0, 4},
  {-5, 0, 5, 1,  0, 5},
  {-5, 1, 5, 1,  0, 5},
  {-5, 2, 5, 1,  0, 5},
  {-5, 0, 5, 2,  0, 99},
  {-5, 1, 5, 2,  0, 99},
  {-5, 2, 5, 2,  0, 99},
  
  {-15, 0, -5, 0,  1, 0},
  {-15, 1, -5, 0,  1, 0},
  {-15, 2, -5, 0,  1, 0},
  {-15, 0, -5, 1,  1, 0},
  {-15, 1, -5, 1,  1, 0},
  {-15, 2, -5, 1,  1, 0},
  {-15, 0, -5, 2,  0,99},
  {-15, 1, -5, 2,  0,99},
  {-15, 2, -5, 2,  0,99},
  
  {90, 0, 99, 0, 91, 98},
  {90, 1, 99, 0, 90, 98},
  {90, 2, 99, 0,  0, 98},
  {90, 0, 99, 1, 91, 99},
  {90, 1, 99, 1, 90, 99},
  {90, 2, 99, 1,  0, 99},
  {90, 0, 99, 2, 91, 99},
  {90, 1, 99, 2, 90, 99},
  {90, 2, 99, 2,  0, 99},
  
  {95, 0,105, 0, 96,99},
  {95, 1,105, 0, 95,99},
  {95, 2,105, 0,  0,99},
  {95, 0,105, 1, 96,99},
  {95, 1,105, 1, 95,99},
  {95, 2,105, 1,  0,99},
  {95, 0,105, 2, 96,99},
  {95, 1,105, 2, 95,99},
  {95, 2,105, 2,  0,99},
  
  {105, 0,115, 0, 100,99},
  {105, 1,115, 0, 100,99},
  {105, 2,115, 0,   0,99},
  {105, 0,115, 1, 100,99},
  {105, 1,115, 1, 100,99},
  {105, 2,115, 1,   0,99},
  {105, 0,115, 2, 100,99},
  {105, 1,115, 2, 100,99},
  {105, 2,115, 2,   0,99},
};


#define TestSkipListManual_keyInInterval_NOPSK 100
#define TestSkipListManual_delRange_NOPSK 2     

void TestSkipListManual(){
  SkipList<SkipListKey,int> one, onecopy;
  multimap<int,int> two;
  
  // to be tested:
  // keyInInterval
  // delRange
  
  int i, j, count, res;
  SkipListKey key, key1, key2;
  int offset;

  
  SkipListRangeTest rt;
  
  printf("Testing SkipList manually, keyInInterval\n");
  
  for (count = 0; count < TestSkipListManual_keyInInterval_NOPSK * 1000; ++count){
    if (count % 10000 == 0){
      printf(" %d iterations\n", count);
      fflush(stdout);
    }
    one.clear(0,0);
    for (i=0; i < 100; ++i){
      key.key = i*10;
      one.insert(key, i+10);
    }

    // do common tests
    for (i=0; i < sizeof(SkipListcommonOffsets)/sizeof(int); ++i){
      offset = SkipListcommonOffsets[i];
      for (j=0; j < sizeof(SkipListcommonRtests)/sizeof(SkipListRangeTest); ++j){
        key1.key = SkipListcommonRtests[j].k1 + offset;
        key2.key = SkipListcommonRtests[j].k2 + offset;
        key.key = -9999;
        res = one.keyInInterval(key1, key2, SkipListcommonRtests[j].typ, key);
        assert(res==SkipListcommonRtests[j].found);
        if (SkipListcommonRtests[j].found){
          if (SkipListcommonRtests[j].absolutekey) assert(key.key==SkipListcommonRtests[j].keyfound);
          else assert(key.key == SkipListcommonRtests[j].keyfound + offset);
        } else assert(key.key == -9999); // check that key was untouched
      }
    }

    // do specific tests
    for (i=0; i < sizeof(SkipListspecificRtests)/sizeof(SkipListSpecificRtest); ++i){
      offset = SkipListspecificRtests[i].offset;
      for (j=0; j < 63; ++j){
        rt = SkipListspecificRtests[i].rt[j];
        key1.key = rt.k1 + offset;
        key2.key = rt.k2 + offset;
        key.key = -9999;
        res = one.keyInInterval(key1, key2, rt.typ, key);
        assert(res==rt.found);
        if (rt.found){
          if (rt.absolutekey) assert(key.key==rt.keyfound);
          else assert(key.key == rt.keyfound + offset);
        } else assert(key.key == -9999); // check that key was untouched
      }
    }
  }
  
  struct SkipListDelRangeTest drt;
  int *valptr;
  
  printf("Testing SkipList manually, delRange\n");
  
  for (count = 0; count < TestSkipListManual_delRange_NOPSK * 1000; ++count){
    if (count % 100 == 0){
      printf(" %d iterations\n", count);
      fflush(stdout);
    }
    for (i=0; i < sizeof(SkipListdelRangeTest)/sizeof(SkipListDelRangeTest); ++i){
      drt = SkipListdelRangeTest[i];
      one.clear(0,0);
      for (j=0; j < 100; ++j){
        key.key = j;
        one.insert(key, j+10);
      }
      key1.key = drt.k1;
      key2.key = drt.k2;
      res = one.delRange(key1, drt.typ1, key2, drt.typ2, 0, 0);
      assert(res == drt.end-drt.beg+1);
      for (j=0; j < 100; ++j){
        key.key = j;
        res = one.lookup(key, valptr);
        if (drt.beg <= j && j <= drt.end) assert(res != 0); // should not find since they range is deleted
        else {
          assert(res==0);
          assert(*valptr == j+10);
        }
      }
    }
  }
}

void TestSkipListBKManual(){
  SkipListBK<SkipListKey,int> one, onecopy;
  multimap<int,int> two;
  
  // to be tested:
  // keyInInterval
  // delRange
  
  int i, j, count, res;
  SkipListKey key1, key2;
  SkipListKey *keyptr;
  int offset;
  
  SkipListRangeTest rt;
  
  
  printf("Testing SkipListBK manually, keyInInterval\n");
  
  for (count = 0; count < TestSkipListManual_keyInInterval_NOPSK * 1000; ++count){
    if (count % 10000 == 0){
      printf(" %d iterations\n", count);
      fflush(stdout);
    }
    one.clear(freeSkipListKey,0);
    for (i=0; i < 100; ++i){
      keyptr = new SkipListKey();
      keyptr->key = i*10;
      one.insert(keyptr, i+10);
    }
    
    // do common tests
    for (i=0; i < sizeof(SkipListcommonOffsets)/sizeof(int); ++i){
      offset = SkipListcommonOffsets[i];
      for (j=0; j < sizeof(SkipListcommonRtests)/sizeof(SkipListRangeTest); ++j){
        key1.key = SkipListcommonRtests[j].k1 + offset;
        key2.key = SkipListcommonRtests[j].k2 + offset;
        keyptr = one.keyInInterval(&key1, &key2, SkipListcommonRtests[j].typ);
        if (keyptr) assert(SkipListcommonRtests[j].found);
        else assert(!SkipListcommonRtests[j].found);
        if (SkipListcommonRtests[j].found){
          if (SkipListcommonRtests[j].absolutekey) assert(keyptr->key==SkipListcommonRtests[j].keyfound);
          else assert(keyptr->key == SkipListcommonRtests[j].keyfound + offset);
        }
      }
    }
    
    // do specific tests
    for (i=0; i < sizeof(SkipListspecificRtests)/sizeof(SkipListSpecificRtest); ++i){
      offset = SkipListspecificRtests[i].offset;
      for (j=0; j < 63; ++j){
        rt = SkipListspecificRtests[i].rt[j];
        key1.key = rt.k1 + offset;
        key2.key = rt.k2 + offset;
        keyptr = one.keyInInterval(&key1, &key2, rt.typ);
        if (keyptr) assert(rt.found);
        else assert(!rt.found);
        if (rt.found){
          if (rt.absolutekey) assert(keyptr->key==rt.keyfound);
          else assert(keyptr->key == rt.keyfound + offset);
        }
      }
    }
  }
  
  struct SkipListDelRangeTest drt;
  int *valptr;
  
  printf("Testing SkipListBK manually, delRange\n");
  
  for (count = 0; count < TestSkipListManual_delRange_NOPSK * 1000; ++count){
    if (count % 100 == 0){
      printf(" %d iterations\n", count);
      fflush(stdout);
    }
    for (i=0; i < sizeof(SkipListdelRangeTest)/sizeof(SkipListDelRangeTest); ++i){
      drt = SkipListdelRangeTest[i];
      one.clear(freeSkipListKey,0);
      for (j=0; j < 100; ++j){
        keyptr = new SkipListKey();
        keyptr->key = j;
        one.insert(keyptr, j+10);
      }
      key1.key = drt.k1;
      key2.key = drt.k2;
      res = one.delRange(&key1, drt.typ1, &key2, drt.typ2, freeSkipListKey, 0);
      assert(res == drt.end-drt.beg+1);
      for (j=0; j < 100; ++j){
        key1.key = j;
        res = one.lookup(&key1, valptr);
        if (drt.beg <= j && j <= drt.end) assert(res != 0); // should not find since they range is deleted
        else {
          assert(res==0);
          assert(*valptr == j+10);
        }
      }
    }
  }
  one.clear(freeSkipListKey,0);
}

struct HashTableNode {
  int key;
  int value;
  HashTableNode *next, *prev, *snext, *sprev;
  int GetKey(){ return key; }
  int *GetKeyPtr(){ return &key; }
  static int HashKey(int k){ return k; }
  static int HashKey(int *k){ return *k; }
  static int CompareKey(int left, int right){
    if (left < right) return -1;
    if (left > right) return +1;
    return 0;
  }
  static int CompareKey(int *left, int *right){
    if (*left < *right) return -1;
    if (*left > *right) return +1;
    return 0;
  }
};

#define TestHashTable_NOPSK 100

void TestHashTable(){
  HashTable<int,HashTableNode> one(5);
  HashTableNode *oneptr;
  multimap<int,int> two;
  Prng rnd(1);
  int i;
  int op;
  int key;
  multimap<int,int>::iterator twoptr;
  multimap<int,int>::reverse_iterator tworptr;
  
  printf("Testing HashTable\n");
  for (i=0; i < TestHashTable_NOPSK*1000; ++i){
    op = rnd.next() % 5;
    key = (int) rnd.next() % 15;
    switch(op){
      case 0: // test insert
        oneptr = new HashTableNode();
        oneptr->key = key;
        oneptr->value = key + 10;
        one.insert(oneptr);
        two.insert(pair<int,int>(key, key+10));
        break;
      case 1: // test lookup
        oneptr = one.lookup(key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        break;
      case 2: // test remove
        oneptr = one.lookup(key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(twoptr!=two.end());
          one.remove(oneptr);
          two.erase(twoptr);
        }
        break;
      case 3: // test operator []
        oneptr = one[key];
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        break;
      case 4: // test getNitems
        assert(one.getNitems() == two.size());
        break;
      default:
        assert(0);
        break;
    }
  
    // test forward iterator and see that lists as the same
    for (oneptr = one.getFirst();
         oneptr != one.getLast();
         oneptr = one.getNext(oneptr)){
      twoptr = two.find(oneptr->key);
      assert(twoptr != two.end());
      assert(oneptr->key == twoptr->first && oneptr->value == twoptr->second);
    }
    
    for (twoptr = two.begin();
         twoptr != two.end();
         ++twoptr){
      oneptr = one.lookup(twoptr->first);
      assert(oneptr);
      assert(oneptr->key == twoptr->first && oneptr->value == twoptr->second);
    }
    if (i % 10000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

void TestHashTableBK(){
  HashTableBK<int,HashTableNode> one(5);
  HashTableNode *oneptr;
  multimap<int,int> two;
  Prng rnd(1);
  int i;
  int op;
  int key;
  multimap<int,int>::iterator twoptr;
  multimap<int,int>::reverse_iterator tworptr;
  
  printf("Testing HashTableBK\n");
  for (i=0; i < TestHashTable_NOPSK*1000; ++i){
    op = rnd.next() % 5;
    key = (int) rnd.next() % 15;
    switch(op){
      case 0: // test insert
        oneptr = new HashTableNode();
        oneptr->key = key;
        oneptr->value = key + 10;
        one.insert(oneptr);
        two.insert(pair<int,int>(key, key+10));
        break;
      case 1: // test lookup
        oneptr = one.lookup(&key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        break;
      case 2: // test remove
        oneptr = one.lookup(&key);
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(twoptr!=two.end());
          one.remove(oneptr);
          two.erase(twoptr);
        }
        break;
      case 3: // test operator []
        oneptr = one[&key];
        twoptr = two.find(key);
        if (!oneptr) assert(twoptr==two.end());
        else {
          assert(oneptr->key == twoptr->first);
          assert(oneptr->value == twoptr->second);
        }
        break;
      case 4: // test getNitems
        assert(one.getNitems() == two.size());
        break;
      default:
        assert(0);
        break;
    }
    
    // test forward iterator and see that lists as the same
    for (oneptr = one.getFirst();
         oneptr != one.getLast();
         oneptr = one.getNext(oneptr)){
      twoptr = two.find(oneptr->key);
      assert(twoptr != two.end());
      assert(oneptr->key == twoptr->first && oneptr->value == twoptr->second);
    }
    
    for (twoptr = two.begin();
         twoptr != two.end();
         ++twoptr){
      int key = twoptr->first;
      oneptr = one.lookup(&key);
      assert(oneptr);
      assert(oneptr->key == twoptr->first && oneptr->value == twoptr->second);
    }
    if (i % 10000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

#define TestStackArray_NOPSK 1000

void TestStackArray(){
  StackArray<int> one(1,2);
  stack<int> two;
  Prng rnd(1);
  int i;
  int op;
  int key, key2;
  
  printf("Testing StackArray\n");
  for (i=0; i < TestStackArray_NOPSK*1000; ++i){
    op = rnd.next() % 2;
    key = (int) rnd.next() % 15;
    switch(op){
      case 0: // test push
        one.push(key);
        two.push(key);
        break;
      case 1: // test pop
        if (one.empty()){
          assert(two.empty());
          break;
        }
        key = one.pop();
        key2 = two.top();
        two.pop();
        assert(key==key2);
        break;
      default:
        assert(0);
        break;
    }
    assert(one.getNitems() == two.size());
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

#define TestSet_NOPSK 1000

void TestSet(){
  Set<Int> one;
  set<int> two;
  set<int>::iterator twoptr;
  Prng rnd(1);
  int i;
  int op;
  int key;
  int res;
  
  printf("Testing Set\n");
  for (i=0; i < TestSet_NOPSK*1000; ++i){
    op = rnd.next() % 3;
    key = (unsigned) rnd.next() % 5;
    switch(op){
      case 0: // test insert
        res = one.insert(key);
        if (res==0){ // item did not exist
          assert(two.find(key) == two.end());
          two.insert(key);
        } else assert(two.find(key) != two.end());
        break;
      case 1: // test remove
        res = one.remove(key);
        if (res){ // item did not exist
          assert(two.find(key) == two.end());
        } else {
          twoptr = two.find(key);
          assert(twoptr != two.end());
          two.erase(twoptr);
        }
        break;
      case 2: // test belongs
        res = one.belongs(key);
        twoptr = two.find(key);
        if (res==0) assert(twoptr==two.end());
        else assert(twoptr!=two.end());
        break;
      default:
        assert(0);
        break;
    }
    assert(one.getNitems() == two.size());
    if (i % 100000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

int TestPtrDeleted; // to track whether destructor of TestPtrStruct has been called

struct TestPtrStruct {
  friend class Ptr<TestPtrStruct>;
  Align4 int refcount;
  int i;
  TestPtrStruct(int val){ refcount=0; i = val;}
  ~TestPtrStruct(){
    TestPtrDeleted = 1;
  }
};

void TestPtr(){
  TestPtrDeleted = 0;
  Ptr<TestPtrStruct> ptr1, ptr2, ptr3;
  TestPtrStruct *rawptr;
  printf("Testing Ptr\n");
  rawptr = new TestPtrStruct(55);
  ptr1 = rawptr; assert(!TestPtrDeleted); // 1
  ptr2 = ptr1; assert(!TestPtrDeleted);  // 12
  ptr3 = ptr2; assert(!TestPtrDeleted);  // 123
  assert(ptr1->i==55);
  ptr1 = 0; assert(!TestPtrDeleted);  // 23
  ptr2 = 0; assert(!TestPtrDeleted);  // 3
  ptr1 = ptr3; assert(!TestPtrDeleted); // 13
  ptr1 = ptr2; assert(!TestPtrDeleted); // 3
  ptr1 = ptr3; assert(!TestPtrDeleted); // 13
  ptr3 = 0; assert(!TestPtrDeleted);  // 1
  assert(ptr1->i==55);
  ptr1 = 0; assert(TestPtrDeleted);  // nobody
}

#define TestHashTableMT_NOPSK 10000

struct HashTableMTKey {
  int key;
  static unsigned hash(const HashTableMTKey &l){ return (unsigned) l.key; }
  static int cmp(const HashTableMTKey &l, const HashTableMTKey &r){
    if (l.key < r.key) return -1;
    if (l.key > r.key) return +1;
    return 0;
  }
};

int TestHashTableKey;
int TestHashTableMTApplyFunc(HashTableMTKey &key, int *value, int status, SkipList<HashTableMTKey,int> *bucket, u64 parm){
  assert(key.key==TestHashTableKey);
  if (!status) return *value + (int) parm;
  else return -1 + (int) parm;
}

void TestHashTableMT(){
  HashTableMT<HashTableMTKey,int> one(10);
  HashTableMTKey key;
  multimap<int,int> two;
  
  // Tested here:
  //  clear
  //  insert
  //  lookup
  //  lookupApply
  //  lookupInsert
  //  remove
  //  lookupRemove
  
  Prng rnd(1);
  int i;
  int op;
  int intkey;
  int val1;
  int *retvalue;
  int res;
  multimap<int,int>::iterator twoptr;
  
  printf("Testing HashTableMT\n");
  for (i=0; i < TestHashTableMT_NOPSK*1000; ++i){
    op = rnd.next() % 7;
    intkey = (int) rnd.next() % 5;
    key.key = intkey;
    switch(op){
      case 0: // test clear
        one.clear(0,0);
        two.clear();
        break;
      case 1: // test insert
        one.insert(key, intkey+10);
        two.insert(pair<int,int>(intkey, intkey+10));
        break;
      case 2: // test lookup
        res = one.lookup(key, val1);
        twoptr = two.find(intkey);
        if (!res){ // found
          assert(twoptr != two.end());
          assert(key.key == twoptr->first && val1 == twoptr->second);
        } else { // not found
          assert(twoptr == two.end());
        }
        break;
      case 3: // test lookupApply
        TestHashTableKey = intkey;
        
        res = one.lookupApply(key, TestHashTableMTApplyFunc, 10);
        twoptr = two.find(intkey);
        res -= 10;
        if (res != -1){ // found
          assert(twoptr != two.end());
          assert(res == twoptr->second);
        } else { // not found
          assert(twoptr == two.end());
        }
        break;
      case 4: // test lookupInsert
        res = one.lookupInsert(key, retvalue, 0);
        twoptr = two.find(intkey);
        if (!res){ // found
          assert(twoptr != two.end());
          assert(key.key == twoptr->first && *retvalue == twoptr->second);
        } else { // not found
          assert(twoptr == two.end());
          two.insert(pair<int,int>(intkey,intkey+10));
          *retvalue = intkey+10;
        }
        break;
      case 5: // test remove
        res = one.remove(key, 0);
        twoptr = two.find(intkey);
        if (res){ // not found
          assert(twoptr == two.end());
        } else { // found
          assert(twoptr->first == intkey);
          two.erase(twoptr);
        }
        break;
      case 6: // test lookupRemove
        res = one.lookupRemove(key, 0, val1);
        twoptr = two.find(intkey);
        if (res){ // not found
          assert(twoptr == two.end());
        } else { // found
          assert(twoptr->first == intkey);
          assert(twoptr->second == val1);
          two.erase(twoptr);
        }
        break;
      default:
        assert(0);
        break;
    }
    if (i % 1000000 == 0){
      printf(" %d iterations (op %d)\n", i, op);
      fflush(stdout);
    }
  }
}

void TestBoundedQueueShort(){
  BoundedQueue<int> queue(10);
  int i;
  int j;
  printf("Testing BoundedQueue\n");
  for (i=0; i < 10; ++i)
    queue.enqueue(i);
  for (i=0; i < 10; ++i)
    assert(queue.dequeue() == i);
  for (i=0; i < 5; ++i)
    queue.enqueue(i);
  for (j=0; j < 3; ++j)
    assert(queue.dequeue() == j);
  for (; i < 10; ++i)
    queue.enqueue(i);
  for (; j < 10; ++j)
    assert(queue.dequeue() == j);
}

#define TestBoundedQueue_NOPSK 1000

BoundedQueue<int> TestBoundedQueueQueue(50);

OSTHREAD_FUNC TestBoundedQueueProducer(void *arg){
  int i;
  for (i=0; i < TestBoundedQueue_NOPSK*1000; ++i){
    TestBoundedQueueQueue.enqueue(i);
  }
  TestBoundedQueueQueue.enqueue(-1); // signal completion
  return 0;
}

OSTHREAD_FUNC TestBoundedQueueConsumer(void *arg){
  int i=0, res;
  while (1){
    res = TestBoundedQueueQueue.dequeue();
    if (res == -1) break;
    assert(res ==i);
    ++i;
    if (i % 100000 == 0){
      printf(" %d iterations\n", i);
      fflush(stdout);
    }
  }
  assert(i==TestBoundedQueue_NOPSK*1000);
  return 0;
}

void TestBoundedQueueConcurrent(){
  OSThread_t thr1, thr2;
  void *res1, *res2;
  int res;
  printf("Testing BoundedQueue (concurrent)\n");
  res = OSCreateThread(&thr1, TestBoundedQueueProducer, 0); assert(res==0);
  res = OSCreateThread(&thr2, TestBoundedQueueConsumer, 0); assert(res==0);
  res = OSWaitThread(thr1, &res1); assert(res==0);
  res = OSWaitThread(thr2, &res2); assert(res==0);
}


#define TestChannel_NOPSK 50000

Channel<int,64> TestChannelChannel;

OSTHREAD_FUNC TestChannelProducer(void *arg){
  int i, ret;
  for (i=0; i < TestChannel_NOPSK*1000; ++i){
    do {
      ret = TestChannelChannel.enqueue(i);
    } while (ret);
  }
  i = -1;
  TestChannelChannel.enqueue(i); // signal completion
  return 0;
}

OSTHREAD_FUNC TestChannelConsumer(void *arg){
  int i=0, res, ret;
  while (1){
    ret = TestChannelChannel.dequeue(res);
    if (ret) continue;
    if (res == -1) break;
    assert(res ==i);
    ++i;
    if (i % 1000000 == 0){
      printf(" %d iterations\n", i);
      fflush(stdout);
    }
  }
  assert(i==TestChannel_NOPSK*1000);
  return 0;
}

void TestChannelConcurrent(){
  OSThread_t thr1, thr2;
  void *res1, *res2;
  int res;
  printf("Testing Channel (concurrent)\n");
  res = OSCreateThread(&thr1, TestChannelProducer, 0); assert(res==0);
  res = OSCreateThread(&thr2, TestChannelConsumer, 0); assert(res==0);
  res = OSWaitThread(thr1, &res1); assert(res==0);
  res = OSWaitThread(thr2, &res2); assert(res==0);
}



int main(int argc, char **argv) {
  TestSimpleLinkList();
  TestLinkList();
  TestSLinkList();
  TestSortedLinkList();
  TestSortedLinkListManual();
  TestSortedLinkListBK();
  TestSortedLinkListBKManual();
  TestSkipList();
  TestSkipListManual();
  TestSkipListBK();
  TestSkipListBKManual();
  TestHashTable();
  TestHashTableBK();
  TestStackArray();
  TestSet();
  TestPtr();
  TestHashTableMT();
  TestBoundedQueueShort();
  TestBoundedQueueConcurrent();
  TestChannelConcurrent();
  return 0;
}
