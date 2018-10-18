#ifndef PICNODE_H
#define PICNODE_H

#include <atomic>
#include "Picture.hpp"

/* The PicNode class is used to implement the internal picture storage of the
 * picLibrary as a singly-linked list. */

// ME : REMEMBER TO EDIT MAKEFILE

class PicNode {

  private:
    /* content of each PicNode */
    string filename;
    Picture pic;

    // ME : atomic_bool deleteflag;

    /* pointers to previous/next nodes in the list*/
    PicNode* prev;
    PicNode* next;

    /* fine-grained locking concurrency element*/
    mutex* picnodemutex;

  public:

    /* class constructor, creates a picture node */
    PicNode(string filename, Picture pic);

    /* PicNode query methods */
    string getfilename();
    Picture getpic();
    // ME : atomic_bool getdeleteflag();
    PicNode* getprev();
    PicNode* getnext();
    mutex* getlock();

    /* PicNode update methods */
    void setpic(Picture newpic);
    // ME : void toggledeleteflag();
    void setprev(PicNode* prev);
    void setnext(PicNode* next);

};

#endif
