/***************************************************************
 * Name:      testApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2009-02-16
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "testApp.h"
#include "MyProjectGUIFrame.h"

IMPLEMENT_APP(testApp);

bool testApp::OnInit()
{
    MyProjectGUIFrame* frame = new MyProjectGUIFrame(0L);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();


    return true;
}
