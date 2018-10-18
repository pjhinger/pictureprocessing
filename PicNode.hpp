//
// Created by pj2017 on 18/10/18.
//

#ifndef PICNODE_H
#define PICNODE_H

#include <atomic>
#include "Picture.hpp"

/* The PicNode class is used to implement the internal picture storage of the
 * picLibrary as a singly-linked list. */

class PicNode {

  private:
    const string filename;
    Picture pic;
    atomic_bool deleteflag;
    PicNode* prev;
    PicNode* next;
    mutex* picnodemutex;

    // ME : REMEMBER TO EDIT MAKEFILE

  public:

    // class constructor, creates a picture node
    PicNode(Picture pic, const string filename){};

    // PicNode query methods - ME : ORGANISE
    string getfilename(PicNode picNode);
    Picture getpic(PicNode picNode);
    void setpic(Picture newpic);
    atomic_bool getdeleteflag();
    void toggledeleteflag();
    PicNode* getprev();
    PicNode* getnext();

    // PicNode update methods
    void setprev(PicNode* prev);
    void setnext(PicNode* next);

};

#endif
