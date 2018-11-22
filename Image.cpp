#include "stdafx.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include <cmath>
//#include <string>
using namespace std;

//Default Constructor
Image::Image(void)
{
	mX_Pixels = 0;
	mY_Pixels = 0;
	mGrayLevel = 0;
	mPixelVal = NULL;
}

//Constructor
Image::Image(int numYPixels, int numXPixels, int grayLevel)
{
	mX_Pixels = numXPixels;
	mY_Pixels = numYPixels;
	mGrayLevel = grayLevel;

	//initialise pixelVal 2D array
	mPixelVal = new int*[mY_Pixels];
	for (int i=0; i<mY_Pixels; i++){
		mPixelVal[i] = new int[mX_Pixels];
		for (int j=0; j<mX_Pixels; j++){
			mPixelVal[i][j] = 0;
		}
	}
}

Image::~Image(void)
{
	mX_Pixels = 0;
    mY_Pixels = 0;
    mPixelVal = 0;
    
    for(int i = 0; i < mY_Pixels; i++)
        delete mPixelVal [mY_Pixels];
    
    delete mPixelVal;
}


Image::Image(const Image& oldImage)
/*copies oldImage into new Image object*/
{    
    mX_Pixels = oldImage.mX_Pixels;
    mY_Pixels = oldImage.mY_Pixels;
    mGrayLevel = oldImage.mGrayLevel;
    
    mPixelVal = new int* [mY_Pixels];
    for(int i = 0; i < mY_Pixels; i++)
    {
        mPixelVal[i] = new int [mX_Pixels];
        for(int j = 0; j < mX_Pixels; j++)
            mPixelVal[i][j] = oldImage.mPixelVal[i][j];
    }
}

void Image::operator=(const Image& oldImage)
//*copies oldImage into whatever you = it to*/
{
    mX_Pixels = oldImage.mX_Pixels;
    mY_Pixels = oldImage.mY_Pixels;
    mGrayLevel = oldImage.mGrayLevel;
    
    mPixelVal = new int* [mY_Pixels];
    for(int i = 0; i < mY_Pixels; i++)
    {
        mPixelVal[i] = new int [mX_Pixels];
        for(int j = 0; j < mX_Pixels; j++)
            mPixelVal[i][j] = oldImage.mPixelVal[i][j];
    }
}

void Image::setImageInfo(int ypixels, int xpixels, int grayVal)
/*sets the number of rows, columns and graylevels*/
{
    mY_Pixels = ypixels;
    mX_Pixels = xpixels;
    mGrayLevel = grayVal;
}

void Image::getImageInfo(int &ypixels, int &xpixels, int &grayVal)
/*returns the number of rows, columns and gray levels*/
{
    ypixels = mY_Pixels;
    xpixels = mX_Pixels;
    grayVal = mGrayLevel;
}

int Image::getPixelVal(int ypixel, int xpixel)
/*returns the gray value of a specific pixel*/
{
    return mPixelVal[ypixel][xpixel];
}

void Image::setPixelVal(int ypixel, int xpixel, int value)
/*sets the gray value of a specific pixel*/
{
    mPixelVal[ypixel][xpixel] = value;
}

bool Image::inBounds(int ypixel, int xpixel)
/*checks to see if a pixel is within the image, returns true or false*/
{
    if(ypixel >= mY_Pixels || ypixel < 0 || xpixel >=mX_Pixels || xpixel < 0)
        return false;
    else
	 return true;
}

void Image::getSubImage(int yorigin, int xorigin, int yend, 
    int xend, Image& oldImage)
/*Pulls a sub image out of oldImage based on users values, and then stores it
    in oldImage*/
{
    int width, height;
    
    width = xend - xorigin + 1;		//+1 is to account for the pixel that the user chooses
    height = yend - yorigin + 1;
    
    Image tempImage(height, width, mGrayLevel);
    
    for(int i = yorigin; i <= yend; i++)
    {
        for(int j = xorigin; j <= xend; j++)
            tempImage.mPixelVal[i - yorigin][j - xorigin] = oldImage.mPixelVal[i][j];
    }
    
    oldImage = tempImage;
}

