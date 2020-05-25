

#include <iostream>
#include <cstdlib>

#include <vector>

// Librerías de OpenCV 4
#include <opencv2/opencv.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>

#include <opencv2/core/core.hpp>
#include <time.h>
using namespace std;
using namespace cv;

#define PI 3.14159
int valorSal = 0;
int valorPimienta = 0;
int kmediana = 3;
int kanteriorMediana = 3;

int kgausian = 3;
int kateriorGausian = 3;

void eventTrack(int v, void* p) {
    cout << "Valor " << v << endl;
}
Mat generarRuido(Mat gris) {
    Mat ruidoSal;
    ruidoSal = gris.clone();
    //porcentaje del ruido SAL
    
    double porcentaje = (double)valorSal/100;
    int ruido = ((int)(porcentaje * ((double)gris.rows * gris.cols)));

    //tiempo de la maquina 
    srand(time(NULL));
    //numeros aleatorios de pixeles para el ruido sal 
    int x, y = 0;
    for (int i = 0; i < ruido; i++) {
        y = rand() % ruidoSal.rows;
        x = rand() % ruidoSal.cols;
        ruidoSal.at<uchar>(y, x) = 255;
    }
    return ruidoSal;
}

void generarRuidoPimienta(Mat gris) {
    Mat ruidoSal;
    ruidoSal = gris.clone();
    //porcentaje del ruido SAL

    double porcentaje = (double)valorPimienta / 100;
    int ruido = ((int)(porcentaje * ((double)gris.rows * gris.cols)));

    //tiempo de la maquina 
    srand(time(NULL));
    //numeros aleatorios de pixeles para el ruido sal 
    int x, y = 0;
    for (int i = 0; i < ruido; i++) {
        y = rand() % ruidoSal.rows;
        x = rand() % ruidoSal.cols;
        ruidoSal.at<uchar>(y, x) = 0;
    }
    imshow("Ruido Pimienta", ruidoSal);
}

Mat  medianaF(Mat ruido) {

    Mat ruidoAtenuado;
    if (kmediana % 2 == 1) {
        medianBlur(ruido, ruidoAtenuado, kmediana);

        imshow("Ruido Atenuado Mediana", ruidoAtenuado);
        kanteriorMediana = kmediana;
    }
    else {
        kmediana = kanteriorMediana;
    }
    return ruidoAtenuado;
}

void gausiano(Mat ruido) {
    if (kgausian %2== 1) {
        Mat ruidoAtenuado;
        GaussianBlur(ruido, ruidoAtenuado, Size(kgausian,kgausian), 3);
        kanteriorMediana = kgausian;
        imshow("Ruido Atenuado Gausiano", ruidoAtenuado);
    }
    else {
        kgausian = kateriorGausian;
    }
    cout << "Kernel" << kgausian << endl;
    
}

void bordeCanny(Mat gris) {

    Mat suavizado;
    Mat canny;
    int k = 7;
    int minimo = 3;
    int ratio = 3;
    //GaussianBlur(gris,suavizado,Size(k,k),1);
    medianBlur(gris,suavizado,k);
    Canny(suavizado, canny, minimo, minimo * ratio, 3);
    imshow("Borde Canny", canny);
}

void bordeLaplacian(Mat gris) {
    Mat suavizado,laplace;
    int k = 15;
    //GaussianBlur(gris,suavizado,Size(k,k),1);
    medianBlur(gris, suavizado, k);
    Laplacian(suavizado,laplace,CV_8UC1,7);
    imshow("Laplace", laplace);
}
int main(int argc, char* argv[]) {
    // Documentación Oficial de OpenCV
    //https://docs.opencv.org/

    string nombreVideo = "D:/Grabaciones/Fortnite/2020.mp4";
    VideoCapture video(nombreVideo);

    if (video.isOpened()) {

        //namedWindow("Video", WINDOW_AUTOSIZE);
        namedWindow("Gris", WINDOW_AUTOSIZE);

        namedWindow("Ruido Sal", WINDOW_AUTOSIZE);
        namedWindow("Ruido Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Ruido Atenuado Mediana", WINDOW_AUTOSIZE);
        namedWindow("Ruido Atenuado Gausiano", WINDOW_AUTOSIZE);
        namedWindow("Borde Canny", WINDOW_AUTOSIZE);
        namedWindow("Laplace", WINDOW_AUTOSIZE);
        Mat frame;
        Mat imagenEcualizada;
        Mat gris,ruidoSal,mediana;
        createTrackbar("Sal ", "Gris", &valorSal, 100, eventTrack, NULL);
        createTrackbar("Pimienta ", "Gris", &valorPimienta, 100, eventTrack, NULL);
        createTrackbar("Mediana ", "Ruido Sal", &kmediana, 15, eventTrack, NULL);
        createTrackbar("Gaussiano ", "Ruido Sal", &kgausian, 15, eventTrack, NULL);
        while (3 == 3) {
            video >> frame;

            if (frame.rows > 0 && frame.cols > 0) {
                //opciones para escalar una imagen 
                 //resize(frame, frame, Size(),0.5,0.5);
                resize(frame, frame, Size(640, 480));
                cvtColor(frame, gris, COLOR_BGR2GRAY);

                ruidoSal=generarRuido(gris);
                generarRuidoPimienta(gris);
                mediana=medianaF(ruidoSal);
                gausiano(ruidoSal);

                bordeCanny(gris);
                bordeLaplacian(gris);

                imshow("Gris", gris);
                imshow("Ruido Sal", ruidoSal);
            }

            if (waitKey(23) == 27)
                break;
        }
    }
    //liberar recursos memoria 
    video.release();
   
    destroyAllWindows();

    return 0;
}
