#include <utility>

#include "LockablePicNodeList.hpp"

using namespace std;

LockablePicNodeList::LockablePicNodeList() {
  /* initialised dummy PicNodes for head and tail */
  head = new PicNode("\n", "images/test.jpg");
  tail = new PicNode("\n", "images/test.jpg");
  head->setnext(tail);
}

/* returns a pair of PicNode* pair<prev, current> */
pair<PicNode *, PicNode *> LockablePicNodeList::findposition(string filename) {
  PicNode *prev = head;
  prev->getlock()->lock();
  PicNode *current = head->getnext();
  current->getlock()->unlock();

  while (current->getfilename().compare(filename) < 0 &&
         current->getnext() != nullptr) {
    prev->getlock()->unlock();
    prev = current;
    current = current->getnext();
    current->getlock();
    if (current == tail) {
      break;
    }
  }

  return make_pair(prev, current);
}

/* implements fine-grained locking - finds, locks and returns PicNode* if
 * filename found, otherwise returns a nullptr - every time a call to
 * findpicnode is made, unlockpicnode() (implemented below) must also be
 * called in order to unlock the node */
PicNode *LockablePicNodeList::findpicnode(string filename) {
  PicNode *picnode = findposition(filename).second;
  if (picnode->getfilename() == filename) {
    return picnode;
  }
  return nullptr;
}

/* used to unlock pair of PicNode* returned from the find method above
 * after transformations have been done on Picture */
void LockablePicNodeList::unlockpicnodes(pair<PicNode *, PicNode *> pair) {
  pair.first->getlock()->unlock();
  pair.second->getlock()->unlock();
}

/* implements fine-grained locking - adds an element to the list (by filename
 * in lexicographical order) - returns true if inserted, false if not
 * inserted meaning PicNode with filename already exists in list */
bool LockablePicNodeList::insertordered(string path, string filename) {
  PicNode *newnode = new PicNode(filename, path);

  pair<PicNode *, PicNode *> position = findposition(filename);
  PicNode *prev = position.first;
  PicNode *current = position.second;

  bool result = current->getfilename() == filename;
  if (result) {
    delete (newnode);
  } else {
    prev->setnext(newnode);
    newnode->setnext(current);
  }

  prev->getlock()->unlock();
  current->getlock()->unlock();
  return !result;
}

/* implements fine-grained locking - removes an element from the list - returns
 * true if removed, false if PicNode with filename doesn't exist */
bool LockablePicNodeList::remove(string filename) {
  pair<PicNode *, PicNode *> position = findposition(filename);
  PicNode *prev = position.first;
  PicNode *current = position.second;

  bool result = current->getfilename() == filename;
  if (result) {
    prev->setnext(current->getnext());
  }

  prev->getlock()->unlock();
  current->getlock()->unlock();
  return result;
}

/* outputs the contents in the list by printing all the filenames currently
 * inside it */
void LockablePicNodeList::printallfilenames() {
  PicNode *prev = head;
  PicNode *current = head->getnext();

  cout << "internal picture storage:" << endl;
  while (current != nullptr) {
    prev = current;
    current = current->getnext();
    cout << prev->getfilename() << endl;
  }
}