float Image::meanGray()
/*returns the mean gray levels of the Image*/
{
    int totalGray = 0;
    
    for(int i = 0; i < mY_Pixels; i ++)
    {
        for(int j = 0; j < mX_Pixels; j++)
            totalGray += mPixelVal[i][j];
    }
    
    float cells = (float)mX_Pixels * (float)mY_Pixels;
    
    return ((float)totalGray / cells);
}

void Image::enlargeImage(int value, Image& oldImage)	//Must be a better way of implementing this
/*enlarges Image and stores it in tempImage, resizes oldImage and stores the 
    larger image in oldImage*/
{
     int ypixels, xpixels, gray;
     int pixel;
     int enlargeRow, enlargeCol;
     
     ypixels = oldImage.mY_Pixels * value;
     xpixels = oldImage.mX_Pixels * value;
     gray = oldImage.mGrayLevel;
     
     Image tempImage(ypixels, xpixels, gray);
     
     for(int i = 0; i < oldImage.mY_Pixels; i++)
     {
             for(int j = 0; j < oldImage.mX_Pixels; j++)
             {
                     pixel = oldImage.mPixelVal[i][j];
                     enlargeRow = i * value;
                     enlargeCol = j * value;
                     for(int c = enlargeRow; c < (enlargeRow + value); c++)
                     {
                             for(int d = enlargeCol; d < (enlargeCol + value); d++)
                             {
                                     tempImage.mPixelVal[c][d] = pixel;
                             }
                     }
             }
     }
     
     oldImage = tempImage;
}

void Image::stripImage(int value, Image& oldImage)
/*Shrinks image as storing it in tempImage, resizes oldImage, and stores it in
    oldImage*/
{
    int ypixels, xpixels, gray;
    
	ypixels = oldImage.mY_Pixels - value*2;
    xpixels = oldImage.mX_Pixels - value*2; 
    gray = oldImage.mGrayLevel; 
    
    Image tempImage(ypixels, xpixels, gray);
    
    for(int i = 0; i < ypixels; i++)
    {
        for(int j = 0; j < xpixels; j++)
            tempImage.mPixelVal[i][j] = oldImage.mPixelVal[i + value][j + value];
    }    
    oldImage = tempImage;
}

void Image::reflectImage(bool flag, Image& oldImage)
/*Reflects the Image based on users input*/
{
    int ypixels = oldImage.mY_Pixels;
    int xpixels = oldImage.mX_Pixels;
    Image tempImage(oldImage);
    if(flag == true) //horizontal reflection
    {
        for(int i = 0; i < ypixels; i++)
        {
            for(int j = 0; j < xpixels; j++)
                tempImage.mPixelVal[ypixels - (i + 1)][j] = oldImage.mPixelVal[i][j];
        }
    }
    else //vertical reflection
    {
        for(int i = 0; i < ypixels; i++)
        {
            for(int j = 0; j < xpixels; j++)
                tempImage.mPixelVal[i][xpixels - (j + 1)] = oldImage.mPixelVal[i][j];
        }
    } 
    
    oldImage = tempImage;
}

void Image::translateImage(int value, Image& oldImage)
/*translates image down and right based on user value*/
{
    int ypixels = oldImage.mY_Pixels;
    int xpixels = oldImage.mX_Pixels;
    int gray = oldImage.mGrayLevel;
    Image tempImage(mY_Pixels, mX_Pixels, mGrayLevel);
    
    for(int i = 0; i < (ypixels - value); i++)
    {
        for(int j = 0; j < (xpixels - value); j++)
            tempImage.mPixelVal[i + value][j + value] = oldImage.mPixelVal[i][j];
    }
    
    oldImage = tempImage;
}

