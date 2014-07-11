#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/core/core.hpp>

#include "../SURF/surf.h"
#include "../FLANN/flann.h"
#include "../Homography/Homography.h"

namespace eyeT
{
    class Tracker
    {
    private:
        /** Informa se foi achado olhos no frame, usado no homographyFinder, que so pode ser
         *  Executado caso tenha achado olhos no frame */
        bool eyesFound;

        /** Limiar que define se foram, ou nao, achados os olhos no frame. Caso o numero armazenado em
         *  matcher.get_numOf_goodMatches() for maior que K, logo, existem os olhos no frame. */
        int K;

        /** Processa e guarda os descritores dos olhos (template). */
        eyeT::SURF eyeDescriptor;
        /** Processa e guarda os descritores do frame, onde deve-se procurar os olhos. */
        eyeT::SURF frameDescriptor;

        /** Comparador que vai ser usado para encontrar caracteristicas semelhantes entre
         *  o frame e o template do olhos, utilizando os descritores ja obtidos. */
        eyeT::FLANN matcher;

        /** Usado para determinar a posição exata dos olhos no frame e extrair a imagem dos olhos
         *  de dentro do frame, mesmo se eles estiverem inclinados. */
        eyeT::Homography homographyFinder;

    public:
        Tracker();

        /**
         * @brief haveEyesInFrame   Procura pelos olhos (eyeTemplate), dentro do frame.
         *
         * @param frame             Frame onde serah procurado o os olhos.
         *
         * @param eyeTemplate       Modelo de olhos que server como base para se procurar olhos
         *                          dentro do frame.
         *
         * @return true, caso se tenha achado olhos dentro do frame,
         *         false, caso contrario. Se admite que achou olhos dentro do frame, quando o numero
         *         de bons casamentos de pontos caracteristicos entre o frame
         *         e o eyeTemplate, eh maior que K.
         */
        bool haveEyesInFrame( cv::Mat frame, cv::Mat eyeTemplate );

        /**
         * @brief haveEyesInFrame   Procura pelos olhos (eyeTemplate), dentro do frame, e grava
         *                          o resultado da comparacao dos pontos caracteristicos do frame
         *                          e do eyeTemplate
         *
         * @param frame             Frame onde serah procurado o os olhos.
         *
         * @param eyeTemplate       Modelo de olhos que server como base para se procurar olhos
         *                          dentro do frame.
         *
         * @param resulImg          Usado para se gravar o resultado gerado pelo processamento
         *                          do comparador de pontos caracteristicos.
         *
         * @return true, caso se tenha achado olhos dentro do frame,
         *         false, caso contrario. Se admite que achou olhos dentro do frame, quando o numero
         *         de bons casamentos de pontos caracteristicos entre o frame
         *         e o eyeTemplate, eh maior que K.
         */
        bool haveEyesInFrame( cv::Mat frame, cv::Mat eyeTemplate, cv::Mat resulImg );
    };
}


#endif // TRACKER_H
