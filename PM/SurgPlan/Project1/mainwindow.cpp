
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boxinteractor.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <QFileDialog.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include "interactor.h"
#include "line.h"
#include <vtkAssembly.h>
#include <vtkAxesActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkTransform.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolygon.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkPointPicker.h>
#include <vtkXMLImageDataReader.h>
#include <vtkMarchingCubes.h>
#include <qdebug.h>

double MainWindow::vector[3] = { 66,0,0 };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //实例化对象
    boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renWin->AddRenderer(renderer);
    iren = vtkSmartPointer<QVTKInteractor>::New();
    iren->SetRenderWindow(renWin);
    
    //读Imagedata文件
    vtkNew<vtkXMLImageDataReader>iread;
    iread->SetFileName("E:\\PM\\SurgPlan\\Data\\seg_bone.vti");
    iread->Update();
    iread->GetOutput()->SetOrigin(0, 0, 0);
    image_bone->DeepCopy(iread->GetOutput());

    vtkNew<vtkSTLReader>sread;
    sread->SetFileName("E:\\PM\\SurgPlan\\Data\\201_final.stl");
    sread->Update();
    
    vtkNew<vtkPolyDataMapper>smapper;
    smapper->SetInputData(sread->GetOutput());
    prosthesis=vtkSmartPointer<vtkActor>::New();
    prosthesis->SetMapper(smapper);

    //面渲染
    vtkNew<vtkMarchingCubes> MC;
    MC->SetInputConnection(iread->GetOutputPort());
    MC->SetValue(0, 250); 
    MC->Update();
    boneMapper = vtkSmartPointer< vtkPolyDataMapper >::New();
    boneMapper->SetInputData(MC->GetOutput());
    boneMapper->ScalarVisibilityOff();
    bone = vtkSmartPointer< vtkActor >::New();
    bone->SetPosition(0,0,0);
    bone->SetMapper(boneMapper);
   

    vtkNew<vtkAxesActor> Axes ;
    Axes->SetPosition(0, 0, 0);
    Axes->SetTotalLength(100,100,100);
    Axes->SetXAxisLabelText("x");
    Axes->SetYAxisLabelText("y");
    Axes->SetZAxisLabelText("z");


    //Add the volume to the scene
    renderer->AddActor(bone);
    renderer->AddActor(prosthesis);
    renderer->AddActor(Axes);
    renderer->GetActiveCamera()->SetViewUp(0,0,1);

    renderer->GetActiveCamera()->SetPosition(400,0,0);
    vtkNew<StyleTrackballActor>style;
    style->SetDefaultRenderer(renderer);
    style->ActorB = bone;

    ui->widget_1->setRenderWindow(renWin);
    //iren->SetInteractorStyle(style);
    iren->Initialize();
    //iren->Start();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString selectFilePath = QFileDialog::getOpenFileName(this, QString("选择STL文件"), QString(""), QString("文件(*.stl)"));
    open_stl(selectFilePath.toUtf8()) ;
}

void MainWindow::open_stl(const char* f)
{
    if (!stlreader)
    {
        stlreader = vtkSmartPointer<vtkSTLReader>::New();
    }

    stlreader->SetFileName(f);                               //这里主要，是文件夹哈，不是文件名
    stlreader->Update();

    //添加映射器
    vtkSmartPointer<vtkPolyDataMapper> stlMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    stlMapper->SetInputConnection(stlreader->GetOutputPort());
    //添加演员
    vtkSmartPointer<vtkActor> stlActor = vtkSmartPointer<vtkActor>::New();
    stlActor->SetMapper(stlMapper);
    double center1[3];
    std::copy(stlActor->GetCenter(), stlActor->GetCenter() + 3, center1);
    std::cout << center1[0] << "," << center1[1] << "," << center1[2] << std::endl;
    iren->SetRenderWindow(renWin);
    renderer->AddActor(stlActor);
    stlActor->SetPosition(0, 0, 0);
    vtkSmartPointer<StyleTrackballActor> style = vtkSmartPointer <StyleTrackballActor>::New();
    //style->SetInteractor(stlActor);
    style->ActorA = stlActor;
    iren->SetInteractorStyle(style);
    //iren->SetInteractorStyle(nullptr);
    renWin->Render();
    iren->Initialize();
    //iren->Start();
}

