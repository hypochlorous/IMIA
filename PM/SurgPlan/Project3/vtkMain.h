#pragma once
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle); ///
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);


#include <vtkXMLImageDataReader.h>

#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkVolume.h>
#include <vtkRenderer.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <algorithm>

class mywindow {

public:
    mywindow() {
        // 初始化非静态成员变量
        leftRender = vtkSmartPointer<vtkRenderer>::New();
        rightRender = vtkSmartPointer<vtkRenderer>::New();
    }
	void GetLRenderer(vtkNew<vtkRenderer>ren) { leftRender = ren; }
	void GetRRenderer(vtkNew<vtkRenderer>ren) { rightRender = ren; }
public:
	vtkSmartPointer<vtkRenderer> leftRender;
	vtkSmartPointer<vtkRenderer> rightRender;

};

