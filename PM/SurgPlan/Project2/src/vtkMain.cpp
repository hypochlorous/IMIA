
#include <iostream>

#include "vtkMain.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataCollection.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"
#include "vtkTriangleFilter.h"
#include "vtkXMLPolyDataReader.h"

#include "vtkContourWidget.h"
#include "vtkOrientedGlyphContourRepresentation.h"
#include "vtkPolygonalSurfacePointPlacer.h"
#include "vtkPolygonalSurfaceContourLineInterpolator.h"
#include <vtkSTLReader.h>



int main(int argc, char* argv[])
{
    vtkSmartPointer<vtkPolyData> polyData;
    if (argc < 2)
    {
        vtkNew<vtkSphereSource> sphereSource;
        sphereSource->SetThetaResolution(40);
        sphereSource->SetPhiResolution(20);
        sphereSource->Update();
    
        polyData = sphereSource->GetOutput();
    }
    else
    {
        vtkNew<vtkXMLPolyDataReader> reader ;
        reader->SetFileName(argv[1]);
        reader->Update();
        polyData = reader->GetOutput();
    }
    vtkNew<vtkSTLReader>sread;
    sread->SetFileName("E:\\PM\\SurgPlan\\Data\\p1_clean.stl");
    sread->Update();
    polyData = sread->GetOutput();

    // The Dijkistra interpolator will not accept cells that aren't triangles
    vtkNew<vtkTriangleFilter> triangleFilter;

    triangleFilter->SetInputData(polyData);
    triangleFilter->Update();

    vtkSmartPointer<vtkPolyData> pd = triangleFilter->GetOutput();

    //Create a mapper and actor
    vtkNew<vtkPolyDataMapper> mapper ;
    mapper->SetInputConnection(triangleFilter->GetOutputPort());

    vtkNew<vtkActor> actor ;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetInterpolationToFlat();

    // Create the render window, renderer and interactor.

    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(1920, 1440);
    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    // Add the actors to the renderer, set the background and size

    renderer->AddActor(actor);
    renderer->SetBackground(.3, .4, .5);

    // Here comes the contour widget stuff...

    vtkNew<vtkContourWidget> contourWidget;
    contourWidget->SetInteractor(interactor);
    vtkSmartPointer<vtkOrientedGlyphContourRepresentation> rep = vtkOrientedGlyphContourRepresentation::SafeDownCast(contourWidget->GetRepresentation());
    rep->GetLinesProperty()->SetColor(1, 0.2, 0);
    rep->GetLinesProperty()->SetLineWidth(3.0);

    vtkNew<vtkPolygonalSurfacePointPlacer> pointPlacer;
    pointPlacer->AddProp(actor);
    pointPlacer->GetPolys()->AddItem(pd);
    rep->SetPointPlacer(pointPlacer);

    vtkNew<vtkPolygonalSurfaceContourLineInterpolator> interpolator;
    interpolator->GetPolys()->AddItem(pd);
    rep->SetLineInterpolator(interpolator);

    renderWindow->Render();
    interactor->Initialize();

    contourWidget->EnabledOn();

    interactor->Start();

    return EXIT_SUCCESS;
}