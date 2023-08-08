#ifndef LINE_H
#define LINE_H

#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLineSource.h>


class customMouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static customMouseInteractorStyle* New();
    vtkTypeMacro(customMouseInteractorStyle, vtkInteractorStyleTrackballCamera);
    virtual void OnLeftButtonDown()
    {
        std::cout << "Pressed left mouse button." << std::endl;
        // Forward events

        clickPos = this->GetInteractor()->GetEventPosition();

        point1[0] = clickPos[0];
        point1[1] = clickPos[1];

        //从interactor得到的renderere而不是custom新定义一个;
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(point1[0], point1[1], 0);
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
        WorldPoint[0] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[0];
        WorldPoint[1] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[1];
        WorldPoint[2] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[2];

    }
    virtual void OnLeftButtonUp()
    {
        std::cout << "Pressed left mouse up." << std::endl;
        vtkIdType  pts[2]{ 0,1 };
        // Forward events
        //	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        //		int* clickPos = this->GetInteractor()->GetEventPosition();
        clickPos1 = this->GetInteractor()->GetEventPosition();
        point2[0] = clickPos1[0];
        point2[1] = clickPos1[1];


        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(point2[0], point2[1], 0);
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
        WorldPoint1[0] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[0];
        WorldPoint1[1] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[1];
        WorldPoint1[2] = (this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint())[2];

        for (int i = 0; i < 3; ++i) {
            (MainWindow::vector)[i] = WorldPoint1[i] - WorldPoint[i];
        }

        vtkNew<vtkLineSource> lineSource;
        lineSource->SetPoint1(WorldPoint);
        lineSource->SetPoint2(WorldPoint1);

        vtkNew<vtkPolyDataMapper> mapper ;
        mapper->SetInputConnection(lineSource->GetOutputPort());
        vtkNew<vtkActor> lineActor;
        lineActor->SetMapper(mapper);
        lineActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(lineActor);
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetRenderWindow()->Render();

    }

    virtual void OnMiddleButtonDown()
    {
        std::cout << "Pressed middle mouse button." << std::endl;
        // Forward events
        vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }

    virtual void OnRightButtonDown()
    {
        std::cout << "Pressed right mouse button." << std::endl;
        // Forward events
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
public:

    virtual double * GetVector() {
        double vector[3] = { 0,666,999 };
        for (int i = 0; i < 3; ++i) {
            //vector[i] = WorldPoint1[i] - WorldPoint[i];
        }
        return vector;
    }
    double point1[3]{ 0 };
    double point2[3]{ 0 };
    double WorldPoint[3]{ 0 };
    double WorldPoint1[3]{ 0 };
    int* clickPos;
    int* clickPos1;
};

vtkStandardNewMacro(customMouseInteractorStyle);

#endif // LINE_H
