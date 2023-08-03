#pragma once

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle); 
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
#include <vtkSmartPointer.h>
#include <vtkXMLImageDataReader.h>
#include <vtkImageData.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkImageStencil.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkClipPolyData.h>
#include <vtkProperty.h>
#include <vtkPlane.h>
#include <vtkRenderWindow.h>
#include <vtkSTLReader.h>
#include <vtkSphere.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageMarchingCubes.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkImplicitDataSet.h>
#include <vtkImageDataToPointSet.h>
#include <vtkXMLImageDataReader.h>
#include <vtkClipVolume.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkMetaImageWriter.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkXMLImageDataReader.h>
#include <vtkImageData.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkImageStencil.h>
#include <vtkAutoInit.h>

class clip 
{
public:
	void show(vtkNew<vtkImageData> image1, vtkNew<vtkImageData>image2);
	vtkNew<vtkImageData> clip_trans(vtkNew<vtkImageData> imageDataB, double vector[3]);
	vtkNew<vtkImageData> clip_trans(vtkNew<vtkImageData>imageDataA, vtkNew<vtkImageData> imageDataB, double vector[3]);


};