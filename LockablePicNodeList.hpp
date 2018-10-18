#ifndef LOCKABLEPICNODELIST_H
#define LOCKABLEPICNODELIST_H

#include "PicNode.hpp"

class LockablePicNodeList {

  private:

    /* pointers to head and tail of the doubly-linked list */
    PicNode* head;
    PicNode* tail;

    /* maintains count on PicNode elements in list */
    int length;

    /* fine-grained locking concurrency elements */
    mutex* headlock;
    mutex* taillock;

  public:

    /* class constructor, constructs an empty LockablePicNodeList */
    LockablePicNodeList();

/*    int getlength();*/ // ME : NEVER USED

    /* LockablePicNodeList query methods */
    Picture getpicnodepicture(string filename); // ME : ALL ACTIVITIES ON NODE IS DONE THROUGH ACCESSING THE POINTER?
    void setpicnodepicture(string filename, Picture pic); // ME : I DONT THINK I NEED THIS IF I HAVE THE GETTER

    bool search(string filename);
    bool insertordered(string path, string filename);
    bool remove(string filename);
    void printallfilenames();

};

#endif
