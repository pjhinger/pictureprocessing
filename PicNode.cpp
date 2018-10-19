#include "PicNode.hpp"

using namespace std;

PicNode::PicNode(string filename, string path) {
  this->filename = filename;
  this->pic = Picture(path);
  // ME : deleteflag = false;
  next = nullptr;
  picnodemutex = new mutex(); /* fine-grained locking */
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

void PicNode::setnext(PicNode* next) {
  this->next = next;
}