#include "PicLibrary.hpp"
#include "Colour.hpp"

void PicLibrary::print_picturestore() {

}

void PicLibrary::loadpicture(string path, string filename) {

}

void PicLibrary::unloadpicture(string filename) {

}

void PicLibrary::savepicture(string filename, string path) {

}

void PicLibrary::display(string filename) {

}

void PicLibrary::invert(string filename) {
    Picture pic = Picture(filename); // ME : CONSTRUCTOR USES filepath, NOT filename
    for (int x = 0; x < pic.getwidth(); x++) {
        for (int y = 0; y < pic.getheight(); y++) {
            Colour pixel = pic.getpixel(x, y); // ME : USE auto OR typeOfVariable CONSISTENTLY? CHOOSE WHEN AUTO IS APPROPRIATE?
            pic.setpixel(x, y, Colour(255 - pixel.getred(), 255 - pixel.getgreen(), 255 - pixel.getgreen()));
        }
    }
    pic.setimage(pic.getimage()); // ME : ARE PICTURE TRANSFORMATIONS APPLIED TO CURRENT PIC OR DO WE SAVE TO NEW PIC? THIS LINE IS WRONG ANYWAYS, YOU'RE SETTING TO THE SAME PICTURE
}

void PicLibrary::grayscale(string filename) {
    Picture pic = Picture(filename); // ME : CONSTRUCTOR USES filepath, NOT filename
    for (int x = 0; x < pic.getwidth(); x++) {
        for (int y = 0; y < pic.getheight(); y++) {
            Colour pixel = pic.getpixel(x, y);
            int avg = (pixel.getred() + pixel.getgreen() + pixel.getblue()) / 3; // ME : IS THIS SUFFICIENT FOR INTEGER DIVISION?
            pic.setpixel(x, y, Colour(avg, avg, avg));
        }
    }
}

void PicLibrary::rotate(int angle, string filename) {
    Picture pic = Picture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic;
    switch(angle) { // ME : IS THERE A WAY TO COMBINE THE STARTING OPERATIONS OF 90 AND 270?
        case 90 : {
            newpic = Picture(picheight, picwidth);
            for (int x = 0; x < picwidth; x++) {
                for (int y = 0; y < picheight; y++) {
                    Colour pixel = pic.getpixel(x, y); // ME : DEFINE A MACRO FOR THIS IN THE HEADER OF Picture.hpp?
                    newpic.setpixel(newpic.getwidth() - y, x, pixel); // ME : CONSISTENCY WITH DIFFERENT USES OF e.g. picwidth AND pic.getwidth AND newPic.getheight etc.
                }
            }
        }
        case 180 : {
            newpic = Picture(picwidth, picheight);
            for (int x = 0; x < picwidth; x++) {
                for (int y = 0; y < picheight; y++) {
                    Colour pixel = pic.getpixel(x, y);
                    newpic.setpixel(picwidth - x, picwidth - y, pixel);
                }
            }
        }
        case 270 : {
            newpic = Picture(picheight, picwidth);
            for (int x = 0; x < picwidth; x++) {
                for (int y = 0; y < picheight; y++) {
                    Colour pixel = pic.getpixel(x, y);
                    newpic.setpixel(y, newpic.getheight() - x, pixel);
                }
            }
        }
        default: {
            // ME : OUTPUT ERROR MESSAGE TO COMMAND LINE;
        }
    }
}

void PicLibrary::flipVH(char plane, string filename) {
    Picture pic = Picture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic;
    if (plane == 'H') {
        newpic = Picture(picwidth, picheight);
        for (int x = 0; x < picwidth; x++) {
            for (int y = 0; y < picheight; y++) {
                // ME : FINISH
            }
        }
    }
}

void PicLibrary::blur(string filename) {

}
