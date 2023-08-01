#pragma once
#include <vtkImageData.h>

#include <vtkPolyData.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkImageMarchingCubes.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <string>
#include <vtkSTLReader.h>
#include <vtkXMLImageDataWriter.h>

class Convert {
public:
    vtkSmartPointer<vtkImageData> PolyDataToImageData(vtkSmartPointer<vtkPolyData> polydata) ;
    vtkSmartPointer<vtkPolyData>ImageDataToPolyData(vtkSmartPointer<vtkImageData> imagedata) ;

    //Read STL file and convert to imagedata type, and save as the .vti file
    void STL2VTI(std::string inputpath,std::string outputpath) ;
};