void MainWindow::on_pushButton_2_clicked()
{
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    iren->SetInteractorStyle(style);
}


void MainWindow::on_pushButton_3_clicked()
{
    vtkNew<vtkInteractorStyleTrackballActor> style ;
    iren->SetInteractorStyle(style);
}


void MainWindow::on_pushButton_4_clicked()
{
    vtkActor* actor ;
    vtkActorCollection* actorCollection = renderer->GetActors();
    int num = actorCollection->GetNumberOfItems();
    //这个函数比较重要，否则getNextActor将没法得到正确的actor
    actorCollection->InitTraversal();
    for (int i=0;i<num;++i)
    {
        //移除栈顶actor
         actor = actorCollection->GetNextActor();
    }
    renderer->RemoveActor(actor);
    renWin->Render();
}


void MainWindow::on_pushButton_5_clicked()
{
    // 创建四个点（梯形的四个顶点）
    vtkNew<vtkPoints> points ;
    points->InsertNextPoint(0.0, 0.0, 0.0);
    points->InsertNextPoint(20.0, 0.0, 0.0);
    points->InsertNextPoint(30, 20.0, 0.0);
    points->InsertNextPoint(-10.0, 20.0, 0.0);

    // 创建一个多边形并添加点
    vtkNew<vtkPolygon> polygon ;
    polygon->GetPointIds()->SetNumberOfIds(4);
    for (unsigned int i = 0; i < 4; i++)
    {
        polygon->GetPointIds()->SetId(i, i);
    }

    // 创建一个单元数组并添加多边形
    vtkNew<vtkCellArray> polygons ;
    polygons->InsertNextCell(polygon);

    // 创建一个PolyData并添加点和多边形
    trapeziumpolydata = vtkSmartPointer<vtkPolyData>::New();
    trapeziumpolydata->SetPoints(points);
    trapeziumpolydata->SetPolys(polygons);

    // 创建映射器和actor
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(trapeziumpolydata);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->SetPosition(100,100,100);


    // 创建渲染器、渲染窗口和交互器

    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor ;


    // 创建交互样式
    vtkNew<vtkInteractorStyleRubberBandPick> style;
    renderWindowInteractor->SetInteractorStyle(style);

    // 添加actor到渲染器，然后刷新渲染 启动循环
    renderer->AddActor(actor);


    renWin->Render();
    iren->Initialize();
}

//切割函数
void MainWindow::on_pushButton_6_clicked()
{
    vtkNew<vtkImageData>imageB,clip_res;
    vtkNew<vtkXMLImageDataReader>iread;
    iread->SetFileName("E:\\PM\\SurgPlan\\Data\\201.vti");
    iread->Update();
    imageB->DeepCopy(iread->GetOutput());

    this->prosthesis->GetPosition(vector);
    qDebug()<<vector[0]<<" "<<vector[1]<<" "<<vector[2];

    //还没有实现normalize,需要将Actor的位移扩大10/3对应Volume
    for (int i = 0; i < 3; ++i) {
        vector[i] *= 3.3333333;
    }
////    如果太小就缩放10倍
//    while ((abs(vector[0]) + abs(vector[1]) + abs(vector[2])) < 10) {
//        for (int i = 0; i < 3; ++i) {
//            vector[i] *= 10;
//        }
//        if (abs(vector[0]) + abs(vector[1]) + abs(vector[2]) == 0) {
//            break;
//        }
//    }
    clip_res=clip_func(image_bone,imageB,vector);
    show_images(image_bone, clip_res);

}

//picker
void MainWindow::on_pushButton_7_clicked()
{

    vtkNew<vtkPointPicker> pointPicker;


    iren->SetPicker(pointPicker);
    iren->SetRenderWindow(renWin);

    //vtkNew<PointPickerInteractorStyle> style;
    //iren->SetInteractorStyle(style);

    renWin->Render();
    iren->Initialize();

}

