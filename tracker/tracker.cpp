#include "tracker.h"

eyeT::Tracker::Tracker() :
    eyesFound( false ), eyesPositionFound( false ), K( 150 )
{}

eyeT::Tracker::Tracker( cv::Mat& __eyesTemplate  ) :
    eyesFound( false ), eyesPositionFound( false ), K( 150 )
{
    this->setEyesTemplate( __eyesTemplate );
}

bool eyeT::Tracker::haveEyesInFrame( cv::Mat& frame )
{
    // Verificar se existe modelo de olhos ja atribuidos.
    if( this->eyesTemplate.empty() )
        return false;
    
    // Obter descritores do frame usando SURF
    this->frameDescriptor.find_descriptors( frame );
    
    // Comparar os descritores usando FLANN
    this->matcher.find_matches( this->eyesDescriptor, this->frameDescriptor);
    
    // Verificar o numero de bons pares de descritores (true, false)
    this->eyesFound = this->matcher.get_numOf_goodMatches() >= this->getK();
    return this->eyesFound ;
}

bool eyeT::Tracker::haveEyesInFrame( cv::Mat& frame, cv::Mat& resulImg )
{
    // Verificar se existe modelo de olhos ja atribuidos.
    if( this->eyesTemplate.empty() )
        return false;

    // Obter descritores do frame usando SURF
    this->frameDescriptor.find_descriptors( frame );

    // Comparar os descritores usando FLANN
    resulImg = this->matcher.find_matches( this->eyesDescriptor, this->frameDescriptor,
                                           this->eyesTemplate, frame );

    // Verificar o numero de bons pares de descritores (true, false)
    this->eyesFound = this->matcher.get_numOf_goodMatches() >= this->getK();
    return this->eyesFound ;
}

cv::RotatedRect eyeT::Tracker::getEyesPosition()
{
    // Verificar se jah foi processado o frame e achados os olhos
    if( !eyesFound )
        return cv::RotatedRect();
    
    // Obter matriz de homografia
    this->homographyFinder.find_homography( this->eyesDescriptor.get_keyPoints(),
                                            this->frameDescriptor.get_keyPoints(),
                                            this->matcher.get_goodMatches() );
    
    // Fazer a transformacao de perspectiva
    this->homographyFinder.perspective_transform( this->eyesTemplate.size );
    
    // Obter o retangulo rotacionado e retorna-lo
    return this->homographyFinder.get_rotated_rect();
}

bool eyeT::Tracker::drawEyesPositionLines( cv::Mat &imgResul, cv::Point2f &offset )
{
    if( !eyesPositionFound )
        return false;

    this->homographyFinder.draw_lines( imgResul, offset );
    return true;
}

//-> eyeTemplate deve ser guardado dentro da classe, e setado separadamente,
//-> quando for setado, deve-se obter sues descritores automaticamente.
void eyeT::Tracker::setEyesTemplate( cv::Mat& __eyesTemplate )
{
    this->eyesTemplate = __eyesTemplate.clone();
    this->eyesDescriptor.find_descriptors( this->eyesTemplate );
}

cv::Mat eyeT::Tracker::getEyesTemplate()
{
    return this->eyesTemplate;
}

void eyeT::Tracker::setK( int __K )
{
    this->K = __K;
}

int eyeT::Tracker::getK()
{
    return this->K;
}
