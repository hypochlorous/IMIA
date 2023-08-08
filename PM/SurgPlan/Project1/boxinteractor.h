#ifndef BOXINTERACTOR_H
#define BOXINTERACTOR_H
#include <vtkBoxWidget.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkVolume.h>
#include <vtkMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageCast.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkNamedColors.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkImageData.h>
#include <vtkVolumeProperty.h>
#include <vtkCamera.h>
#include <vtkImageShiftScale.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkClipPolyData.h>
#include <vtkImplicitBoolean.h>
#include <vtkPlanes.h>
#include <vtkClipVolume.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkBoxRepresentation.h>
#include <vtkResampleToImage.h>
#include <vtkBoxWidget.h>
#include <vtkClipPolyData.h>
#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkPlanes.h>
#include <vtkCallbackCommand.h>
#include "vtkDICOMImageReader.h"

// 创建剪裁回调函数
class vtkBoxWidgetCallback : public vtkCommand
{
public:
    static vtkBoxWidgetCallback* New()
    {
        return new vtkBoxWidgetCallback;
    }

    virtual void Execute(vtkObject* caller, unsigned long, void*)
    {
        vtkBoxWidget* boxWidget = dynamic_cast<vtkBoxWidget*>(caller);
        if (boxWidget)
        {
            vtkPlanes* planes = vtkPlanes::New();
            boxWidget->GetPlanes(planes);
            vtkSmartPointer<vtkClipVolume> volumeClip = vtkSmartPointer<vtkClipVolume>::New();
            volumeClip->SetClipFunction(planes);
            volumeClip->SetInputData(this->Data);
            volumeClip->InsideOutOn();
            volumeClip->Update();
            vtkSmartPointer<vtkResampleToImage> resampleToImage = vtkSmartPointer<vtkResampleToImage>::New();
            resampleToImage->SetInputConnection(volumeClip->GetOutputPort());
            //resampleToImage->SetSamplingDimensions(128, 128, 128); // 设置输出图像的采样维度
            resampleToImage->Update();
            vtkSmartPointer<vtkImageData> outputImageData = resampleToImage->GetOutput();
            this->selectedMapper->SetInputData(outputImageData);
            this->renderer->GetRenderWindow()->Render();
        }
    }

    vtkSmartPointer<vtkImageData> Data;
    vtkSmartPointer<vtkVolumeMapper> selectedMapper;
    vtkSmartPointer<vtkVolume> selectedvolume;
    vtkSmartPointer<vtkRenderWindow> renderwindow;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkVolume> Volume;
};

#endif // BOXINTERACTOR_H
