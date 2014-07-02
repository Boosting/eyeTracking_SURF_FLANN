#ifndef SURF_H
#define SURF_H

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>

namespace eyeT
{
    class SURF
    {
    private:
        /* Detector dos pontos caracteristicos do frame. */
        cv::SurfFeatureDetector detector;
        /* Estrutra que guarda os pontos caracteristicos detectados. */
        std::vector <cv::KeyPoint> keyPnts;
        /* Extrator dos descritores do frame (obtido com base nos KeyPoints). */
        cv::SurfDescriptorExtractor extractor;
        /* Estrutura no formato de imagem (matriz) que representa os descritores detectados. */
        cv::Mat descriptors;

    public:
        SURF (double _minHessian = 400.0);

        /**
         * @brief find_descriptors    Extrai os descritores de uma imagem passada
         *                            como parametro e grava o resultado no atributo
         *                            'descriptors'.
         *
         * @param img    Imagem que sera usada para calcular os descritores.
         *
         *
         * @param keyPnts    Vector que será usado para guardar todos os
         *                   pontos caracteristicos.
         */
        void find_descriptors (cv::Mat& img);

        /**
         * @brief set_minHessian    Atribuir um novo limiar para a Matriz Hessiana usada no
         *                          detector de pontos chave (key points -> keyPnts)
         *
         * @param __minHessian      Valor do limiar para a Matriz Hesssiana
         */
        void set_minHessian (double __minHessian);

        /**
         * @brief get_minHessian    Pegar o limiar para a Matriz Hessiana sendo utilizado
         *                          para detectar os keypoints.
         *
         * @return Limiar para a Matriz Hessiana
         */
        double get_minHessian ();

        /**
         * @brief get_descriptors_image     Pegar os descritores ddo frame processado.
         *
         * @return Uma imagem que representa os descritores do frame processado.
         */
        cv::Mat get_descriptors_image ();

        /**
         * @brief get_keyPoints_number  Pegar o numero de pontos caracteristicos detectados.
         *
         * @return quantidade de pontos caracteristicos (keypoints)
         */
        int get_keyPoints_number ();

        /**
         * @brief get_keyPoints     Pegar a estrutura que guarda os pontos
         *                          caracteristicos detectados, e suas informacoes.
         *
         * @return std::vector de cv::Keypoint que contem os pontos caracteristicos detectados
         */
        std::vector<cv::KeyPoint> get_keyPoints ();
    };
}

#endif // SURF_H
