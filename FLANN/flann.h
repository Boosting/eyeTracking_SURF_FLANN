#ifndef FLANN_H
#define FLANN_H

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include "../SURF/surf.h"

namespace eyeT
{
    class FLANN
    {
    private:
        /***********  Variaveis ***********/

        cv::FlannBasedMatcher matcher;
        std::vector <cv::DMatch> matches, good_matches;

        double matches_minDist, matches_maxDist;

        float distanceThreshold;
        bool use_distanceThreshold;

        /***********  Funcoes ***********/

        void minMaxDist ();

        void findGoodMatches ();

    public:
        FLANN ();
        virtual void ~FLANN();


        void find_matches (eyeT::SURF& eye, eyeT::SURF& roi);

        cv::Mat find_matches (eyeT::SURF& eye, eyeT::SURF& roi,
                              cv::Mat& frameEYE, cv::Mat& frameROI);

        double get_minDist ();

        double get_maxDist ();

        int get_numOf_goodMatches ();

        int get_numOf_matches ();

        void set_use_DistanceThreshold ();

        void set_DistanceThreshold (float __distance);

        void unSet_use_DistanceThreshold ();

        bool using_distanceThreshold();
    };
}

#endif // FLANN_H
