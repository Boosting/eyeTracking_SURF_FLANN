#include "orb.h"

eyeT::ORB::ORB()
{}

void eyeT::ORB::find_descriptors(cv::Mat &img)
{
    detector.detect (img, this->keyPnts);
    extractor.compute (img, this->keyPnts, this->descriptors);
}

cv::Mat eyeT::ORB::get_descriptors_image()
{
    if (this->descriptors.empty())
        return cv::Mat();

    return this->descriptors;
}

int eyeT::ORB::get_keyPoints_number()
{
    return this->keyPnts.size();
}

std::vector< cv::KeyPoint > eyeT::ORB::get_keyPoints()
{
    return this->keyPnts;
}

void eyeT::ORB::set_newParams( int nfeatures, float scaleFactor, int nlevels, int edgeThreshold,
                               int firstLevel, int WTA_K, int scoreType, int patchSize )
{
    this->detector = cv::OrbFeatureDetector( nfeatures, scaleFactor, nlevels, edgeThreshold,
                                             firstLevel, WTA_K, scoreType, patchSize );
    this->extractor = cv::OrbDescriptorExtractor( nfeatures, scaleFactor, nlevels, edgeThreshold,
                                                  firstLevel, WTA_K, scoreType, patchSize );
}
