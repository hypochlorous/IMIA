//
//#include <iostream>
//
//#include "vtkMain.h"
//#include <vtkNrrdReader.h>
//#include <vtkImageReslice.h>
//#include <vtkTransform.h>
//#include <vtkFixedPointVolumeRayCastMapper.h>
//
//
//
//
//
//int main()
//{
//    vtkNew<vtkXMLImageDataReader>bone_read;
//    vtkNew<vtkXMLImageDataReader>ireader;
//    bone_read->SetFileName("E:\\PM\\SurgPlan\\Data\\seg_bone.vti");
//    bone_read->Update();
//    ireader->SetFileName("E:\\PM\\SurgPlan\\Data\\201.vti");
//    ireader->Update();
//    bone_read->GetOutput()->SetOrigin(0, 0, 0);
//    ireader->GetOutput()->SetOrigin(0, 0, 0);
//    
//
//    vtkNew<vtkNrrdReader>nread1,nread2;
//    nread1->SetFileName("E:\\PM\\SurgPlan\\Data\\jiati01.nrrd");
//    
//    nread2->SetFileName("E:\\PM\\SurgPlan\\Data\\jinggu.nrrd");
//    
//    nread1->GetOutput()->SetOrigin(0, 0, 0);
//    nread2->GetOutput()->SetOrigin(0, 0, 0);
//    nread1->Update();
//    nread2->Update();
//    mywindow window;
//
//
//    vtkTransform* tranForm = vtkTransform::New();//通过vtkTransForm进行坐标变换操作，当然大佬们可以自己算坐标变换矩阵。
//    tranForm->RotateZ(90);
//    tranForm->Translate(0, 0, -3);
//
//    vtkImageReslice* changes = vtkImageReslice::New();
//    changes->SetInputData(ireader->GetOutput());//设置vtkimageData类型的数据
//    //changes->SetResliceAxes(axis);//这个是用坐标旋转矩阵的
//    changes->SetInformationInput(ireader->GetOutput());
//    changes->SetResliceTransform(tranForm);//通过此函数调用坐标变换
//
//    changes->SetInterpolationModeToLinear();
//    //changes->SetOutputDimensionality(3);
//    changes->Update();
//
//
//    //window.show_imagedatas(bone_read->GetOutput(),ireader->GetOutput());
//
//    vtkNew<vtkFixedPointVolumeRayCastMapper>mp;
//    mp->SetInputData(changes->GetOutput());
//
//    vtkNew<vtkVolume>volume;
//    volume->SetMapper(mp);
//
//    vtkNew<vtkRenderer>render;
//    render->AddVolume(volume);
//    render->SetBackground(0, 1, 1);
//
//    vtkSmartPointer<vtkRenderWindowInteractor> rwi =
//        vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    
//    vtkNew<vtkRenderWindow>rw;
//    rw->AddRenderer(render);
//    rwi->SetRenderWindow(rw);
//
//    rw->Render();
//    
//    rwi->Start();
//    //vtkNew<vtkImageData>imagedata;
//    //int dimA[3], dimB[3];
//}
//
//

//在每个例子下面需要添加，这一段宏阻塞，否则加载不出图像
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle); ///
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
#include "vtkDICOMImageReader.h"

#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkImageMapToColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkCutter.h"
#include "vtkPlane.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageCast.h"
#include "vtkPointData.h"
#include "vtkImageShiftScale.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkImageStencil.h"
#include "vtkLookupTable.h"
#include "vtkImagePlaneWidget.h"
#include "vtkCellArray.h"
#include "vtkType.h"
#include "vtkPlaneWidget.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkProbeFilter.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkClipPolyData.h"
#include <vtkXMLImageDataReader.h>
class vtkWidgetCall : public vtkCommand
{
public:


    static vtkWidgetCall* New()
    {
        return new vtkWidgetCall;
    }
public:
    virtual void Execute(vtkObject* caller, unsigned long eventId, void* callData)
    {
        vtkPlaneWidget* pWidget = vtkPlaneWidget::SafeDownCast(caller);

        pWidget->GetPlane(pPlane);

    }
public:

