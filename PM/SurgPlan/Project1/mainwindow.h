#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkImageData.h>
#include "boxinteractor.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <vtkSTLReader.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkAutoInit.h>
#include <vtkStripper.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin;
    vtkSmartPointer<QVTKInteractor> iren;
    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkBoxWidget> boxWidget;
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper;
    vtkSmartPointer<vtkVolume> volume;

    vtkSmartPointer<vtkActor> bone;
    vtkSmartPointer<vtkActor>prosthesis;

    double center[3];
    vtkSmartPointer<vtkPolyData> trapeziumpolydata;
    vtkSmartPointer<vtkStripper> boneStripper;
    vtkSmartPointer<vtkPolyDataMapper> boneMapper ;
    vtkNew<vtkImageData>image_bone;
    vtkNew<vtkImageData>imageA;
    //类中静态成员需要在类外进行初始化,否则会出现无法解析的外部符号(msvc),(还不能在头文件里初始化,需要在.cpp实现文件中;
    static double vector[3];
public:
    //输入被切割物体A(骨)切割物体B(假体),位移向量vector[3];
    vtkNew<vtkImageData>clip_func(vtkImageData* imageDataA, vtkImageData* imageDataB,double vector[3]);

    void show_images(vtkImageData* imageA, vtkImageData* imageB);

    vtkNew<vtkImageData>normalize(vtkImageData* image);

public slots:
    void open_stl(const char *f);


private slots:

    //添加STL文件;
    void on_pushButton_clicked();

    //切换交互为Camera
    void on_pushButton_2_clicked();

    //切换交互为Actor
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();
private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkSTLReader> stlreader = nullptr;

};


#endif // MAINWINDOW_H
