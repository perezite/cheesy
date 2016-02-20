#include "MyProjectDialogNew.h"
#include "MyProjectGUIFrame.h"

MyProjectDialogNew::MyProjectDialogNew( wxWindow* parent )
        :
        DialogNew( parent )
{
//    MessageBox(NULL, "bla", "bla", NULL);
}

void MyProjectDialogNew::OnCreate( wxCommandEvent& event )
{
    int x = atoi((m_textCtrl11->GetValue()).c_str());
    int y = atoi((m_textCtrl12->GetValue()).c_str());
    SetLevelSizes(x, y);
    EndModal(wxID_OK);
}
