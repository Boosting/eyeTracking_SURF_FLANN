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

    /** 4 pontos que delimitam o objeto, e o objeto dentro do frame. */
    std::vector <cv::Point2f> obj_corners;
    std::vector <cv::Point2f> scene_obj_corners;

    /** Indica se a transfomracao de perspectiva ja foi realizada. */
    bool transformed_perpective;
    /** Indica se a matriz Hessiana ja foi calculada*/
    bool hessian_mastrix_found;
    /** Indica se o 'rotated_rect' atual corresponde aos dados atuais */
    bool rotated_rect_isCurrent;
    /** Indica se o 'rect' atual corresponde aos dados atuais */
    bool rect_isCurrent;
//    /* Angulo de inclinacao do 'rotated_rect' */
//    double rotated_rect_angle;
	
    /** KeyPoints of Good Matches. */
    std::vector <cv::Point2f> obj;
    std::vector <cv::Point2f> scene;
	
    /** Matriz de homograﬁa que deﬁne o mapeamento de um conjunto
     *  de pontos correspondentes entre dois planos[1].          */
    cv::Mat H;

    /** Retangulo rotacionado que delimita a posicao do objeto no frame */
	cv::RotatedRect rotated_rect;
    /** Imagem com o conteudo de 'rotated_rect' */
    cv::Mat rotated_rect_img;

public:
    Homography ();

    /**
     * @brief find_homography   Utiliza os keypoints dos bons casamentos para gerar
     *                          uma matriz de homografia, que mapeia os pontos de um plano em outro.
     *
     * @param obj_keyPoints     Pontos caracteristicos do objeto (olho).
     *
     * @param scene_keyPoints   Pontos caracteristicos do frame
     *                          (imagem onde esta se procurando o olho).
     *
     * @param good_matches      Bons casamentos entre os pontos detectados anteriormente.
     */
    void find_homography (const std::vector <cv::KeyPoint>& obj_keyPoints,
                          const std::vector <cv::KeyPoint>& scene_keyPoints,
                          const std::vector <cv::DMatch>& good_matches);

    /**
     * @brief perspective_transform     Utiliza a matriz de homografia calculada previamente
     *                                  para se obter os pontos do objeto procurado (olho) no frame.
     *
     * @param obj_img_size              Tamanho do frame do objeto.
     *
     * @return true, caso tenha calculado a trasformacao de perspectiva,
     *         false, caso contrario, Pelo fato de nao se ter calculado a matriz Hessiana ainda.
     */
    bool perspective_transform (const cv::Size& obj_img_size);
	
    /**
     * @brief draw_lines    Desenha linhas baseadas nos 4 pontos detectados que delimitam
     *                      o objeto dentro do frame. Deve-se ter usado o perspective_transform,
     *                      e consequentemente, o find_homography.
     *
     * @param img           Imagem onde sera desenhado as linhas.
     *
     * @param offset        Valor de deslocamento do desenho das linhas.
     *
     * @return true, caso tenha tenha ocorrido tudo como o esperado,
     *         false, caso nao tenha desenhado as linhas por conta de nao ter feito a
     *         transformacao de perspectiva ainda.
     */
    bool draw_lines (cv::Mat& img, cv::Point2f offset = cv::Point2f (0,0));
	
    /**
     * @brief get_rotated_rect  Determina um RotatedRect que compreende a aerea delimitada
     *                          pelos 4 pontos que caracterizam o objeto procurado dentro
     *                          do frame
     *
     * @return  um objeto da classe RotatedRect contendo as informacoes que especificam a
     *          area do objeto dento do frame, caso tenha ocorrido tudo bem,
     *          RotatedRect(), caso nao tenha sido feita ainda a transformacao de perspectiva.
     */
    cv::RotatedRect get_rotated_rect();

    /**
     * @brief get_rotated_rect_img  Usada para obter a imagem que esta contida dentro do RotatedRect.
     *
     * @param img                   Frame onde esta contido o objeto delimitado pelo RotatedRect.
     *
     * @return  Imagem do que esta dentro do RotedRect, caso tenha ocorrido tudo bem,
     *          cv::Mat(), caso o RotatedRect atual nao corresponda aos dados atuais, ou seja,
     *          RotatedRect esta desatualizado.
     */
    cv::Mat get_rotated_rect_img (cv::Mat& img);

    /**
     * @brief get_rotated_rect_angle Usado par se obter o angulo de rotacao do RotatedRect atual.
     *
     * @return Angulo do RotatedRect.
     */
    double get_rotated_rect_angle ();
};
}

#endif // HOMOGRAPHY_H
