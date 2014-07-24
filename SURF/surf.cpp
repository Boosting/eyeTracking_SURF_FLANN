/*
 * Referencias:
 * [1] - http://stackoverflow.com/questions/17613723/whats-the-meaning-of-minhessian-surffeaturedetector
 */

#include "surf.h"

eyeT::SURF::SURF(double _minHessian)
{
    this->detector = cv::SurfFeatureDetector (_minHessian);
}

void eyeT::SURF::find_descriptors(cv::Mat &img)
{
    detector.detect (img, this->keyPnts);
    extractor.compute (img, this->keyPnts, this->descriptors);
}

void eyeT::SURF::set_minHessian(double __minHessian)
{
    this->detector.hessianThreshold = __minHessian;
}

double eyeT::SURF::get_minHessian()
{
    return this->detector.hessianThreshold;
}

cv::Mat eyeT::SURF::get_descriptors_image ()
{
    if (this->descriptors.empty())
        return cv::Mat();

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
