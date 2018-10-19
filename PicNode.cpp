#include "PicNode.hpp"

using namespace std;

PicNode::PicNode(string filename, string path) {
  this->filename = filename;
  this->pic = Picture(path);
  next = nullptr;
  picnodemutex = new mutex();
}

string PicNode::getfilename() {
  return filename;
}

Picture PicNode::getpic() {
return pic;
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

void PicNode::setnext(PicNode* next) {
  this->next = next;
}