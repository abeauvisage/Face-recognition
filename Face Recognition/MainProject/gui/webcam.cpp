#include "webcam.h"

Webcam::Webcam(): videoCapture(0), timer(0), rectangle(0)
{
    videoCapture = new cv::VideoCapture(0);
    if(videoCapture->isOpened()){
        timer = new QTimer(this);
        rectangle = new cv::Rect((int)videoCapture->get(CV_CAP_PROP_FRAME_WIDTH)/2-150,(int)videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT)/2-150,300,300);
        connect(timer, SIGNAL(timeout()), this, SLOT(readImage()));
    }
}

bool Webcam::isOpened() const{
    return videoCapture->isOpened();
}

void Webcam::setImage(cv::Mat image){
    QMutexLocker locker(&mutex);
    img = image;
}

cv::Mat Webcam::getImage(bool rect){
    QMutexLocker locker(&mutex);
    cv::Mat copy = img.clone();
    if(rect){
            cv::rectangle(copy,cvPoint(rectangle->x-1,rectangle->y-1),cvPoint(rectangle->x+rectangle->width+1,rectangle->y+rectangle->height+1),cvScalar(0,255,0));
    }
    return copy;
}

cv::Mat Webcam::getCroppedImage(){
    cv::Mat img = getImage();
    cv::Mat croppedImg(img,*rectangle);
    return croppedImg;
}

void Webcam::readImage(){
    cv::Mat src;
    videoCapture->read(src);
    setImage(src);
    //imshow("output",src);
    emit imageChanged();
}

void Webcam::run(){
    start();
}

void Webcam::start(){
    timer->start(50);
}

void Webcam::stop(){
    timer->stop();
}
