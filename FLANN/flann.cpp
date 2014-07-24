/*
 * Referencias:
 * 1 - http://docs.opencv.org/doc/tutorials/features2d/feature_flann_matcher/feature_flann_matcher.html
 */
#include "flann.h"

eyeT::FLANN::FLANN () : matches_minDist (100.0), matches_maxDist (0.0),
                             distanceThreshold (0.02), use_distanceThreshold (false)
{}

void eyeT::FLANN::find_matches (eyeT::SURF& eye, eyeT::SURF& roi)
{
    this->matcher.match (eye.get_descriptors_image(), roi.get_descriptors_image(), this->matches);

    minMaxDist ();

    findGoodMatches ();
}

cv::Mat eyeT::FLANN::find_matches (eyeT::SURF& eye, eyeT::SURF& roi,
                                   cv::Mat& eyeTemplate, cv::Mat& frame)
{
    this->matcher.match (eye.get_descriptors_image(), roi.get_descriptors_image(), this->matches);

    minMaxDist ();

    findGoodMatches ();

    cv::Mat imgResul;
    cv::drawMatches (eyeTemplate, eye.get_keyPoints(), frame, roi.get_keyPoints(),
                     this->good_matches, imgResul, cv::Scalar::all(-1),
                     cv::Scalar::all(-1), std::vector <char>(),
                     cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    return imgResul;
}

double eyeT::FLANN::get_minDist()
{
    return this->matches_minDist;
}

double eyeT::FLANN::get_maxDist()
{
    return this->matches_maxDist;
}

int eyeT::FLANN::get_numOf_goodMatches()
{
    return this->good_matches.size();
}

std::vector<cv::DMatch> eyeT::FLANN::get_goodMatches()
{
    return this->good_matches;
}

int eyeT::FLANN::get_numOf_matches()
{
    return this->matches.size();
}

void eyeT::FLANN::set_DistanceThreshold(float __distance)
{
    this->distanceThreshold = __distance;
}

float eyeT::FLANN::get_DistanceTreshold()
{
    return this->distanceThreshold;
}

void eyeT::FLANN::set_use_DistanceThreshold()
{
    this->use_distanceThreshold = true;
}

void eyeT::FLANN::unSet_use_DistanceThreshold()
{
    this->use_distanceThreshold = false;
}

bool eyeT::FLANN::using_distanceThreshold()
{
    return this->use_distanceThreshold;
}


/********************************************************************************\
 ******************************* Funcoes Privadas *******************************
\********************************************************************************/

void eyeT::FLANN::minMaxDist ()
{
    this->matches_minDist = 100; this->matches_maxDist = 0;

    for (cv::DMatch& it : this->matches)
    {
        if (it.distance < this->matches_minDist)
            this->matches_minDist = it.distance;
        if (it.distance > this->matches_maxDist)
            this->matches_maxDist = it.distance;
    }
}

//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
//-- small)
//-- PS.- radiusMatch can also be used here.
void eyeT::FLANN::findGoodMatches ()
{
    this->good_matches.clear();

    for (cv::DMatch& it : this->matches)
        if (it.distance <= ((using_distanceThreshold()) ?
                this->distanceThreshold : cv::max(2 * this->matches_minDist, 0.02)))
            good_matches.push_back(it);
}
