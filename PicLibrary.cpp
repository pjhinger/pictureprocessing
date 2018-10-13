#include "PicLibrary.hpp"
#include "Colour.hpp"

void PicLibrary::print_picturestore() {
  for (auto i = internalstorage.begin(); i != internalstorage.end(); i++) // ME : map<string, Picture>::iterator (USE auto WHEN DECLARING ITERATORS)
    cout << i->first << endl;
}

void PicLibrary::loadpicture(string path, string filename) { // ME : COME BACK TO TEST THIS
  Picture pic = Picture(path);
  if (internalstorage.count(filename) == 0) {
    if (true) // ME : isitjpg(path) ONLY NEEDS THE PATH, SO TAKE METHOD OUT OF PICTURE CLASS AND PUT IT SOMEWHERE APPROPRIATE
      internalstorage.insert(pair<string, Picture>(filename, pic));
    else
      cout << "error: could not load image into internal picture storage because " + path + " does not point to a .jpg file." << endl;
  } else
    cout << "error: could not load image into internal picture storage because " + filename + " already exists." << endl;
}

void PicLibrary::unloadpicture(string filename) {
  if (internalstorage.count(filename) != 0)
    internalstorage.erase(filename);
  else
    cout << "error: could not unload image from internal picture storage because " + filename + " doesn't exist." << endl;
}

void PicLibrary::savepicture(string filename, string path) { // ME : COME BACK TO TEST THIS
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    imgio.saveimage(pic.getimage(), path);
  } else
    cout << "error: could not locate image of " + filename + " from internal picture storage because the image doesn't exist." << endl;
  // ME : IF path EXISTS, SHOULD OVERWRITE IMAGE DATA
}

void PicLibrary::display(string filename) {
  char keystroke;
  imgio.displayimage(internalstorage[filename].getimage());
  // ME : WHAT ABOUT THE KEYSTROKE CONDITION (KEEP OPEN UNTIL KEYSTROKE ENTERED)
}

void PicLibrary::invert(string filename) {
  Picture pic = internalstorage[filename];
  for (int x = 0; x < pic.getwidth(); x++) {
    for (int y = 0; y < pic.getheight(); y++) {
      Colour pixel = pic.getpixel(x, y); // ME : USE auto OR typeOfVariable CONSISTENTLY? CHOOSE WHEN AUTO APPROPRIATE?
      pic.setpixel(x, y, Colour(255 - pixel.getred(), 255 - pixel.getgreen(), 255 - pixel.getblue()));
    }
  }
}

void PicLibrary::grayscale(string filename) {
  Picture pic = internalstorage[filename];
  for (int x = 0; x < pic.getwidth(); x++) {
    for (int y = 0; y < pic.getheight(); y++) {
      Colour pixel = pic.getpixel(x, y);
      int avg = (pixel.getred() + pixel.getgreen() + pixel.getblue()) / 3; // ME : IS THIS SUFFICIENT FOR INTEGER DIVISION?
      pic.setpixel(x, y, Colour(avg, avg, avg));
    }
  }
}

void PicLibrary::rotate(int angle, string filename) {
  Picture pic = internalstorage[filename];
  int picwidth = pic.getwidth();
  int picheight = pic.getheight();
  Picture newpic;
  switch (angle) { // ME : IS THERE A WAY TO COMBINE THE STARTING OPERATIONS OF 90 AND 270? YES - PUT THE FOR LOOPS ON THE OUTSIDE AND SWITCH INSIDE IT
    case 90 : {
      cout << "2here" << endl;
      newpic = Picture(picheight, picwidth);
      cout << "3here" << endl;
      for (int x = 0; x < picwidth; x++) {
        for (int y = 0; y < picheight; y++) {
          Colour pixel = pic.getpixel(x, y); // ME : DEFINE A MACRO FOR THIS IN THE HEADER OF Picture.hpp?
          newpic.setpixel(newpic.getwidth() - y - 1, x, pixel); // ME : CONSISTENCY WITH DIFFERENT USES OF e.g. picwidth AND pic.getwidth AND newPic.getheight etc.
        }
      }
      cout << "4here" << endl;
      break;
    }
    case 180 : {
      cout << "5here" << endl;
      newpic = Picture(picwidth, picheight);
      cout << "6here" << endl;
      for (int x = 0; x < picwidth; x++) {
        for (int y = 0; y < picheight; y++) {
          Colour pixel = pic.getpixel(x, y);
          newpic.setpixel(picwidth - x - 1, picheight - y - 1, pixel);
        }
      }
      cout << "7here" << endl;
      break;
    }
    case 270 : {
      cout << "8here" << endl;
      newpic = Picture(picheight, picwidth);
      cout << "9here" << endl;
      for (int x = 0; x < picwidth; x++) {
        for (int y = 0; y < picheight; y++) {
          Colour pixel = pic.getpixel(x, y);
          newpic.setpixel(y, newpic.getheight() - x - 1, pixel);
        }
      }
      cout << "10here" << endl;
      break;
    }
    default: {
      cout << "error: can only rotate images by 90, 180 or 270 degrees clockwise." << endl;
      break;
    }
  }
  internalstorage[filename] = newpic;
}

void PicLibrary::flipVH(char plane, string filename) {
  Picture pic = internalstorage[filename];
  int picwidth = pic.getwidth();
  int picheight = pic.getheight();
  Picture newpic = Picture(picwidth, picheight);
  if (plane == 'H') { // ME : IS THERE A GOOD WAY I COULD SIMPLIFY THE USE OF TWO NESTED FOR LOOPS? YES - PUT THE FOR LOOPS OUTSIDE AND THE IF-ELSE INSIDE
    for (int x = 0; x < picwidth; x++) {
      for (int y = 0; y < picheight; y++)
        newpic.setpixel(picwidth - x - 1, y, pic.getpixel(x, y));
    }
  } else if (plane == 'V') {
    for (int x = 0; x < picwidth; x++) {
      for (int y = 0; y < picheight; y++)
        newpic.setpixel(x, picheight - y - 1, pic.getpixel(x, y));
    }
  } else {
    // ME : ERROR MESSAGE? EXCEPTION THROWN
  }
  internalstorage[filename] = newpic;
}

void PicLibrary::blur(string filename) {
  Picture pic = internalstorage[filename];
  int picwidth = pic.getwidth();
  int picheight = pic.getheight();
  Picture newpic = Picture(picwidth, picheight);
  for (int x = 0; x < picwidth; x++) {
    for (int y = 0; y < picheight; y++) {
      newpic.setpixel(x, y, pic.blurpixel(x, y));
    }
  }
  internalstorage[filename] = newpic;
}
