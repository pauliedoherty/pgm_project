#pragma once
//#include <stdlib.h>
#include <string>
class PGMFile
{
public:
	PGMFile();
	PGMFile(std::string fpath, std::string fname);
	//PGMFile(const PGMFile &pgmFile);
	//void operator=(const PGMFile &pgmFile);
	~PGMFile();
	bool getfileExist() const;
	void setFileExist(bool fileExist);
	std::string getFilePath() const;
	void setFilePath(std::string fpath);
	std::string getFileName() const;
	void setFileName(std::string fname);
	std::string PGMFile::getFullFileName() const;
	std::string getfullfname() const;
	void setfullfname();
	bool readFile(PGMFile& pgmFile, int &xpixels, int &ypixels, int &grayvals, int &imageStartPosition) const;		//Checks is file header is PGM, matches image content and if no pixels has greater value that 255
	//int headerLength();		//Returns how many lines are in the header
	//int imgStartPosition(); //Return start position of image, this will used for image processing
	////int  xlength ylength //returns number of xpixel number and ypixel number
	//bool isCorrupt();		//Checks is file header is PGM, matches image content and if no pixels has greater value that 255
	//void addComment();		//add comment to existing pgm file
	//void writeFile();		//Method to write a new pgm file

private:
	std::string m_fpath;		//file location
	std::string m_fname;		//file name
	std::string m_fullfname;
	int m_pixels, m_ypixels, m_grayval, m_imageStartPosition;
	bool m_fexists;			//holds value if file exists or not

};

