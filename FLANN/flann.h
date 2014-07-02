#ifndef FLANN_H
#define FLANN_H

#include <vector>

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
        /* Utilizado para encontrar pares de keypoints que batem, *
         * de dois frames diferentes.                             */
        cv::FlannBasedMatcher matcher;
        /* Guarda todos os casamentos de pontos aceitos pelo combinador (matcher). */
        std::vector <cv::DMatch> matches, good_matches;

        /* Menor e maior distancia dos casamentos, usados para definir os bons casamentos *
         * dentre todos os casamentos detectados                                          */
        double matches_minDist, matches_maxDist;

        /* limiar de distancia definido pelo usuario, substituindo matches_minDist para o *
         * Calculo dos bons casamentos                                                    */
        float distanceThreshold;
        /* Determina se sera ou nao, usado o limiar de distancia definido pelo usuario, *
         * ou a implementacao automatizada                                              */
        bool use_distanceThreshold;

        /***********  Funcoes ***********/

        /**
         * @brief minMaxDist    Obtem a menor a maior distancia de todos os casamentos
         *                      de pontos encontrados pelo matcher
         */
        void minMaxDist ();

        /**
         * @brief findGoodMatches   Encontra os bons casamentos se baseando na menor distancia
         *                          encontrada entre os casamentos, ou utilizando um limiar
         *                          definido pelo usuario.
         */
        void findGoodMatches ();

    public:
        FLANN ();

        /**
         * @brief find_matches  Encontra os casamentos dos pontos passados como parametro,
         *                      e os grava em matches e good_maches.
         *
         * @param eye           Estrutura que representa os descritores e pontos
         *                      caracteristicos de uma imagem, no caso, dos olhos (template).
         *
         * @param roi           Estrutura que representa os descritores e pontos
         *                      caracteristicos de outra imagem, no caso, do proximo frame,
         *                      onde sera procurado os olhos dentro.
         */
        void find_matches (eyeT::SURF& eye, eyeT::SURF& roi);

        /**
         * @brief find_matches  Encontra os casamentos dos pontos passados como parametro,
         *                      os grava em matches e good_maches, e gera uma imagem que
         *                      msotra graficamente, as duas imagens e seus casamentos (bons).
         *
         * @param eye           Estrutura que representa os descritores e pontos
         *                      caracteristicos de uma imagem, no caso, dos olhos (template).
         *
         * @param roi           Estrutura que representa os descritores e pontos
         *                      caracteristicos de outra imagem, no caso, do proximo frame,
         *                      onde sera procurado os olhos dentro.
         *
         * @param frameEYE      Imagem do template
         *
         * @param frameROI      Imagem do frame completo (proximo frame).
         *
         * @return imagem que mostra graficamente, as duas imagens e seus casamentos (bons).
         */
        cv::Mat find_matches (eyeT::SURF& eye, eyeT::SURF& roi,
                              cv::Mat& frameEYE, cv::Mat& frameROI);

        /**
         * @brief get_minDist   Se obtem a menos distancia encontrada entre os pares definidos.
         *
         * @return Menor distancia entre os pares.
         */
        double get_minDist ();

        /**
         * @brief get_maxDist   Se obtem a maior distancia encontrada entre os pares definidos.
         *
         * @return Maior distancia entre os pares.
         */
        double get_maxDist ();

        /**
         * @brief get_numOf_goodMatches     Retorna o numero de bons pares encontrados.
         *
         * @return Numero de bons pares encontrados.
         */
        int get_numOf_goodMatches ();

        /**
         * @brief get_numOf_matches     Retorna o numero de pares encontrados.
         *
         * @return Numero de pares encontrados.
         */
        int get_numOf_matches ();

        /**
         * @brief set_use_DistanceThreshold     Habilita o uso do limiar de distancia
         *                                      definido externamente.
         *
         */
        void set_use_DistanceThreshold ();

        /**
         * @brief set_DistanceThreshold     Determinar um novo limiar de distancia usado para
         *                                  definir qual par de pontos caracteristicos pode ser
         *                                  dito como um bom par.
         *
         *
         * @param __distance                Novo limiar de distancia.
         */
        void set_DistanceThreshold (float __distance);

        /**
         * @brief unSet_use_DistanceThreshold   Desabilita o uso do limiar de distancia
         *                                      definido externamente.
         */
        void unSet_use_DistanceThreshold ();

        /**
         * @brief using_distanceThreshold   Informa se esta sendo usado um limiar de distancia
         *                                  definido externamente ou nao.
         *
         * @return true, caso esteja sendo usado um limiar externo,
         *         falso, caso esteja sendo usado um limar determinado por um calculo que leva em
         *         consideracao a menor distancia entre os pares achados, definido de forma interna.
         */
        bool using_distanceThreshold();
    };
}

#endif // FLANN_H
