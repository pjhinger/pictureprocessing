#ifndef PICNODE_H
#define PICNODE_H

#include "Picture.hpp"

/* The PicNode class is used to implement the internal picture storage of the
 * picLibrary as a singly-linked list. */

class PicNode {

  private:
    /* content of each PicNode */
    string filename;
    Picture pic;

    /* pointers to previous/next nodes in the list*/
    PicNode* next;

    /* fine-grained locking concurrency element*/
    mutex* picnodemutex;

  public:

  /* class constructor, creates a picture node */
  PicNode(string filename, string path);

  /* PicNode query methods */
  string getfilename();
  Picture getpic();
  PicNode* getnext();
  mutex* getlock();

  /* PicNode update methods */
  void setpic(Picture newpic);
  void setnext(PicNode* next);

};

#endif