//void Image::rotateImage(int theta, Image& oldImage)
///*based on users input and rotates it around the center of the image.*/
//{
//    int r0, c0;
//    int r1, c1;
//    int ypixels, xpixels;
//    ypixels = oldImage.mY_Pixels;
//    xpixels = oldImage.mX_Pixels;
//    Image tempImage(ypixels, xpixels, oldImage.mGrayLevel);
//    
//    float rads = (theta * 3.14159265)/180.0;
//    
//    r0 = ypixels / 2;		//Get centre point of image
//    c0 = xpixels / 2;
//    
//    for(int r = 0; r < ypixels; r++)
//    {
//        for(int c = 0; c < xpixels; c++)
//        {
//            r1 = (int) (r0 + ((r - r0) * cos(rads)) - ((c - c0) * sin(rads))) - 1;
//            c1 = (int) (c0 + ((r - r0) * sin(rads)) + ((c - c0) * cos(rads)));
//            
//            if(inBounds(r1,c1))
//            {
//                tempImage.mPixelVal[r1][c1] = oldImage.mPixelVal[r][c];
//            }
//        }
//    }
//    
//    for(int i = 0; i < ypixels; i++)
//    {
//        for(int j = 0; j < xpixels; j++)
//        {
//            if(tempImage.mPixelVal[i][j] == 0)
//                tempImage.mPixelVal[i][j] = tempImage.mPixelVal[i][j+1];
//        }
//    }
//    oldImage = tempImage;
//}

void Image::rotateImage(Image& oldImage){
	//Rotate image by pixel
	int ypixels = oldImage.mY_Pixels; 
	int xpixels = oldImage.mX_Pixels;
	int grayVal = oldImage.mGrayLevel;

	Image tempImage(ypixels,xpixels,grayVal);
	int size = ypixels;		//Get length of image matrix
	int layerCount = size/2;			//Holds number of layers in matrix
	int first, last, offset;				//hold values of elements in matrix
	int top, rightSide, bottom, leftSide;	//variables to hold layer elements to be swapped

	//Loop to move through layers
	//i holds value to increment through layers
	for (int layer=0; layer<layerCount;layer++){
		
		first = layer;
		last = size - first - 1;

		//Loop to move through elements in layers
		//element holds value of element in layer
		for (int element=first; element<last; element++){

			offset = element - first;
			//element increments xpixels accross right
			top = oldImage.mPixelVal[first][element];
			//element increments ypixels along right side of layer
			rightSide = oldImage.mPixelVal[element][last];
			//offset decrements accross bottom matrix layer
			bottom = oldImage.mPixelVal[last][last-offset];
			//offset decrements along left side of matrix layer
			leftSide = oldImage.mPixelVal[last-offset][first];
			
			//Set top of Image equal to right side
			tempImage.mPixelVal[first][element] = rightSide;	
			//Set Right of image equal to bottom
			tempImage.mPixelVal[element][last] = bottom;
			//Set bottom of image equal to left side
			tempImage.mPixelVal[last][last-offset] = leftSide;
			//Set left side equal to bottom
			tempImage.mPixelVal[last-offset][first] = top;
		}
	}

	//If there there is an odd number of  Rows/Cols then the centre value needs to be asigned
	if ((ypixels%2)!=0){
		int i = ypixels/2;
		tempImage.mPixelVal[i][i] = oldImage.mPixelVal[i][i];
	}

	oldImage = tempImage;

}



Image Image::operator+(const Image &oldImage)
 /*adds images together, half one image, half the other*/
{
    Image tempImage(oldImage);
    
    int ypixels, xpixels;
    ypixels = oldImage.mY_Pixels;
    xpixels = oldImage.mX_Pixels;
    
    for(int i = 0; i < ypixels; i++)
    {
        for(int j = 0; j < xpixels; j++)
            tempImage.mPixelVal[i][j] = (mPixelVal[i][j] + oldImage.mPixelVal[i][j]) / 2;
    }
    
    return tempImage;
}

Image Image::operator-(const Image& oldImage)
/*subtracts images from each other*/
{
    Image tempImage(oldImage);
    
    int ypixels, xpixels;
    ypixels = oldImage.mY_Pixels;
    xpixels = oldImage.mX_Pixels;
    int tempGray = 0;
    
     for(int i = 0; i < ypixels; i++)
    {
        for(int j = 0; j < xpixels; j++)
        {
            
            tempGray = abs(mPixelVal[i][j] - oldImage.mPixelVal[i][j]);
            if(tempGray < 35)// accounts for sensor flux
                tempGray = 0;
            tempImage.mPixelVal[i][j] = tempGray;
        }
    
    }
    
    return tempImage;    
}

