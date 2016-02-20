#ifndef __MyProjectDialogClone__
#define __MyProjectDialogClone__

/**
@file
Subclass of DialogClone, which is generated by wxFormBuilder.
*/

#include "GUIFrame.h"

/** Implementing DialogClone */
class MyProjectDialogClone : public DialogClone
{
protected:
	// Handlers for DialogClone events.
	void OnButtonClickGo( wxCommandEvent& event );

public:
	/** Constructor */
	MyProjectDialogClone( wxWindow* parent );
	/** GETters*/
	void GetValues(int gaps[9], int *angle, int *times);
};

#endif // __MyProjectDialogClone__
