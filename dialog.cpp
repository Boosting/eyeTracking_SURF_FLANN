/*
 * Referencias:
 * [1] - http://stackoverflow.com/questions/12452118/what-does-waitkey-30-mean-in-opencv
 */

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::Dialog ),
    foundEyes( false ), inputFPS( 0.0 )
{
    ui->setupUi( this );

    // Determinar a opcao de entrada como arquivo como pardrao
    this->ui->radioButton_arqVideo->setChecked( 1 );

    // Verificar se o computador tem Webcam
    this->cap.open( 0 );
    if( !this->cap.isOpened() )
        this->ui->radioButton_webcam->setEnabled( 0 );

    // Desativar layout de execucao
    this->ui->checkBox_drawnLineDescriptors->setEnabled( 0 );
    this->ui->pushButton_pausarContinuar->setEnabled( 0 );
    this->ui->pushButton_reiniciar->setEnabled( 0 );
    this->ui->label_goodMatchesNumber->setText( "0" );
    this->ui->label_executionStatus->setText( "Não inicializado" );
    this->ui->horizontalSlider_hessianTresholdEyeTemplate->setEnabled( 0 );
    this->ui->horizontalSlider_hessianTresholdFrame->setEnabled( 0 );
    this->ui->horizontalSlider_matcherDistanceTreshold->setEnabled( 0 );
    this->ui->checkBox_matcherDistanceTreshold->setEnabled( 0 );
    this->ui->horizontalSlider_KNumber->setEnabled( 0 );

    // Criar e atrelar timer para repeticao de deteccao
    this->timer = new QTimer(this);
    connect( this->timer, SIGNAL( timeout() ), this, SLOT( processFrame() ) );
}

Dialog::~Dialog()
{
    delete ui;
    this->timer->stop();
    this->timer->deleteLater();

    cv::destroyAllWindows();

    if( this->cap.isOpened() )
        this->cap.release();
    if( !this->inputFrame.empty() )
        this->inputFrame.release();
    if( !this->inputFrameGray.empty() )
        this->inputFrameGray.release();
    if( !this->resulFrame.empty() )
        this->resulFrame.release();
    if( !this->eyesTemplate.empty() )
        this->eyesTemplate.release();
    if( !this->eyesInFrame.empty() )
        this->eyesInFrame.release();
}

void Dialog::processFrame()
{
    // Obter frame
    if( !this->cap.read( this->inputFrame ) )
    {
        this->close();
        return;
    }

    // Redimensionar o novo frame
    cv::resize( this->inputFrame, this->inputFrame, this->newInputFrameSize );

    // Transforma-lo para escala de cinza
    cv::cvtColor( this->inputFrame, this->inputFrameGray, CV_BGR2GRAY );

    // Verificar se foram achados olhos na iteraco anterior
    if( !this->foundEyes )
    {
        if( this->detectEyes() )
        {
            cv::imshow( WINDOW_TITLE_EYES_TEMPLATE, this->eyesTemplate );
            this->tracker.setEyesTemplate( this->eyesTemplate );

            this->resulFrame = this->inputFrame.clone();
            cv::rectangle( this->resulFrame, this->eye_bb, cv::Scalar( 0, 255, 0 ) );
            cv::imshow( WINDOW_TITLE_RESUL_FRAME, this->resulFrame );

            this->ui->label_executionStatus->setText( "Rastreando os olhos" );
            this->foundEyes = true;
        }
        else
            cv::imshow( WINDOW_TITLE_RESUL_FRAME, this->inputFrame );
    }
    else
    {
        if( this->trackEyes() )
        {
            this->ui->label_goodMatchesNumber->setText( QString::number( this->tracker.get_numOf_goodMatches() ) );
            this->ui->label_keyPointsEyeTemplateNumber->setText( QString::number( this->tracker.get_eyeTemplate_numOf_keyPoints() ) );
            this->ui->label_keyPointsFrameNumber->setText( QString::number( this->tracker.get_frame_numOf_keyPoints() ) );

            cv::imshow( WINDOW_TITLE_RESUL_FRAME, this->resulFrame );

            //cv::imshow( WINDOW_TITLE_EYES_IN_FRAME, this->eyesInFrame );
        }
        else
        {
            cv::destroyWindow( WINDOW_TITLE_EYES_TEMPLATE );
            this->eyesTemplate.release();

            if( !this->eyesInFrame.empty() )
            {
                this->eyesInFrame.release();
                cv::destroyWindow( WINDOW_TITLE_EYES_IN_FRAME );
            }

            cv::imshow( WINDOW_TITLE_RESUL_FRAME, this->inputFrame );

            this->ui->label_goodMatchesNumber->setText( QString::number( 0 ) );
            this->ui->label_keyPointsEyeTemplateNumber->setText( QString::number( 0 ) );
            this->ui->label_keyPointsFrameNumber->setText( QString::number( 0 ) );
            this->ui->label_executionStatus->setText( "Detectando os olhos." );
            this->foundEyes = false;
        }
    }

    // OpenCV Gui update (GTK)
    cv::waitKey( 1 );
}