    vtkPlane* pPlane;

};
void main()
{

    vtkRenderer* pRender1 = vtkRenderer::New();
    vtkRenderer* pRender2 = vtkRenderer::New();
    vtkRenderWindow* pWin = vtkRenderWindow::New();
    vtkRenderWindowInteractor* pInt = vtkRenderWindowInteractor::New();
    pWin->SetSize(600, 600);
    pWin->AddRenderer(pRender1);
    pWin->AddRenderer(pRender2);
    pRender1->SetViewport(0, 0, .5, 1);
    pRender2->SetViewport(0.5, 0, 1, 1);
    pRender2->SetBackground(.3, .3, .3);
    pInt->SetRenderWindow(pWin);


    vtkNew<vtkXMLImageDataReader> pReader;
    pReader->SetFileName("E:\\PM\\SurgPlan\\Data\\seg_bone.vti");
    pReader->Update(); //读取

    double range[2];
    pReader->GetOutput()->GetScalarRange(range);
    vtkImageShiftScale* m_pShift = vtkImageShiftScale::New();
    m_pShift->SetShift(-1.0 * range[0]);
    m_pShift->SetScale(255.0 / (range[1] - range[0]));
    m_pShift->SetOutputScalarTypeToUnsignedChar();
    m_pShift->SetInputConnection(pReader->GetOutputPort());
    m_pShift->ReleaseDataFlagOff();
    m_pShift->Update();

    vtkFixedPointVolumeRayCastMapper* pMapper = vtkFixedPointVolumeRayCastMapper::New();
    pMapper->SetInputConnection(pReader->GetOutputPort()); //映射:第一步

    vtkColorTransferFunction* pColor = vtkColorTransferFunction::New(); //第二步:color
    pColor->AddRGBSegment(0, 1, 1, 1, 255, 1, 1, 1);

    vtkPiecewiseFunction* pPiceFun = vtkPiecewiseFunction::New();
    pPiceFun->AddSegment(0, 0, 3000, 1);//第三步: vtkPiecwise分段

    pMapper->SetBlendModeToMaximumIntensity();

    vtkVolumeProperty* pProperty = vtkVolumeProperty::New();//设置属性
    pProperty->SetColor(pColor);

    pProperty->SetScalarOpacity(pPiceFun);
    pProperty->SetInterpolationTypeToLinear();
    pProperty->ShadeOff();

    vtkVolume* pVolume = vtkVolume::New();
    pVolume->SetProperty(pProperty);
    pVolume->SetMapper(pMapper); //生成体对象

    pRender1->AddVolume(pVolume); //增加体数据到渲染器中渲染
    pRender1->SetBackground(.3, .4, .5);

    vtkPlaneWidget* pWidget = vtkPlaneWidget::New();
    pWidget->SetInteractor(pInt);
    pWidget->SetInputData(m_pShift->GetOutput());
    pWidget->SetResolution(10);
    pWidget->GetPlaneProperty()->SetColor(.9, .4, .4);
    pWidget->GetHandleProperty()->SetColor(0, .4, .7);
    pWidget->GetHandleProperty()->SetLineWidth(1.5);
    pWidget->NormalToYAxisOn();
    pWidget->SetRepresentationToSurface();
    pWidget->SetPlaceFactor(1.0);
    pWidget->SetCenter(m_pShift->GetOutput()->GetCenter());
    pWidget->PlaceWidget();

    pWidget->On();


    vtkPlane* pPlane = vtkPlane::New();
    pPlane->SetOrigin(pReader->GetOutput()->GetCenter());
    pPlane->SetNormal(1, 0, 0);
    vtkCutter* pCut = vtkCutter::New();
    pCut->SetCutFunction(pPlane);
    pCut->SetInputConnection(m_pShift->GetOutputPort());
    pCut->Update();


    vtkPolyDataMapper* selectMapper = vtkPolyDataMapper::New();
    selectMapper->SetInputConnection(pCut->GetOutputPort());

    vtkActor* pActor = vtkActor::New();
    pActor->SetMapper(selectMapper);
    pRender2->AddActor(pActor);


    vtkWidgetCall* pCall = vtkWidgetCall::New();
    pCall->pPlane = pPlane;
    pWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);

    pInt->Initialize();
    pInt->Start();
}