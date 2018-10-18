#include "PicNode.hpp"

using namespace std;

PicNode::PicNode(string filename, Picture pic) {
  this->filename = filename;
  this->pic = pic;
  // ME : deleteflag = false;
  prev = NULL;
  next = NULL;
  picnodemutex = new mutex(); /* fine-grained locking */
  // ME : NEED TO SET PREV AND NEXT STILL
}

string PicNode::getfilename() {
  return filename;
}

Picture PicNode::getpic() {
return pic;
}

/*
atomic_bool PicNode::getdeleteflag() {
  return deleteflag;
}
*/

PicNode* PicNode::getprev() {
  return prev;
}

PicNode* PicNode::getnext() {
  return next;
}

/* fine-grained locking */
mutex* PicNode::getlock() {
  return picnodemutex;
}

void PicNode::setpic(Picture newpic) {
  pic = newpic;
}

/*
void PicNode::toggledeleteflag() {
  deleteflag = not deleteflag;
}
*/

void PicNode::setprev(PicNode* prev) {
  this->prev = prev;
}

void PicNode::setnext(PicNode* next) {
  this->next = next;
}