//包围box
void MainWindow::on_pushButton_8_clicked()
{   
    boxWidget->SetInteractor(iren);
    boxWidget->SetPlaceFactor(1.25); // 设置包围盒的大小
    boxWidget->SetProp3D(bone);
    boxWidget->PlaceWidget();
    boxWidget->On(); // 开启交互
    iren->Initialize();
}

//划线
void MainWindow::on_pushButton_9_clicked()
{
    vtkNew<customMouseInteractorStyle> style;
    iren->SetInteractorStyle(style);
    //在style中对vector赋值   
    //double* res;
    //res= style->GetVector();

    //iren->Initialize();
    //renWin->Render();

}

//测试
void MainWindow::on_pushButton_10_clicked()
{
    this->prosthesis->GetPosition(vector);
    qDebug() << vector[0] << " " << vector[1] << " " << vector[2];
}


vtkNew<vtkImageData> MainWindow::clip_func(vtkImageData* imageA, vtkImageData* imageB, double vector[3])
{
    int dimensions[3], dimensions2[3];
    
    //qDebug() << dimensions2[0] << " " << dimensions2[1] << " " << dimensions2[2];
    //double space[3];
    //imageB->GetSpacing(space);
    //qDebug() << space[0] << " " << space[1] << " " << space[2];

    imageA->GetDimensions(dimensions);
    imageB->GetDimensions(dimensions2);

    vtkNew<vtkImageData>result;
    result->DeepCopy(imageA);
    // 遍历每个数据点
    for (int z = 0; z < dimensions[2]; ++z)//遍历A,(x,y,z)在A中不会越界
    {
        for (int y = 0; y < dimensions[1]; ++y)
        {
            for (int x = 0; x < dimensions[0]; ++x)
            {
                double position[3];//当前点(x,y,z)变换前,在B中的位置position
                position[0] = x - vector[0];
                position[1] = y - vector[1];
                position[2] = z - vector[2];
                double value = imageA->GetScalarComponentAsDouble(x, y, z, 0); // 获取数据点的值
                if (value > 0) //说明该点在A中非空
                {
                    if (position[0] < dimensions2[0] && position[1] < dimensions2[1] && position[2] < dimensions2[2] && position[0]>0 && position[1]>0 && position[2]>0)//点(x,y,z)在变换前B不会越界;
                    {
                        double value2 = imageB->GetScalarComponentAsDouble(position[0], position[1], position[2], 0);
                        if (value2 > 0)//在B中非空
                        {
                            result->SetScalarComponentFromDouble(x, y, z, 0, 0); // 设置A中数据点的值为 0;result是A的副本
                        }
                    }
                }
            }
        }
    }
    return result;
}


