// ImageProcessingApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "image.h"
#include "PGMFile.h"
#include <iostream>
#include <fstream>

using namespace std;

bool displayMenu(int &selection);

int main(int argc, char *argv[])
{
	
	int numXpixels=0, numYpixels=0, grayVal=0;	// cols, Rows, grayscale variables
	int imgStartPos = 0;
	int val=0;
	string fname;
	string fpath;
	bool done = false;		//To control when user want to exit app
	int selection;			//To hold value of users input
	int size;				//to hold user integer variable for function inputs
	int caseDone = 0;
	PGMFile pgmFile;

	cout << "Image Processing App" << endl;
	cout << "-----------------" << endl;


	while (done != true){
		cout << "Enter File path where image is located and hit enter.." << endl;
		cin >> fpath;
		cout << "Enter image name and extention and hit enter.." << endl;
		cin >> fname;
		pgmFile.setFilePath(fpath);
		pgmFile.setFileName(fname);
		pgmFile.setfullfname();

		if (pgmFile.getfileExist() && pgmFile.readFile(pgmFile, numXpixels, numYpixels, grayVal, imgStartPos)){
			done = true;
		}
	}
	done = false;
	

	Image image(numYpixels, numXpixels, grayVal);
	image.readImage(pgmFile, imgStartPos, image);

	while (done != true){
		if (displayMenu(selection)){
			switch(selection){
			case 1:
				//Display pixel values of image to console
				image.displayPixels();
				break;
			case 2:
				//Display Mean gray value of image to console
				cout << image.meanGray();
				break;
			case 3: 
				//edit image by enlarging
				cout << "How much larger do you want the image?" << endl;
				cin >> size;
				image.enlargeImage(size, image);
				break;
			case 4:
				//edit image by striping layer
				cout << "How many pixel layers do you want to strip from image?" << endl;
				cin >> size;
				image.stripImage(size, image);
				break;
			case 5:
				//	Rotate image 90 degrees
				image.rotateImage(image);
				cout << "Image Rotated 90 degrees" << endl;
				break;
			case 6:
				//Edit image by translating
				cout << "By how many pixels would you like to translate the image?" << endl;
				cin >> size;
				image.translateImage(size, image);
				break;
			case 7:
				//Edit image by reflecting
				cout << "How would you like to reflect the image?" << endl;
				cout << "Enter 1 for Horizontal" << endl;
				cout << "Enter 2 for Vertical" << endl;
				cin >> size;
				while(caseDone != 1){
					switch (size){
					case 1:
						image.reflectImage(true, image);
						caseDone = 1;
						break;
					case 2: 
						image.reflectImage(false, image);
						caseDone = 1;
						break;
					case 3:
						cout << "Please enter 1 or 2 only, thank you" << endl;
						break;
					}
				}
				break;
			case 0:
				//Close application
				done = 1;
				break;
			default:
				//If other number non-valid input is entered
				cout << "Input not valid, please select option from menu" << endl;
				break;
			}
		}	
	}

	std::cin.get();
	return 0;

}

bool displayMenu(int &selection)
{
	//int selection;
	string input;
	cout << endl;
	cout << "Image Processing App" << endl;
	cout << "-----------------" << endl;
	cout << "Please type one of the following menu options (1,2,3...etc) and hit enter" << endl;
	cout << "1) Display image" << endl;
	cout << "2) Get mean pixel value" << endl;
	cout << "3) Enlarge image" << endl;
	cout << "4) Strip image" << endl;
	cout << "5) Rotate image 90 degrees" << endl;
	cout << "6) Translate image" << endl;
	cout << "7) Reflect image" << endl;
	cout << "8) Write to file" << endl;
	cout << "0) Quit" << endl;
	cout << "------";
	cin >> input;
	cout << endl;

	//For loop checks that user entry is a valid integer input
	for (int i=0; i<input.length(); i++){
		if ((int)input[i] < 48 || (int)input[i]> 57 ){
			cout << "please input positive integer values only, thanks!" << endl;
			return false;
		}
	}


	selection = stoi(input);
	return true;
}

//void saveImage(){
//
//	ofstream newFile;
//	newFile.open("Test.txt");
//	newFile << "Hello" << endl;
//	newFile << "World" << endl;
//	newFile.close();
//	
//
//}

