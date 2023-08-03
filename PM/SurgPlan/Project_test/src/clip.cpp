#include "clip.h"



void show(vtkSmartPointer<vtkImageData> image, vtkSmartPointer<vtkImageData>image2)
{

    //设置体绘制相关属性
    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
        vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetInterpolationTypeToLinear(); //设置线性插值方式
    volumeProperty->ShadeOn();//开启阴影属性
    volumeProperty->SetAmbient(0.4);//设置环境温度
    volumeProperty->SetDiffuse(0.6);//设置漫反射系数
    volumeProperty->SetSpecular(0.2);//设置镜面反射系数
    //添加灰度不透明度属性
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
        vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(70, 0.0);
    compositeOpacity->AddPoint(90, 0.4);
    compositeOpacity->AddPoint(180, 0.6);
    volumeProperty->SetScalarOpacity(compositeOpacity);
    //添加梯度不同明度属性
    vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity =
        vtkSmartPointer<vtkPiecewiseFunction>::New();
    gradientOpacity->AddPoint(10, 0.0);
    gradientOpacity->AddPoint(90, 0.5);
    gradientOpacity->AddPoint(100, 1.0);
    volumeProperty->SetGradientOpacity(gradientOpacity);
    //添加颜色传输
    vtkSmartPointer<vtkColorTransferFunction> color =
        vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0, 0, 0, 0);
    color->AddRGBPoint(64, 1.0, 0.52, 0.3);
    color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
    color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
    volumeProperty->SetColor(color);
    /***********************************************************/

    vtkSmartPointer<vtkGPUVolumeRayCastMapper> leftMapper =
        vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    //leftMapper->SetInputConnection(reader->GetOutputPort());
    leftMapper->SetInputData(image);


    //vtkSmartPointer<vtkGPUVolumeRayCastMapper> origMapper =
    //    vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    //origMapper->SetInputData(reader->GetOutput());


    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =
        vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    //volumeMapper->SetInputData(reader2->GetOutput());
    volumeMapper->SetInputData(image2);
    /*****************************************************************/

    //渲染管道
    vtkSmartPointer<vtkVolume> leftVolume =
        vtkSmartPointer<vtkVolume>::New();
    leftVolume->SetMapper(leftMapper);
    leftVolume->SetProperty(volumeProperty);


    vtkSmartPointer<vtkVolume> croppingVolume =
        vtkSmartPointer<vtkVolume>::New();
    croppingVolume->SetMapper(volumeMapper);
    croppingVolume->SetProperty(volumeProperty);

    double leftView[4] = { 0, 0, 0.5, 1 };
    double croppingView[4] = { 0.5, 0, 1, 1 };
    vtkNew<vtkRenderer> leftRender;
    leftRender->AddVolume(leftVolume);
    leftRender->AddVolume(croppingVolume);
    leftRender->SetBackground(1, 1, 0);
    leftRender->SetViewport(leftView);


    vtkSmartPointer<vtkRenderer> croppingRender =
        vtkSmartPointer<vtkRenderer>::New();
    croppingRender->AddVolume(croppingVolume);
    croppingRender->SetBackground(0, 1, 0);
    croppingRender->SetViewport(croppingView);
    ///
    vtkSmartPointer<vtkRenderWindow> rw =
        vtkSmartPointer<vtkRenderWindow>::New();
    rw->AddRenderer(leftRender);
    rw->AddRenderer(croppingRender);
    rw->SetWindowName("VolumeRenderWindows");
    rw->SetSize(640, 320);


    vtkSmartPointer<vtkRenderWindowInteractor> rwi =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(rw);

    vtkNew<vtkInteractorStyleTrackballActor>style;
    style->SetInteractor(rwi);
    rwi->SetInteractorStyle(style);

    double position[3];
    leftVolume->GetPosition(position);
    croppingVolume->SetPosition(position);
    cout << position[0] << endl << position[1] << endl;


    leftRender->GetActiveCamera()->SetPosition(0, -1, 0);
    leftRender->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    leftRender->GetActiveCamera()->SetViewUp(0, 0, 1);
    leftRender->GetActiveCamera()->Azimuth(30);
    leftRender->GetActiveCamera()->Elevation(30);
    leftRender->ResetCamera();
    croppingRender->SetActiveCamera(leftRender->GetActiveCamera());

    rw->Render();
    rwi->Start();
    return;
}



