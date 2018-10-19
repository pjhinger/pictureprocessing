#ifndef LOCKABLEPICNODELIST_H
#define LOCKABLEPICNODELIST_H

#include "PicNode.hpp"

class LockablePicNodeList {

  private:

    /* pointers to head and tail of the doubly-linked list */
    PicNode* head;
    PicNode* tail;

  public:

  /* class constructor, constructs an empty LockablePicNodeList */
  LockablePicNodeList();

  /* LockablePicNodeList query methods */
  pair<PicNode*, PicNode*> findposition(string filename);
  PicNode* findpicnode(string filename);

  /* LockablePicNodeList update methods */
  void unlockpicnodes(pair<PicNode*,PicNode*> pair);
  bool insertordered(string path, string filename);
  bool remove(string filename);
  void printallfilenames();

};

#endif
