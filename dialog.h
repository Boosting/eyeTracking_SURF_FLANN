#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "tracker/tracker.h"  // Rastreador de objetos
#include "eyeDetector/CPTM.h" // Detector de olhos

#define WINDOW_TITLE_RESUL_FRAME    "Imagem de entrada processada - Eye Tracking"
#define WINDOW_TITLE_EYES_TEMPLATE  "Modelo de olhos - Eye Tracking"
#define WINDOW_TITLE_EYES_IN_FRAME  "Olhos no frame atual - Eye Tracking"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog( QWidget *parent = 0 );
    ~Dialog();

public slots:
    /**
     * @brief processFrame  Sera executada em todas as iteracoes, verificando se o ha um modelo de olhos
     *                      para ser rasterrado, com base nisso, eh decidio qual funcao sera executada,
     *                      trackEyes() ou detectEyes().
     */
    void processFrame();

private slots:
    /**
     * @brief trackEyes     Utiliza a classe Tracker para rastrear os olhos no frame
     *                      tendo um modelo como base.
     *
     * @return true, caso tenha achado olhos no frame,
     *         flase, caso contrario.
     */
    bool trackEyes();

    /**
     * @brief detectEyes    Procura os olhos no frame utilizando um padrao definido em um arquivo XML.
     *                      Primeiramente se procura um rosto, e dentro do primeiro rosto achado, se procura
     *                      os olhos. Eh necessario ter 2 arquivos XML descrevendo o padrao do rosto e olhos
     *                      em uma pasta chamada haar/, junto ao executavel.
     *
     * @return true, caso tenha detectado olhos no frame,
     *         false, caso contrario.
     */
    bool detectEyes();

    /**
     * @brief restartDetection   Reinicializa a deteccao, deletando o modelo de olhos atual
     *                           para se obter um novo.
     */
    void restartDetection();

    void on_pushButton_iniciar_clicked();

    void on_horizontalSlider_hessianTresholdEyeTemplate_valueChanged(int value);

    void on_horizontalSlider_hessianTresholdFrame_valueChanged(int value);

    void on_horizontalSlider_matcherDistanceTreshold_valueChanged(int value);

    void on_checkBox_matcherDistanceTreshold_clicked();

    void on_horizontalSlider_KNumber_valueChanged(int value);

private:
    Ui::Dialog *ui;

    /** Especifica se foi achado olhos no frame, seja pelo rastrreador ou detector. */
    bool foundEyes;

    /** Especifica a taxa de quadros por segundo do video de entrada. */
    double inputFPS;

    /** Temporizador que controla a velocidade de execucao do programa, podendo pausa-lo tambem. */
    QTimer* timer;

    /** Diretorio onde esta contido o video, caso se opte por usar um video como entrada. */
    std::string videoDir;

    /** Frame de entrada, podendo ser por video ou Webcam. */
    cv::Mat inputFrame;

    /** Frame de entrada em escala de cinza. */
    cv::Mat inputFrameGray;

    /** Resultado do processamento do frame de entrada, mostrando o contorno dos olhos achados, ou
        ate mesmo a relacao de casamentos de pontos caracteristicos do frame de entrada com o template.
        Esta imagem so sera apresentada se ohhso foram encontrados. */
    cv::Mat resulFrame;

    /** Modelo de olhos obtidos pela tecnica de processamento HAAR Cascades. */
    cv::Mat eyesTemplate;

    /** Imagem dos olhos retirados do frame atual, com base na posicao obtida pela
        transformacao de homografia. */
    cv::Mat eyesInFrame;

    /** Novo tamanho do frame de entrada */
    cv::Size newInputFrameSize;

    /** Utilizada para capturar o video da Webcam ou de um arquivo qualquer. */
    cv::VideoCapture cap;

    /** Rastreador usado para se obter a nova posicao dos olhos no frame */
    eyeT::Tracker tracker;

    /** Delimita a posicao dos olhos achada pelo detector */
    cv::Rect eye_bb;
};

#endif // DIALOG_H
