#include "stdafx.h"
#include "PGMFile.h"
#include <fstream>
#include <iostream>

//Default Constructor
PGMFile::PGMFile(void)
{
	m_fpath = "";
	m_fname = "";
	m_fexists = false;
}

//Constructor
PGMFile::PGMFile(std::string fpath, std::string fname)
{
	m_fpath = fpath;
	m_fname = fname;
	PGMFile::setfullfname();
	bool fexists = PGMFile::getfileExist();
	PGMFile::setFileExist(fexists);
}



PGMFile::~PGMFile(void)
{
}

std::string PGMFile::getFilePath() const
	//Returns file path
{
	return m_fpath;
}

void PGMFile::setFilePath(std::string fpath)
	//Sets file Path
{
	m_fpath = fpath;
}

std::string PGMFile::getFileName() const
	//Return file name
{
	return m_fname;
}

void PGMFile::setFileName(std::string fname)
	//Sets file name
{
	m_fname = fname;
}

std::string PGMFile::getFullFileName() const
{
	return m_fullfname;
}

void PGMFile::setfullfname()
	//sets full name of file - location and name
{
	m_fullfname = m_fpath + m_fname;
}


bool PGMFile::getfileExist() const
	//Returns true if the file exists and stream is good
{
	std::ifstream f(m_fullfname.c_str());	//c_str() converts string to char* to allow string to work with ifstream library
	if(f.fail()){
		std::cout << "Wrong Path or File name entered..." << std::endl;
		return false;
	}

	return f.good();
}

void PGMFile::setFileExist(bool fexists)
	//Setter for mfexists
{
	m_fexists = fexists;
}

//bool PGMFile::readFile(std::string fullfname, int &xpixels, int &ypixels, int &grayval, int imageStartPosition) const
bool PGMFile::readFile(PGMFile& pgmFile, int &xpixels, int &ypixels, int &grayval, int &imageStartPosition) const
{

	char header [100], *ptr;
    std::ifstream ifp;

 /*   ifp.open(fullfname, std::ios::in | std::ios::binary);*/
    ifp.open(pgmFile.m_fullfname, std::ios::in | std::ios::binary);

    if (!ifp) 
    {
        std::cout << "Can't read image: " << pgmFile.m_fullfname << std::endl;
		return false;
    }

	// read header
    ifp.getline(header,100,'\n');	//Read first line header
    if ( (header[0] == !80) && (header[1]== !53) ) //If header does not indicate that file is PGM then exit function
    {  
      std::cout << "Image " << pgmFile.m_fullfname << " is not PGM, Header must start with P5 magic number" << std::endl;
      return false;
    }

    ifp.getline(header,100,'\n');	//Get next line from file, Ignore it if is a comment section (#)
    while(header[0]=='#'){
        ifp.getline(header,100,'\n');
	}

    xpixels=strtol(header,&ptr,0);	 //Read pixel column val and pixel row val
    ypixels=atoi(ptr);			

    ifp.getline(header,100,'\n');	//Read  max grayval 

    grayval=strtol(header,&ptr,0);
	if (grayval > 255) {
		std::cout << "Max grayval is greater than 255" << std::endl;
		return false;
	}


	imageStartPosition = ifp.tellg();	//Return position in pgm file where the image pixel data starts

	//count number characters in raw image 
	ifp.ignore( std::numeric_limits<std::streamsize>::max() ); 
	std::streamsize imageLength = ifp.gcount();		//gcount returns the number of characters extracted by the last unformatted input operation
	ifp.clear();									//Since ignore will have set eof.
	ifp.seekg(imageStartPosition);					//reset next character to be read position

	char* charImage = new char [imageLength+1];		//create char* to hold data read from pixel data
	char* ptrImage;

	ifp.read(charImage, imageLength);				//Read pixel data into charImage
	ifp.close();									//Close file
	charImage[imageLength] = NULL;					//Set Null character  of charImage
	std::string imageString = charImage;			//Save to string

	delete [] charImage;							//Delete heap memory

	//**********************************************//
	//--- This section checks that pixel data is    //
	//-------     formatted correctly      ---------//
	//**********************************************//

	long val = 0;				//Holds value of pixel
	int count_xpix = 0;			//Variable to count number of pixels in x direction
	int count_ypix = 0;			//Variable to count number of pixels in y direction
	std::string subImageString;	//Substring to hold rows 
	int subStringStart = 0;		//Variable to hold start position of substring
	int pos = 0;				//Iterate through substring xpix

	//Check if Image Data is valid as per header
	// 1. Loop over image data 1 char at a time
	for (int i=0; i<imageString.size()-1; i++){
		
		// 2. Check each char for end of line character '\n'
		if(imageString[i] == '\n'){
			
			// 3. Create substring to hold data Row
			subImageString = imageString.substr(subStringStart, i - subStringStart + 1);	//create substring of xpixels
			subStringStart = i+1;								//update substring position for next iteration

			// 4.Iterate over substring extracting pixel values
			for (pos; pos<=xpixels; pos++){												//reset errno to 0 before call
				val = strtol(subImageString.c_str(), &ptrImage, 0);		//Assign Val with next integer val

				// 5. Check following conditions that pixels values are valid -

					//check that a pixels has been found and increment x count if so
					if (subImageString.c_str() != ptrImage){
						count_xpix++;

						//Check if pixel is on a new row - Increment ypixel count if so
						if (pos == 0){
							count_ypix++;
						}
						// 5a. Check if number of pixels exceeds header file
						if (count_xpix > xpixels){
							std::cout << "Number of pixels in row " << count_ypix << " exceeds number specified in header" << std::endl;
							return false;
						}
						if (count_ypix > ypixels){
							std::cout << "Number of pixels in row " << count_ypix << " exceeds number specified in header" << std::endl;
							return false;
						}
					}
					// 5b. Check that there is a no pixel data after header data been satisfied - 
					if (val == 0 && subImageString.c_str() == ptrImage && count_ypix == ypixels && count_xpix == xpixels){
						continue;	
					}
					// 5c. Check there is the correct amount of xpixel entries in row 
					else if ((subImageString.c_str() == ptrImage) && (count_xpix != xpixels) && (count_ypix != ypixels)){
						std::cout << "File Corrupt: Image data does not match header" << std::endl;  
						std::cout << "Number of pixels in row " << count_ypix << " not correct" << std::endl;
						return false;
					}
					// 5d. Check that pixel value does not exceeed Max Gray Val called out in header
					else if (val > grayval){            //Checks that read pixel value are lower than max gray
						std::cout << "File Corrupt: pixel value greater than max gray value: " << grayval << std::endl;	
						return false;
					}

					subImageString = ptrImage;	//set substring equal to ptrImage
				}
			} 
			//End of substring reached reinitialise variables
			count_xpix = 0;			
			subImageString.clear();			//Clear substring so it can be reassigned in next iteration
			pos = 0;						//reset counter
	}
	return true;
}