//输入切割物体B(假体),B的位移向量;(默认待切割物体A数据为seg_bone.vti读取)
vtkNew<vtkImageData>clip_trans(vtkNew<vtkImageData> imageDataB, double vector[3])
{
    // 读取 VTK 的 vtkImageData 文件,并赋给A
    vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
    reader->SetFileName("E:\\PM\\SurgPlan\\Data\\seg_bone.vti");
    reader->Update();
    vtkSmartPointer<vtkImageData> imageDataA = reader->GetOutput();
    vtkNew<vtkImageData>result;//A的副本,不能用浅拷贝
    result->DeepCopy(imageDataA);
    // 获取数据维度
    int dimensions[3];
    imageDataA->GetDimensions(dimensions);
    //imageData->SetDimensions()
    int dimensions2[3];
    imageDataB->GetDimensions(dimensions2);
    // 遍历每个数据点
    for (int z = 0; z < dimensions[2]; ++z)//遍历A,(x,y,z)在A中不会越界
    {
        for (int y = 0; y < dimensions[1]; ++y)
        {
            for (int x = 0; x < dimensions[0]; ++x)
            {
                double position[3];//当前点(x,y,z)变换前在B的位置position
                position[0] = x - vector[0];
                position[1] = y - vector[1];
                position[2] = z - vector[2];
                double value = imageDataA->GetScalarComponentAsDouble(x, y, z, 0); // 获取数据点的值
                if (value > 0) //说明该点在A中非空
                {
                    if (position[0] < dimensions2[0] && position[1] < dimensions2[1] && position[2] < dimensions2[2] && position[0]>0 && position[1]>0 && position[2]>0)//点(x,y,z)在变换前B不会越界;
                    {
                        double value2 = imageDataB->GetScalarComponentAsDouble(position[0], position[1], position[2], 0);
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


//输入被切割物体A,切割物体B(假体),B的位移向量;
vtkNew<vtkImageData>clip_trans(vtkNew<vtkImageData>imageDataA, vtkNew<vtkImageData> imageDataB, double vector[3])
{
    // 获取 vtkImageData 对象
    vtkNew<vtkImageData>result;
    result->DeepCopy(imageDataA);
    // 获取数据维度(边界)
    int dimensions[3];
    imageDataA->GetDimensions(dimensions);
    //imageData->SetDimensions()
    int dimensions2[3];
    imageDataB->GetDimensions(dimensions2);
    // 遍历每个数据点
    for (int z = 0; z < dimensions[2]; ++z)//遍历A,(x,y,z)在A中不会越界
    {
        for (int y = 0; y < dimensions[1]; ++y)
        {
            for (int x = 0; x < dimensions[0]; ++x)
            {
                //int indexB = x + y * dimensions2[0] + z * dimensions2[0] * dimensions2[1];
                double position[3];//当前点(x,y,z)变换前在B的位置position
                position[0] = x - vector[0];
                position[1] = y - vector[1];
                position[2] = z - vector[2];
                double value = imageDataA->GetScalarComponentAsDouble(x, y, z, 0); // 获取数据点的值
                if (value > 0) //说明该点在A中非空
                {
                    if (position[0] < dimensions2[0] && position[1] < dimensions2[1] && position[2] < dimensions2[2] && position[0]>0 && position[1]>0 && position[2]>0)//点(x,y,z)在变换前B不会越界;
                    {
                        double value2 = imageDataB->GetScalarComponentAsDouble(position[0], position[1], position[2], 0);
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