bool Dialog::trackEyes()
{
    if( this->ui->checkBox_drawnLineDescriptors->isChecked() )
    {
        if( !this->tracker.haveEyesInFrame( this->inputFrameGray, this->resulFrame ) )
            return false;

        this->tracker.getEyesPosition();

        this->tracker.drawEyesPositionLines( this->resulFrame, cv::Point2f( this->tracker.getEyesTemplate().cols, 0.0 ) );
    }
    else
    {
        if( !this->tracker.haveEyesInFrame( this->inputFrameGray ) )
            return false;
        this->resulFrame = this->inputFrame.clone();

        this->tracker.getEyesPosition();

        this->tracker.drawEyesPositionLines( this->resulFrame );
    }

    //this->eyesInFrame = this->tracker.getEyesInFrame( this->inputFrame );

    return true;
}

bool Dialog::detectEyes()
{
    return ( detectEye( this->inputFrameGray, this->eyesTemplate, this->eye_bb ) > 0 );
}

void Dialog::restartDetection()
{

}

void Dialog::on_pushButton_iniciar_clicked()
{
    // Obter modo de entrada
    if( this->ui->radioButton_arqVideo->isChecked() )
    {
        // Obter diretorio do arquivo.
        this->videoDir = this->ui->lineEdit_videoDir->text().toStdString();

        if( !this->cap.open( this->videoDir ) )
        {
            QMessageBox::critical( this, "Falha na captura", "Não foi possível abrir o arquivo" );
            return;
        }
    }
    else if ( this->ui->radioButton_webcam->isChecked() )
    {
        if( !this->cap.open( 0 ) )
        {
            QMessageBox::critical( this, "Falha na captura", "Não foi possível ativar a Webcam" );
            return;
        }
    }

    // Desativar layout de inicializacao
    this->ui->radioButton_arqVideo->setEnabled( 0 );
    this->ui->radioButton_webcam->setEnabled( 0 );
    this->ui->lineEdit_videoDir->setEnabled( 0 );
    this->ui->pushButton_iniciar->setEnabled( 0 );

    // Onter taxa de quadros por segundo do video de entrada.
    this->inputFPS = this->cap.get( CV_CAP_PROP_FPS );

    // Capturar 33 frames para estabilizar limunisidade captada pela camera.
    for( int i = 0; i < 33; i++ )
        this->cap >> this->inputFrame;

    // Obter um tamanaho do frame onde o maior eixo seja no maximo 800
    find_new_size( this->inputFrame, this->newInputFrameSize, 800 );

    // Ativar layout de execucao
    this->ui->checkBox_drawnLineDescriptors->setEnabled( 1 );
    this->ui->pushButton_pausarContinuar->setEnabled( 1 );
    this->ui->pushButton_reiniciar->setEnabled( 1 );
    this->ui->label_executionStatus->setText( "Detectando os olhos." );
    this->ui->horizontalSlider_hessianTresholdEyeTemplate->setEnabled( 1 );
    this->ui->horizontalSlider_hessianTresholdFrame->setEnabled( 1 );
    this->ui->horizontalSlider_matcherDistanceTreshold->setEnabled( 1 );
    this->ui->checkBox_matcherDistanceTreshold->setEnabled( 1 );
    this->ui->horizontalSlider_KNumber->setEnabled( 1 );

    cv::namedWindow( WINDOW_TITLE_RESUL_FRAME, cv::WINDOW_AUTOSIZE );

    // Ativar timer para repeticao, levando em consideracao o fps do video de entrada.
    //-> Trabalha com no maximo 33 frames por segundo.
    this->timer->start( 1000 / (int) ( ( this->inputFPS < 33.0 && this->inputFPS != 0.0 ) ?
                                        this->inputFPS : 33.0 ) );
}

void Dialog::on_horizontalSlider_hessianTresholdEyeTemplate_valueChanged( int value )
{
    this->tracker.set_eyesTemplate_minHessian( value );
    this->ui->label_hessianTresholdEyeTemplate_value->setText( QString::number( this->tracker.get_eyesTemplate_minHessian() ) );
}

void Dialog::on_horizontalSlider_hessianTresholdFrame_valueChanged(int value)
{
    this->tracker.set_frame_minHessian( value );
    this->ui->label_hessianTresholdFrame_value->setText( QString::number( this->tracker.get_frame_minHessian() ) );
}

void Dialog::on_horizontalSlider_matcherDistanceTreshold_valueChanged(int value)
{
    this->tracker.set_matcher_distanceTreshold( (float) value / 100.0 );
}

void Dialog::on_checkBox_matcherDistanceTreshold_clicked()
{
    if( this->ui->checkBox_matcherDistanceTreshold->isChecked() )
        this->tracker.matcher_useDistanceTreshold( true );
    else
        this->tracker.matcher_useDistanceTreshold( false );
}

void Dialog::on_horizontalSlider_KNumber_valueChanged(int value)
{
    this->tracker.setK( value );
    this->ui->label_KNumber_value->setText( QString::number( this->tracker.getK() ) );
}
