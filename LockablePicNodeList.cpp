#include "LockablePicNodeList.hpp"

using namespace std;

LockablePicNodeList::LockablePicNodeList() {
  head = NULL;
  tail = NULL;

  /* fine-grained locking */
  headlock = new mutex();
  taillock = new mutex();
}

Picture LockablePicNodeList::getpicnodepicture(string filename) {
  headlock->lock();

  PicNode* current = NULL;
  PicNode* next = head;

  string name;

  /* traversal of all PicNode elements */
  while (next != NULL && name != filename) {
    next->getlock()->lock();
    if (next == head) {
      headlock->unlock();
    } else {
      current->getlock()->unlock();
    }

    current = next;
    next = current->getnext();
    name = current->getfilename();
    if (name == filename) {
      // current->getlock()->unlock();
      // return true;
      current->getlock()->lock();
      return current->getpic();
    }
  }

  /* filename is not in the list so unlock everything locked by this method */
  /*if (current == NULL) { // ME : THE ASSUMPTION, IT'S BEEN CHECKED THAT filename exists
    headlock->unlock();
  } else {
    current->getlock()->unlock();
  }
  return false;*/
}

// ME : I DON'T THINK WE NEED THIS METHOD
void LockablePicNodeList::setpicnodepicture(string filename, Picture pic) {
  headlock->lock();

  PicNode* current = NULL;
  PicNode* next = head;

  string name;

  /* traversal of all PicNode elements */
  while (next != NULL && name != filename) {
    next->getlock()->lock();
    if (next == head) {
      headlock->unlock();
    } else {
      current->getlock()->unlock();
    }

    current = next;
    next = current->getnext();
    name = current->getfilename();
    if (name == filename) {
      // current->getlock()->unlock();
      // return true;
      current->setpic(pic);
      current->getlock()->unlock();
    }
  }

  /* filename is not in the list so unlock everything locked by this method */
  /*if (current == NULL) { // ME : THE ASSUMPTION, IT'S BEEN CHECKED THAT filename exists
    headlock->unlock();
  } else {
    current->getlock()->unlock();
  }
  return false;*/
}

/*
int LockablePicNodeList::getlength() {
  return length;
}
*/

/* implements fine-grained locking - looks for an element in the list -
 * returns true if filename found, false if not found */
bool LockablePicNodeList::search(string filename) {
  headlock->lock();

  PicNode* current = NULL;
  PicNode* next = head;

  string name;

  /* traversal of all PicNode elements */
  while (next != NULL && name != filename) {
    next->getlock()->lock();
    if (next == head) {
      headlock->unlock();
    } else {
      current->getlock()->unlock();
    }

    current = next;
    next = current->getnext();
    name = current->getfilename();
    if (name == filename) {
      current->getlock()->unlock();
      return true;
    }
  }

  /* filename is not in the list so unlock everything locked by this method */
  if (current == NULL) {
      headlock->unlock();
  } else {
      current->getlock()->unlock();
  }
  return false;
}

/* implements fine-grained locking - adds an element to the list (by filename
 * in lexicographical order) - returns true if inserted, false if not
 * inserted meaning PicNode with filename already exists in list */
bool LockablePicNodeList::insertordered(string path, string filename) {
  headlock->lock();

  PicNode* newNode = new PicNode(filename, Picture(path));

  /* special case for an empty list */
  if (head == NULL && tail == NULL){
    head = newNode;
    tail = newNode;
    headlock->unlock();
    length++;
    return true;
  }

  PicNode* prev = NULL;
  PicNode* current = NULL;
  PicNode* next = head;

  /* traversal of all PicNode elements */
  while (next != NULL) {
    next->getlock()->lock();
    if (current != NULL) {
      if (current == head) {
        headlock->unlock();
      } else {
        prev->getlock()->unlock();
      }
    }

    prev = current;
    current = next;
    next = current->getnext();
    string name = current->getfilename();

    if (name == filename) {
      delete(newNode);
      current->getlock()->unlock();
      if (prev != NULL) {
        prev->getlock()->unlock();
      } else {
        headlock->unlock();
      }
      return false;
    }

    if (name.compare(filename) > 0) {
      /* update newNode's pointers */
      newNode->setprev(prev);
      newNode->setnext(current);

      /* updates prev pointer of current */
      current->setprev(newNode);

      /* update next pointer of prev element (or list head) */
      if (prev != NULL) {
        prev->setnext(newNode);
        prev->getlock()->unlock();
      } else {
        head = newNode;
        headlock->unlock();
      }

      current->getlock()->unlock();
      length++;
      return true;
    }
  }

  /* if newNode not inserted in the while loop then item belongs on the end of the list */
  taillock->lock();
  tail->setnext(newNode);
  newNode->setprev(tail);
  tail = newNode;
  taillock->unlock();
  if (prev != NULL) {
    prev->getlock()->unlock();
  } else {
    headlock->unlock();
  }
  current->getlock()->unlock();
  length++;
  return true;
}

/* implements fine-grained locking -  removes an element from the list returns
 * true if removed, false if PicNode with filename doesn't exist */
bool LockablePicNodeList::remove(string filename) {
  headlock->lock();

  PicNode* prev = NULL;
  PicNode* current = NULL;
  PicNode* next = head;

  /* traversal of all PicNode elements */
  while (next != NULL) {
    next->getlock()->lock();
    if (current != NULL) { // ME : WHY????
      if (current == head) {
        headlock->unlock();
      } else {
        prev->getlock()->unlock();
      }
    }

    prev = current;
    current = next;
    next = current->getnext();
    string name = current->getfilename();

    if (name == filename) {

      /* updates prev pointer of next element (or list tail) */
      if (next != NULL) {
        next->getlock()->lock();
        next->setprev(prev);
      } else {
        taillock->lock();
        tail = prev;
      }

      /* updates next pointer of prev element (or list head) */
      if (prev != NULL) {
        prev->setnext(next);
      } else {
        head = next;
      }

      /* frees up memory for PicNode object */
      delete(current);

      if (next != NULL) {
        next->getlock()->unlock();
      } else {
        taillock->unlock();
      }

      if (prev != NULL) {
        prev->getlock()->unlock();
      } else {
        headlock->unlock();
      }

      length--;
      return true;
    }
  }

  if (prev != NULL) {
    prev->getlock()->unlock();
  } else {
    headlock->unlock();
  }

  if (current != NULL) {
    current->getlock()->unlock();
  }

  return false;
}

/* implements fine-grained locking - outputs the contents in the list by
 * printing all the filenames currently inside it */
void LockablePicNodeList::printallfilenames() {
  PicNode* current = NULL;
  PicNode* next = head;

  cout << "internal picture storage:" << endl;
  if (length != 0) {

    while (next != NULL) {
      current = next;
      next = current->getnext();
      cout << current->getfilename() << endl;
    }
  } else {
    cout << "(internal picture storage is currently empty)" << endl;
  }
}