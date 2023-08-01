#include "convert.h"

vtkSmartPointer<vtkImageData> Convert::PolyDataToImageData(vtkSmartPointer<vtkPolyData> polydata)
{
    //vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    vtkNew<vtkImageData>imageData;
    double bounds[6];
    polydata->GetBounds(bounds);
    double spacing[3];// desired volume spacing
    spacing[0] = 0.3;
    spacing[1] = 0.3;
    spacing[2] = 0.3;
    imageData->SetSpacing(spacing);

    //compute dimensions
    int dim[3];
    for (int i = 0; i < 3; ++i) {
        dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i]));
    }
    imageData->SetDimensions(dim);
    imageData->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);

    double origin[3];
    origin[0] = bounds[0] + spacing[0] / 2;
    origin[1] = bounds[2] + spacing[1] / 2;
    origin[2] = bounds[4] + spacing[2] / 2;
    imageData->SetOrigin(origin);

    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

    //fill the image with foreground voxels:
    unsigned char inval = 255;
    unsigned char outval = 0;
    vtkIdType count = imageData->GetNumberOfPoints();
    for (vtkIdType i = 0; i < count; ++i) {
        imageData->GetPointData()->GetScalars()->SetTuple1(i, inval);
    }

    //polygonal data --> image stencil:
    vtkSmartPointer<vtkPolyDataToImageStencil> pdtoImageStencil = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
    pdtoImageStencil->SetInputData(polydata);
    pdtoImageStencil->SetOutputOrigin(origin);
    pdtoImageStencil->SetOutputSpacing(spacing);
    pdtoImageStencil->SetOutputWholeExtent(imageData->GetExtent());
    pdtoImageStencil->Update();

    //cut the corresponding white image and set the background:
    vtkSmartPointer<vtkImageStencil> imageStencil = vtkSmartPointer<vtkImageStencil>::New();
    imageStencil->SetInputData(imageData);
    imageStencil->SetStencilConnection(pdtoImageStencil->GetOutputPort());
    imageStencil->ReverseStencilOff();
    imageStencil->SetBackgroundValue(outval);
    imageStencil->Update();

    imageData->DeepCopy(imageStencil->GetOutput());
    return imageData;
}

vtkSmartPointer<vtkPolyData>Convert::ImageDataToPolyData(vtkSmartPointer<vtkImageData> Imagedata)
{
    vtkSmartPointer<vtkImageMarchingCubes> imageMarchingCubes = vtkImageMarchingCubes::New();
    imageMarchingCubes->SetInputData(Imagedata);
    imageMarchingCubes->SetValue(0, (0 + 255) / 2);
    imageMarchingCubes->SetNumberOfContours(1);
    imageMarchingCubes->Update();
    vtkSmartPointer<vtkPolyDataMapper> polyDataMapper = vtkPolyDataMapper::New();
    polyDataMapper->SetInputData(imageMarchingCubes->GetOutput());
    return imageMarchingCubes->GetOutput();
}
void Convert::STL2VTI(std::string inputpath, std::string outputpath)
{

    vtkNew<vtkSTLReader>sreader;
    sreader->SetFileName(inputpath.c_str());
    sreader->Update();

    //vtkNew<vtkImageData>imagedata;
    //imagedata->DeepCopy(polyDataToImageData(sreader->GetOutput()));

    vtkNew<vtkXMLImageDataWriter>writer;
    writer->SetFileName(outputpath.c_str());
    writer->SetInputData(PolyDataToImageData(sreader->GetOutput()));
    writer->Update();
}
