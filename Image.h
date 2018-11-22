#pragma once
#ifndef IMAGE_H //Stops the header file being processed mukltiple times by the compiler
#define IMAGE_H
#include <string>
#include "PGMFile.h"
class Image
{
      public:
             Image();
             Image(int xPixels, int yPixels, int grayLevels);
             ~Image();
             Image(const Image& oldImage);
			 void displayPixels() const;
			 void displayPixelsSpecific(int x, int y) const;
			 int getPixelVal(int ypixel, int xpixel);
			 void setPixelVal(int xPixel, int yPixel, int value);
			  //function to input image file to mPixelVal. Arg 1: image file path/name Arg 2: start position of raw data after header
			 /*int readImage(char fname[], int imageStartPostion, Image &image);*/
			 /*int readImage(std::string fname, int imageStartPostion, Image &image);*/
			 int readImage(PGMFile& pgmFile, int imageStartPostion, Image &image);
             void operator=(const Image&);
             void setImageInfo(int ypixels, int xpixels, int grayVal);
             void getImageInfo(int &ypixels, int &xpixels, int &grayVal);
             bool inBounds(int row, int col);
             void getSubImage(int upperLeftRow, int upperLeftCol, 
				 int lowerRightRow, int lowerRightCol, Image& oldImage);
             float meanGray();
             void enlargeImage(int value, Image& oldImage);
             void stripImage(int value, Image& oldImage);
             void reflectImage(bool flag, Image& oldImage);
             void translateImage(int value, Image& oldImage);
             //     /*
             //       r' = r + t
             //       c' = c + t
             //     */
             void rotateImage(Image& oldImage);
             Image operator+(const Image &oldImage);
             Image operator-(const Image& oldImage);
             void negateImage(Image& oldImage);
      private:
              int mX_Pixels; // number of rows
              int mY_Pixels; // number of columns
              int mGrayLevel; // number of gray levels
              int **mPixelVal; //2D pointer to pointer integer array to hold image
};

#endif