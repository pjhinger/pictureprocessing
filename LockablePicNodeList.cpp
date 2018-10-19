#include "LockablePicNodeList.hpp"

using namespace std;

LockablePicNodeList::LockablePicNodeList() {
  cout << "inside constructor for list" << endl;
  head = new PicNode("\n", Picture("images/test.jpg"));
  tail = new PicNode("\n", Picture("images/test.jpg"));
  head->setnext(tail);
  cout << "finished constructing list" << endl;
}

/* returns a pair of PicNode* pair<prev, current> */
pair<PicNode*, PicNode*> LockablePicNodeList::findposition(string filename) {
  PicNode* prev = head;
  prev->getlock()->lock();
  PicNode* current = head->getnext();
  current->getlock()->unlock();
  while (current->getfilename().compare(filename) < 0) {// break when
    // filename found, iterate until the tail (check tail for name \n
    prev->getlock()->unlock();
    prev = current;
    current = current->getnext();
    current->getlock();
    if (current == tail) {
      break;
    }
  }
  prev->getlock()->unlock();
  current->getlock()->unlock();
  return make_pair(prev, current);
}

/* implements fine-grained locking - finds, locks and returns PicNode* if
 * filename found, otherwise returns a nullptr - every time a call to
 * findpicnode is made, unlockpicnode() (implemented below) must also be
 * called in order to unlock the node */
PicNode* LockablePicNodeList::findpicnode(string filename) { // ME : skipping a node if it's locked? (you can use filename to find out if you need to access it?)
  PicNode* picnode = findposition(filename).second;
  picnode->getlock()->lock();
  if (picnode->getfilename() == filename) { // ME : && picnode != tail ?????
    return picnode;
  }
  return nullptr;
}

/* used to unlock PicNode* picnode returned from the find method above after
 * transformations have been done on Picture */
void LockablePicNodeList::unlockpicnode(PicNode* picnode) {
  picnode->getlock()->unlock();
}

/* implements fine-grained locking - adds an element to the list (by filename
 * in lexicographical order) - returns true if inserted, false if not
 * inserted meaning PicNode with filename already exists in list */
bool LockablePicNodeList::insertordered(string path, string filename) {
  PicNode* newnode = new PicNode(filename, Picture(path));
  cout << "Visiting here";
  pair<PicNode*, PicNode*> position = findposition(filename);
  PicNode* prev = position.first;
  prev->getlock()->lock();
  PicNode* current = position.second;
  current->getlock()->lock();
  cout << "Visiting here";
  bool result = current->getfilename() == filename;
  if (result) {
    delete(newnode);
  } else {
    prev->setnext(newnode);
    newnode->setnext(current);
  }

  prev->getlock()->unlock();
  current->getlock()->unlock();
  return !result;
}

/* implements fine-grained locking -  removes an element from the list returns
 * true if removed, false if PicNode with filename doesn't exist */
bool LockablePicNodeList::remove(string filename) {
  pair<PicNode*, PicNode*> position = findposition(filename);
  PicNode* prev = position.first;
  prev->getlock()->lock();
  PicNode* current = position.second;
  current->getlock()->lock();

  bool result = current->getfilename() == filename;
  if (result) {
    prev->setnext(current->getnext());
  }

  prev->getlock()->unlock();
  current->getlock()->unlock();
  return result;
}

/* implements fine-grained locking - outputs the contents in the list by
 * printing all the filenames currently inside it */
void LockablePicNodeList::printallfilenames() {
  PicNode* prev = head;
  PicNode* current = head->getnext();

  cout << "internal picture storage:" << endl;
  if (head == nullptr && tail == nullptr) {
    cout << "(internal picture storage is currently empty)" << endl;
  } else {
    while (current != nullptr) {
      prev = current;
      current = current->getnext();
      cout << prev->getfilename() << endl;
    }
  }
}