void Image::negateImage(Image& oldImage)
/*negates image*/
{
    int ypixels, xpixels, gray;
    ypixels = mY_Pixels;
    xpixels = mX_Pixels;
    gray = mGrayLevel;
    
    Image tempImage(mY_Pixels,mX_Pixels,mGrayLevel);
    
    for(int i = 0; i < ypixels; i++)
    {
        for(int j = 0; j < xpixels; j++)
            tempImage.mPixelVal[i][j] = -(mPixelVal[i][j]) + 255;
    }
    
    oldImage = tempImage;
}

//Output entire grayscale image to console
void Image::displayPixels() const
{
	cout << endl<< "Displaying gray values of image:" << endl << endl;
	int tempPix;
	char ch = '-';
	int count;	// x;
	for (int i=0;i<mY_Pixels; i++){
		if(i!=0){
			cout << endl;
			//for(x=0; x<80; x++){
			//cout << "-";
			//}
			//cout << endl;
		}
		for (int j=0; j<mX_Pixels; j++){
			tempPix = mPixelVal[i][j];
			count = 1;
			while (tempPix>=10){
				tempPix = tempPix/10;		//Find how many characters is in pixel size
				count++;
			}
					switch (count)
			{
			case 1: 
				cout << " " << mPixelVal[i][j] << " ";
				break;
			case 2:
				cout << " " << mPixelVal[i][j];
				break;
			case 3:
				cout << mPixelVal[i][j];
				break;
			}
			if (j!=(mX_Pixels-1)){
				cout << "|";
			}

		}
	}
}

//Output specific image pixel to console
void Image::displayPixelsSpecific(int ypixel, int xpixel) const
{
	cout << endl << "Displaying Specific Row: " << ypixel << " Col: " << xpixel << endl ;
	cout << mPixelVal[ypixel][xpixel];
}

int Image::readImage(PGMFile& pgmFile, int imageStartPostion, Image &image){

    ifstream ifp;		//ifstream for input
	std::string fname;
	fname = pgmFile.getFullFileName();
    ifp.open(fname, ios::in | ios::binary); //open image.
	
	//Check that image file opened successfully
    if (!ifp)  
    {
        cout << "Can't read image: " << fname << endl;
        exit(1);
    }

	////Sets next character to be read to be the first pixel val of image
	ifp.seekg(imageStartPostion);

	////count number characters in raw image 
	ifp.ignore( std::numeric_limits<std::streamsize>::max() ); 
	std::streamsize imageLength = ifp.gcount();		//gcount returns the number of characters extracted by the last unformatted input operation
	ifp.clear();									//Since ignore will have set eof.
	ifp.seekg(imageStartPostion);					//reset next character to be read position

	char* charImage = new char [imageLength+1];		//create char* to hold data read from image file
	char* ptrImage = new char[1];	 			    //Pointer to parse data from charImage

	//Read data from file and save it to charImage
	ifp.read(charImage, imageLength);
	charImage[(imageLength)] = NULL;			//adds null pointer to charImage

	//Check that data was successfully read
    if (ifp.fail()) 
    {
        cout << "Image " << fname << " has wrong size" << endl;	
        exit(1);
    }

    ifp.close();		//Close file

    int val = 0;
	int posY = 0, posX = 0;
	int pos = 0;
	
	for(pos; pos<mX_Pixels*mY_Pixels; pos++){   //Number of iteration changes because strtol function handles while space characters
		
		val = strtol(charImage, &ptrImage, 0);  //Takes integer number in charImage and saves it to val. then takes assigns ptrImage to array element after  integer
		charImage = ptrImage;					//UPdate charImage to remainder of char*

		image.setPixelVal(posY, posX, val);		//Convert to real number format
		//image.displayPixelsSpecific(posY, posX);
		posX++;
	
		if (posX == mX_Pixels){
			posX = 0;		//This if statement is required to go to next Row once end of Column has been reached
			posY++;
		}
	}

    return (1);
}
