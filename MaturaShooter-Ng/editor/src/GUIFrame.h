///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIFrame__
#define __GUIFrame__

#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/dialog.h>
#include <wx/button.h>

///////////////////////////////////////////////////////////////////////////

#define ToolDelete 1000
#define ToolEdit 1001

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel2;
		wxTreeCtrl* m_treeCtrl3;
		
		wxScrolledWindow* m_scrolledWindow21;
		wxStaticText* m_staticText111;
		wxStaticBitmap* m_bitmapBitmap;
		
		wxStaticText* m_staticText161;
		wxSpinCtrl* m_spinCtrlXCoord;
		wxStaticText* m_staticText1611;
		wxSpinCtrl* m_spinCtrlYCoord;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlEnergy;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_textCtrlAttack;
		wxStaticText* m_staticText12;
		wxTextCtrl* m_textCtrlLifetime;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_textCtrlVelocity;
		wxStaticText* m_staticText14;
		wxCheckBox* m_checkBoxCollDetection;
		wxStaticText* m_staticText15;
		wxChoice* m_choiceInitialState;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_textCtrlInitialAnimFrame;
		wxStaticText* m_staticText17;
		wxTextCtrl* m_textCtrlAnimLifetime;
		wxStaticText* m_staticText19;
		wxCheckBox* m_checkBoxAnimated;
		wxStaticText* m_staticText18;
		wxTextCtrl* m_textCtrlGfxFile;
		wxScrolledWindow* m_scrolledWindow2;
		wxMenuBar* m_menubar1;
		wxMenu* m_menuFile;
		wxMenu* m_menuEdit;
		wxMenu* m_menu4;
		wxMenu* m_menu5;
		wxMenu* m_menu3;
		wxStatusBar* m_statusBarMode;
		wxToolBar* m_toolBar1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnTreeCtrlItemActivated( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnSpinCtrlXCoord( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSpinCtrlYCoord( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextEnergy( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextAttack( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextLifetime( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextInitVelocity( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckBoxCollDetection( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnChoiceInitialState( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextInitialAnimFrame( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextAnimLifetime( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckBoxAnimated( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextGfxFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnScrWindowClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnScrWindowMotion( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnScrWindowPaint( wxPaintEvent& event ){ event.Skip(); }
		virtual void OnScrWindowRClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnMenuItemNew( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemOpen( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemSave( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemSaveAs( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemCopy( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemPaste( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemCut( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemSelectAll( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemCancelSelection( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemDeleteSelected( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemEditMode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemSelectionMode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemDeletionMode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemFrameMode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemShowObjects( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemShowTiles( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemClone( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemConfigGrid( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemShowGrid( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemDockOnGrid( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemMagnetize( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuItemAbout( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 517,764 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GUIFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogAbout
///////////////////////////////////////////////////////////////////////////////
class DialogAbout : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText12;
	
	public:
		DialogAbout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 295,343 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DialogAbout();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogNew
///////////////////////////////////////////////////////////////////////////////
class DialogNew : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText13;
		wxTextCtrl* m_textCtrl11;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_textCtrl12;
		wxButton* m_button6;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCreate( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		DialogNew( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create New Level"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 380,461 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DialogNew();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogClone
///////////////////////////////////////////////////////////////////////////////
class DialogClone : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* textCtrl11;
		wxTextCtrl* textCtrl21;
		wxTextCtrl* textCtrl31;
		wxTextCtrl* textCtrl12;
		wxTextCtrl* textCtrl22;
		wxTextCtrl* textCtrlAngle;
		wxTextCtrl* textCtrl32;
		wxTextCtrl* textCtrl13;
		wxTextCtrl* textCtrl23;
		wxTextCtrl* textCtrl33;
		wxStaticText* m_staticText15;
		wxTextCtrl* textCtrlTimes;
		wxButton* buttonGo;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClickGo( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		DialogClone( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Clone"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 533,518 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DialogClone();
	
};

#endif //__GUIFrame__
