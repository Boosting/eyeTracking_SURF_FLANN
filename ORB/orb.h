#ifndef ORB_H
#define ORB_H

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace eyeT {
    class ORB
    {
    private:
        /** Detector dos pontos caracteristicos do frame. */
        cv::OrbFeatureDetector detector;
        /** Extrator dos descritores do frame (obtido com base nos KeyPoints). */
        cv::OrbDescriptorExtractor extractor;
        /** Estrutra que guarda os pontos caracteristicos detectados. */
        std::vector< cv::KeyPoint > keyPnts;
        /** Estrutura no formato de imagem (matriz) que representa os descritores detectados. */
        cv::Mat descriptors;

    public:
        ORB();

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
        std::vector< cv::KeyPoint > get_keyPoints ();

        void set_newParams( int nfeatures = 500, float scaleFactor = 1.2f, int nlevels = 8, int edgeThreshold = 31,
                            int firstLevel = 0, int WTA_K=2, int scoreType=cv::ORB::HARRIS_SCORE, int patchSize=31 );
    };
}

#endif // ORB_H
