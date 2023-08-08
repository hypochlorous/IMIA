#ifndef INTERACTOR_H
#define INTERACTOR_H

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1


#include <vtkInteractorStyleTrackballActor.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>

class StyleTrackballActor :public vtkInteractorStyleTrackballActor
{
public:
    static StyleTrackballActor* New();
    //vtkTypeMacro(InteractorStyle, vtkInteractorStyleRubberBandPick);
    vtkTypeMacro(StyleTrackballActor, vtkInteractorStyleTrackballActor);

    virtual void OnLeftButtonDown() override
    {
        // Forward events
        //vtkInteractorStyleTrackballActor::OnLeftButtonDown();

        if (this->InteractionProp == this->ActorA)
        {
            std::cout << "Picked ActorA." << std::endl;
        }
        else if (this->InteractionProp == this->ActorB)
        {
            vtkInteractorStyleTrackballActor::OnLeftButtonDown();
        }
    }

    vtkActor* ActorA;
    vtkActor* ActorB;


};
vtkStandardNewMacro(StyleTrackballActor);



#endif // INTERACTOR_H
