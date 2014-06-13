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
        cv::SurfFeatureDetector detector;
        std::vector <cv::KeyPoint> keyPnts;
        cv::SurfDescriptorExtractor extractor;
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

        void set_minHessian (double __minHessian);

        double get_minHessian ();

        cv::Mat get_descriptors_image ();

        int get_keyPoints_number ();

        std::vector<cv::KeyPoint> get_keyPoints ();
    };
}

#endif // SURF_H
