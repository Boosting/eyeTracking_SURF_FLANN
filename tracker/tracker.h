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
         *  Executado caso tenha achado olhos no frame. */
        bool eyesFound;
        
        /** Informa se foi determinaod a posicao dos olhos no frame, caso exitam. Usado
         *  na funcao drawEyesPositionLines(). */
        bool eyesPositionFound;

        /** Limiar que define se foram, ou nao, achados os olhos no frame. Caso o numero armazenado em
         *  matcher.get_numOf_goodMatches() for maior que K, logo, existem os olhos no frame. */
        int K;

        /** Processa e guarda os descritores dos olhos (template). */
        eyeT::SURF eyesDescriptor;
        /** Processa e guarda os descritores do frame, onde deve-se procurar os olhos. */
        eyeT::SURF frameDescriptor;

        /** Comparador que vai ser usado para encontrar caracteristicas semelhantes entre
         *  o frame e o template do olhos, utilizando os descritores ja obtidos. */
        eyeT::FLANN matcher;

        /** Usado para determinar a posição exata dos olhos no frame e extrair a imagem dos olhos
         *  de dentro do frame, mesmo se eles estiverem inclinados. */
        eyeT::Homography homographyFinder;

        /** Modelo de olhos que serve como base para se procurar olhos dentro do frame. */
        cv::Mat eyesTemplate;

    public:
        Tracker();

        /**
         * @brief Tracker           Construtor da classe que ja inicializa com um modelo dos olhos.
         *
         * @param __eyesTemplate    Modelo de olhos que sera utilizado para o processamento.
         */
        Tracker( cv::Mat& __eyesTemplate );

        /**
         * @brief haveEyesInFrame   Procura pelos olhos (eyeTemplate), dentro do frame.
         *
         * @param frame             Frame onde serah procurado o os olhos.
         *
         * @return true, caso se tenha achado olhos dentro do frame,
         *         false, caso contrario. Se admite que achou olhos dentro do frame, quando o numero
         *         de bons casamentos de pontos caracteristicos entre o frame
         *         e o eyeTemplate, eh maior que K.
         */
        bool haveEyesInFrame( cv::Mat& frame );

        /**
         * @brief haveEyesInFrame   Procura pelos olhos (eyeTemplate), dentro do frame, e grava
         *                          o resultado da comparacao dos pontos caracteristicos do frame
         *                          e do eyeTemplate
         *
         * @param frame             Frame onde serah procurado o os olhos.
         *
         * @param resulImg          Usado para se gravar o resultado gerado pelo processamento
         *                          do comparador de pontos caracteristicos.
         *
         * @return true, caso se tenha achado olhos dentro do frame,
         *         false, caso contrario. Se admite que achou olhos dentro do frame, quando o numero
         *         de bons casamentos de pontos caracteristicos entre o frame
         *         e o eyeTemplate, eh maior que K.
         */
        bool haveEyesInFrame( cv::Mat& frame, cv::Mat& resulImg );
        
        /**
         * @brief getEyesPosition   Determina a posicao dos olhos no frame já processado, e retorna
         *                          uma estrutura que especifica essa posicao.
         * 
         * @return Retangulo que pode estar rotacionado, que especifica a posicao 
         *         dos olhos dentro do frame.
         */
        cv::RotatedRect getEyesPosition();
        
        /**
         * @brief drawEyesPositionLines     Desenha linhas que delimitam a posicao dos olhos dentro
         *                                  do frame, processado previamente pelas funcoes
         *                                  haveEyesInFrame() e getEyesPosition().
         * 
         * @param imgResul                  Imagem onde sera gravado o desenho das linhas.
         * 
         * @param offset                    Deslocamento desejado para se fazer o desenho.
         * 
         * @return true, caso tenha desenhado as linhas como previsto, false, caso nao se tenha 
         *         desenhado as linhas, ou por nao ter feito o processamento necessario ainda, que
         *         consistem em procurar os olhos no frame e determina a posicao deles, ou por conta
         *         do imgResul passado como parametro nao ter ao menos o tamanho minimo necessario
         *         para se desenha as linhas.
         */
        bool drawEyesPositionLines( cv::Mat& imgResul, cv::Point2f& offset = cv::Point2f( 0,0 )  );
        
        /**
         * @brief getEyesInFrame    Retorna a imagem delimitada pela posicao dos olhos
         *                          obtidos dentro do frame ja previamente processados processado.
         * 
         * @param frame             Imagem onde foram achados os olhos e determinado a posicao dos mesmos.
         * 
         * @return A imagem dos olhos dentro do frame, caso tenha ocorrido tudo bem, ou cv::Mat(), se
         *         o processamento necessario para se obter a imagem dos olhos ainda nao foi feito 
         *         (haveEyesInFrame() e getEyesPosition()), ou se o tamanho frame passado como parametro 
         *         nao eh compativel com a posicao dos olhos, jah determinada.
         */
        cv::Mat getEyesInFrame( cv::Mat& frame );
        
        /**
         * @brief setK  Atrbui um novo valor de K, que eh um limiar que define se foram, ou nao, 
         *              achados os olhos no frame.
         * 
         * @param __K   Novo valor de K.
         */
        void setK( int __K );
        
        /**
         * @brief getK  Retorna o valor atual de K, que eh um limiar que define se foram, ou nao, 
         *              achados os olhos no frame.
         * 
         * @return Valor atual de K.
         */
        int getK();

        /**
         * @brief setEyesTemplate   Atribui um novo modelo de olhos para ser procurado nos frames.
         *                          Automaticamente, eh feito um processamento para se obter os
         *                          descritores do modelo atrbuido.
         *
         * @param __eyesTemplate    Novo modelo de olhos que sera atrbuido.
         */
        void setEyesTemplate( cv::Mat& __eyesTemplate );

        /**
         * @brief getEyesTemplate   Usado para se obter o modelo de olhos usado atualmente.
         *
         * @return cv::Mat representado o modleo de olhos que esta sendo utilizado.
         */
        cv::Mat getEyesTemplate();
    };
}


#endif // TRACKER_H
