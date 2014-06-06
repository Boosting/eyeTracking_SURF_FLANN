#include "surf.h"

void eyeT::SURF::SURF(int _minHessian)
    : detector (_minHessian)
{}

virtual void ~SURF () {}

void eyeT::SURF::make_descriptors(cv::Mat &img)
{
    detector.detect (img, this->keyPnts);
    extractor.compute (img, this->keyPnts, this->descriptors);
}

void eyeT::SURF::set_minHessian(double __minHessian)
{
    this->detector = __minHessian;
}

double eyeT::SURF::get_minHessian()
{
    return this->detector.hessianThreshold;
}

cv::Mat eyeT::SURF::get_descriptors_image ()
{
    if (this->descriptors.empty())
        return NULL;

    return this->descriptors;
}

int eyeT::SURF::get_keyPoints_number()
{
    return this->keyPnts.size();
}

std::vector<cv::KeyPoint> eyeT::SURF::get_keyPoints ()
{
    return this->keyPnts;
}
