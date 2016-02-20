#include "MyProjectDialogClone.h"

MyProjectDialogClone::MyProjectDialogClone( wxWindow* parent )
:
DialogClone( parent )
{

}

void MyProjectDialogClone::OnButtonClickGo( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}


void MyProjectDialogClone::GetValues(int gaps[9], int *angle, int *times)
{
    // Collect information about the textCtrl's
    gaps[0] = atoi(textCtrl32->GetValue());
    gaps[1] = atoi(textCtrl31->GetValue());
    gaps[2] = atoi(textCtrl21->GetValue());
    gaps[3] = atoi(textCtrl11->GetValue());
    gaps[4] = atoi(textCtrl12->GetValue());
    gaps[5] = atoi(textCtrl13->GetValue());
    gaps[6] = atoi(textCtrl23->GetValue());
    gaps[7] = atoi(textCtrl33->GetValue());
    gaps[8] = atoi(textCtrl22->GetValue());
    *angle = atoi(textCtrlAngle->GetValue());
    *times = atoi(textCtrlTimes->GetValue());

}
