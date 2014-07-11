#include "tracker.h"

eyeT::Tracker::Tracker() : eyesFound( false ), eyesPositionFound( false ), K( 150 )
{}

//-> eyeTemplate deve ser guardado dentro da classe, e setado separadamente,
//-> quando for setado, deve-se obter sues descritores automaticamente.
bool eyeT::Tracker::haveEyesInFrame( cv::Mat& frame, cv::Mat& eyeTemplate )
{
    // Verificar se o template foi processado
    
    // Obter descritores do frame e do template (caso ainda nao tenha sido obtido) usando SURF
    
    // Comparar os descritores usando FLANN
    
    // Verificar o numero de bons pares de descritores (true, false)
}

cv::RotatedRect eyeT::Tracker::getEyesPosition( cv::Size& eyesFrameSize )
{
    // Verificar se jah foi processado o frame e achados os olhos
    
    // Obter matriz de homografia
    
    // Fazer a transformacao de perspectiva
    
    // Obter o retangulo rotacionado e retorna-lo
}
