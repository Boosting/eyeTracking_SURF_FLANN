#include "Homography.h"

eyeT::Homography::Homography() : obj_corners (std::vector <cv::Point2f> (4)),
                                 scene_obj_corners (std::vector <cv::Point2f> (4))

{
}

void eyeT::Homography::find_homography(std::vector <cv::KeyPoint>& obj_keyPoints,
                                   std::vector <cv::KeyPoint>& scene_keyPoints,
                                   std::vector <cv::DMatch>& good_matches)
{
    for (cv::DMatch& good_match : good_matches)
    {
        //-- Get the keypoints from the good matches
        this->obj.push_back (obj_keyPoints[good_match.queryIdx].pt);
        this->scene.push_back (scene_keyPoints[good_match.trainIdx].pt);
    }

    this->H = cv::findHomography (obj, scene, CV_RANSAC);

}

void eyeT::Homography::perspective_transform(cv::Size& obj_img_size)
{
    this->obj_corners[0] = cv::Point (0,0);
    this->obj_corners[1] = cv::Point (obj_img_size.width,0);
    this->obj_corners[2] = cv::Point (obj_img_size.width, obj_img_size.height);
    this->obj_corners[3] = cv::Point (0,obj_img_size.height);

    cv::perspectiveTransform( this->obj_corners, this->scene_obj_corners, this->H);
}
