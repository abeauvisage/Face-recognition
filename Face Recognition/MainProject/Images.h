#include "cv.h"
#include "dirent.h"
#include <vector>
#include <highgui.h> // open cv GUI include file

#pragma once
class Images
{
private:
	std::vector<cv::Mat*> ColorImages;
	std::vector<cv::Mat*> GrayImages;
	std::vector<int> labels;
	std::map<std::string, int> directory2label;
	std::map<int, std::string> label2directory;

	std::vector<std::string> getDirectory(std::string path, bool directory=0);



public:
	Images();
	Images(std::string path);
	~Images();

	void addImage(cv::Mat& ColorImage, int& label);
	unsigned int size() const;

	std::vector<cv::Mat*> getColorImages() const;
	std::vector<cv::Mat*> getGrayImages();
	std::vector<int> getLabels() const;
	
	cv::Mat* getColorImage(unsigned int i) const;
	cv::Mat* getGrayImage(unsigned int i);
	int getLabel(unsigned int i) const;

	int dir2label(std::string dir);
	std::string label2dir(int label);
};

std::vector<std::string> Images::getDirectory(std::string path, bool directory/* =0 */){
	std::vector<std::string> ret;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_DIR){
				//printf("Directory: %s\n", ent->d_name);
				ret.push_back(ent->d_name);
			}
			else if (!directory){
				//printf("%s\n", ent->d_name);
				ret.push_back(path + ent->d_name);
			}


		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("Directory error!");
		return ret;
	}
	return std::vector<std::string>(ret.begin()+2,ret.end());
}


Images::Images(){}

Images::Images(std::string path){
	std::vector<std::string> Directories;
	std::vector<std::vector<std::string> > files;
	Directories = getDirectory(path, 1);

	for (unsigned int DirectoryIterator = 0; DirectoryIterator < Directories.size(); ++DirectoryIterator){
		directory2label[Directories[DirectoryIterator]] = DirectoryIterator;
		label2directory[DirectoryIterator] = Directories[DirectoryIterator];
		files.push_back(getDirectory(path + Directories[DirectoryIterator] + "/"));
	}

	for (unsigned int DirectoryIterator = 0; DirectoryIterator < files.size(); ++DirectoryIterator)
	for (unsigned int FileIterator = 0; FileIterator < files[DirectoryIterator].size(); ++FileIterator){
		cv::Mat* tmpImg = new cv::Mat();
		*tmpImg = cv::imread(files[DirectoryIterator][FileIterator],CV_8SC3);
		ColorImages.push_back(tmpImg);
		GrayImages.push_back(NULL);
		labels.push_back(DirectoryIterator);
	}
}

Images::~Images(){
	for (unsigned int i = 0; i < ColorImages.size(); i++){
		delete(ColorImages[i]);
		delete(GrayImages[i]);
	}

}

void Images::addImage(cv::Mat& ColorImage, int& label){
	cv::Mat* tmpImage = new cv::Mat(ColorImage);
	ColorImage.copyTo(*tmpImage);
	ColorImages.push_back(tmpImage);
	GrayImages.push_back(NULL);
	labels.push_back(label);
}

unsigned int Images::size() const{
	return ColorImages.size();
}

std::vector<cv::Mat*> Images::getColorImages() const
{
	return(ColorImages);
}

std::vector<cv::Mat*> Images::getGrayImages()
{
	if (this->size() == 0)
		return std::vector<cv::Mat*>(0);
	if (GrayImages[0] == NULL)
	{
		for (unsigned int ImageIterator = 0; ImageIterator < this->size(); ++ImageIterator){
			cv::cvtColor(*ColorImages[ImageIterator], *GrayImages[ImageIterator], CV_RGBA2GRAY);
		}
	}

	return(GrayImages);
}

std::vector<int> Images::getLabels() const
{
	return(labels);
}

cv::Mat* Images::getColorImage(unsigned int i) const{
	if (i >= this->size())
		return NULL;
	return(ColorImages[i]);
}

cv::Mat* Images::getGrayImage(unsigned int i){
	if (i >= this->size())
		return NULL;
	if (GrayImages[0] == NULL)
	{
		for (unsigned int ImageIterator = 0; ImageIterator < this->size(); ++ImageIterator){
			cv::Mat* tmpImage = new cv::Mat(*ColorImages[ImageIterator]);
			GrayImages[ImageIterator] = tmpImage;
			cv::cvtColor(*ColorImages[ImageIterator], *GrayImages[ImageIterator], CV_RGBA2GRAY);
		}
	}

	return(GrayImages[i]);
}

int Images::getLabel(unsigned int i) const{
	if (i >= this->size())
		return NULL;
	return(labels[i]);
}

int Images::dir2label(std::string dir){
	return (directory2label[dir]);
}

std::string Images::label2dir(int label){
	return (label2directory[label]);
}