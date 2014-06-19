#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

namespace eyeT
{
class Homography
{
private:

    /* 4 pontos que delimitam o objeto, e o objeto dentro do frame. */
    std::vector <cv::Point2f> obj_corners;
    std::vector <cv::Point2f> scene_obj_corners;

    /* Indica se a transfomracao de perspectiva ja foi realizada. */
    bool transformed_perpective;
    /* Indica se o 'rotated_rect' atual corresponde aos dados atuais */
    bool rotated_rect_isCurrent;
    /* Indica se o 'rect' atual corresponde aos dados atuais */
    bool rect_isCurrent;
//    /* Angulo de inclinacao do 'rotated_rect' */
//    double rotated_rect_angle;
	
    /* KeyPoints of Good Matches. */
    std::vector <cv::Point2f> obj;
    std::vector <cv::Point2f> scene;
	
    /* Matriz de homograﬁa que deﬁne o mapeamento de um conjunto
       de pontos correspondentes entre dois planos[1]. */
    cv::Mat H;

    /* Retangulo rotacionado que delimita a posicao do objeto no frame */
	cv::RotatedRect rotated_rect;
    /* Imagem com o conteudo de 'rotated_rect' */
    cv::Mat rotated_rect_img;

public:
    Homography ();

    void find_homography (std::vector <cv::KeyPoint>& obj_keyPoints,
                          std::vector <cv::KeyPoint>& scene_keyPoints,
                          std::vector <cv::DMatch>& good_matches);

    void perspective_transform (cv::Size& obj_img_size);
	
	bool draw_lines (cv::Mat& img, cv::Point2f& offset = cv::Point2f (0,0));
	
	bool get_transformed_perpective();
	
    cv::RotatedRect get_rotated_rect();

    cv::Mat get_rotated_rect_img (cv::Mat& img);
};
}

#endif // HOMOGRAPHY_H
