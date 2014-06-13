#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

namespace eyeT
{
class Homography
{
private:

    std::vector <cv::Point2f> obj_corners;
    std::vector <cv::Point2f> scene_obj_corners;

    std::vector <cv::Point2f> obj;
    std::vector <cv::Point2f> scene;

    cv::Mat H;

public:
    Homography ();

    void find_homography (std::vector <cv::KeyPoint>& obj_keyPoints,
                          std::vector <cv::KeyPoint>& scene_keyPoints,
                          std::vector <cv::DMatch>& good_matches);

    void perspective_transform (cv::Size &obj_img_size);

};
}

#endif // HOMOGRAPHY_H