void MainWindow::show_images(vtkImageData* imageA, vtkImageData* imageB)
{
    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->SetInterpolationTypeToLinear(); //设置线性插值方式
    volumeProperty->ShadeOn();//开启阴影属性
    volumeProperty->SetAmbient(0.4);//设置环境温度
    volumeProperty->SetDiffuse(0.6);//设置漫反射系数
    volumeProperty->SetSpecular(0.2);//设置镜面反射系数
    //添加灰度不透明度属性
    vtkNew<vtkPiecewiseFunction> compositeOpacity;
    compositeOpacity->AddPoint(70, 0.0);
    compositeOpacity->AddPoint(90, 1);
    compositeOpacity->AddPoint(180, 1);
    volumeProperty->SetScalarOpacity(compositeOpacity);
    //添加梯度不同明度属性
    vtkNew<vtkPiecewiseFunction> gradientOpacity;
    gradientOpacity->AddPoint(10, 0.0);
    gradientOpacity->AddPoint(90, 0.5);
    gradientOpacity->AddPoint(100, 1.0);
    volumeProperty->SetGradientOpacity(gradientOpacity);
    //添加颜色传输
    vtkNew<vtkColorTransferFunction> color;
    color->AddRGBPoint(0, 0, 0, 0);
    color->AddRGBPoint(64, 1.0, 0.52, 0.3);
    color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
    color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
    volumeProperty->SetColor(color);


    /***********************************************************/
    imageA->SetOrigin(0, 0, 0);
    imageB->SetOrigin(0, 0, 0);

    vtkNew<vtkGPUVolumeRayCastMapper> leftMapper, rightMapper;
    leftMapper->SetInputData(imageA);
    rightMapper->SetInputData(imageB);
    /*****************************************************************/

    vtkNew<vtkRenderer>leftRender, rightRender;
    //渲染管道
    vtkNew<vtkVolume> leftVolume;
    leftVolume->SetMapper(leftMapper);
    leftVolume->SetProperty(volumeProperty);
    vtkNew<vtkVolume> leftVolume2;
    //leftVolume2->SetMapper(leftMapper2);
    leftVolume2->SetProperty(volumeProperty);

    vtkNew<vtkVolume> rightVolume;
    rightVolume->SetMapper(rightMapper);
    rightVolume->SetProperty(volumeProperty);

    double leftView[4] = { 0, 0, 0.5, 1 };
    double rightView[4] = { 0.5, 0, 1, 1 };


    vtkNew<vtkRenderWindow> rw;
    rw->AddRenderer(leftRender);
    rw->AddRenderer(rightRender);

    rw->SetWindowName("VolumeRenderWindows");
    rw->SetSize(1440, 960);


    vtkNew<vtkRenderWindowInteractor> rwi;
    rwi->SetRenderWindow(rw);

    //ui->widget_1->setRenderWindow(rw);
    //设置interactor style为actor
    vtkNew<vtkInteractorStyleTrackballActor>style;
    style->SetInteractor(rwi);


    //style->SetRenderer(leftRender);

    rwi->SetInteractorStyle(style);
    style->SetInteractor(rwi);


    vtkNew<vtkImageData>clip_result;

    leftVolume->SetPosition(0, 0, 0);
    leftVolume2->SetPosition(0, 0, 0);

    leftRender->AddVolume(leftVolume);
    leftRender->AddVolume(leftVolume2);
    //leftRender->AddVolume(rightVolume);
    leftRender->SetBackground(1, 1, 0);
    leftRender->SetViewport(leftView);


    rightRender->AddVolume(rightVolume);
    rightRender->SetBackground(0, 1, 0);
    rightRender->SetViewport(rightView);



    double position[3];

    leftVolume->GetPosition(position);
    rightVolume->SetPosition(position);

    leftRender->GetActiveCamera()->SetPosition(0, -1, 0);
    leftRender->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    leftRender->GetActiveCamera()->SetViewUp(0, 0, 1);
    leftRender->GetActiveCamera()->Azimuth(30);
    leftRender->GetActiveCamera()->Elevation(30);
    leftRender->ResetCamera();
    rightRender->SetActiveCamera(leftRender->GetActiveCamera());

    rw->Render();
    rwi->Start();
    return;
}

vtkNew<vtkImageData> MainWindow::normalize(vtkImageData* image)
{
    //处理imageA的spacing为[1,1,1]
    vtkNew<vtkImageData>image_nor;
    image_nor->DeepCopy(image);

    double newSpacing[3] = { 1.0, 1.0, 1.0 }; // 新的间距
    int extent[6]; // 旧的extent
    image->GetExtent(extent);

    int newDimension[3];
    newDimension[0] = (extent[1] - extent[0] + 1) * (image->GetSpacing()[0] / newSpacing[0]);
    newDimension[1] = (extent[3] - extent[2] + 1) * (image->GetSpacing()[1] / newSpacing[1]);
    newDimension[2] = (extent[5] - extent[4] + 1) * (image->GetSpacing()[2] / newSpacing[2]);
    
    
    image_nor->SetSpacing(newSpacing);
    image_nor->SetExtent(0, newDimension[0] - 1, 0, newDimension[1] - 1, 0, newDimension[2] - 1);
    //调用Modified()方法会标记对象为已修改，从而在下一次渲染时重新计算对象的数据,以确保与对象的状态匹配。
    image_nor->Modified();

    return image_nor;
}

