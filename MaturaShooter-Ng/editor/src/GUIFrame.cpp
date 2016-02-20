///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_treeCtrl3 = new wxTreeCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxSize( 250,300 ), wxTR_DEFAULT_STYLE );
	m_treeCtrl3->SetMinSize( wxSize( 250,300 ) );
	
	bSizer19->Add( m_treeCtrl3, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer19, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 10, 0, 0, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_scrolledWindow21 = new wxScrolledWindow( m_panel2, wxID_ANY, wxPoint( -1,-1 ), wxSize( 250,300 ), wxHSCROLL|wxVSCROLL );
	m_scrolledWindow21->SetScrollRate( 5, 5 );
	m_scrolledWindow21->SetMinSize( wxSize( 250,300 ) );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText111 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Picture"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	bSizer17->Add( m_staticText111, 1, wxALL|wxEXPAND, 5 );
	
	m_bitmapBitmap = new wxStaticBitmap( m_scrolledWindow21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_bitmapBitmap, 1, wxALL|wxALIGN_RIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer6->Add( bSizer17, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer191->Add( 0, 10, 1, wxEXPAND, 5 );
	
	bSizer6->Add( bSizer191, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText161 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("X Coordinate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText161->Wrap( -1 );
	bSizer35->Add( m_staticText161, 1, wxALL|wxEXPAND, 5 );
	
	m_spinCtrlXCoord = new wxSpinCtrl( m_scrolledWindow21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer35->Add( m_spinCtrlXCoord, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer35, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer351;
	bSizer351 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1611 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("X Coordinate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1611->Wrap( -1 );
	bSizer351->Add( m_staticText1611, 1, wxALL|wxEXPAND, 5 );
	
	m_spinCtrlYCoord = new wxSpinCtrl( m_scrolledWindow21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer351->Add( m_spinCtrlYCoord, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer351, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Energy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer7->Add( m_staticText1, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlEnergy = new wxTextCtrl( m_scrolledWindow21, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_textCtrlEnergy, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText11 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Attack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer71->Add( m_staticText11, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlAttack = new wxTextCtrl( m_scrolledWindow21, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_textCtrlAttack, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer71, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer72;
	bSizer72 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Lifetime (ms)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer72->Add( m_staticText12, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlLifetime = new wxTextCtrl( m_scrolledWindow21, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer72->Add( m_textCtrlLifetime, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer72, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer73;
	bSizer73 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Init. Velocity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer73->Add( m_staticText13, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlVelocity = new wxTextCtrl( m_scrolledWindow21, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer73->Add( m_textCtrlVelocity, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer73, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer74;
	bSizer74 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText14 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Coll. Detection"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer74->Add( m_staticText14, 1, wxALL|wxEXPAND, 5 );
	
	m_checkBoxCollDetection = new wxCheckBox( m_scrolledWindow21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxCollDetection->SetValue(true);
	
	bSizer74->Add( m_checkBoxCollDetection, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer74, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer75;
	bSizer75 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText15 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Initial state"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer75->Add( m_staticText15, 1, wxALL|wxEXPAND, 5 );
	
	wxArrayString m_choiceInitialStateChoices;
	m_choiceInitialState = new wxChoice( m_scrolledWindow21, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceInitialStateChoices, 0 );
	m_choiceInitialState->SetSelection( 0 );
	bSizer75->Add( m_choiceInitialState, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer75, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer76;
	bSizer76 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Initial Anim. Frame"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer76->Add( m_staticText16, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlInitialAnimFrame = new wxTextCtrl( m_scrolledWindow21, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer76->Add( m_textCtrlInitialAnimFrame, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer76, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer77;
	bSizer77 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText17 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Anim. lifetime (ms)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer77->Add( m_staticText17, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlAnimLifetime = new wxTextCtrl( m_scrolledWindow21, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer77->Add( m_textCtrlAnimLifetime, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer77, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer79;
	bSizer79 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText19 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Animated"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer79->Add( m_staticText19, 1, wxALL|wxEXPAND, 5 );
	
	m_checkBoxAnimated = new wxCheckBox( m_scrolledWindow21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxAnimated->SetValue(true);
	
	bSizer79->Add( m_checkBoxAnimated, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer79, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer78;
	bSizer78 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText18 = new wxStaticText( m_scrolledWindow21, wxID_ANY, wxT("Gfx file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer78->Add( m_staticText18, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlGfxFile = new wxTextCtrl( m_scrolledWindow21, wxID_ANY, wxT("unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlGfxFile->Enable( false );
	
	bSizer78->Add( m_textCtrlGfxFile, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer78, 1, wxEXPAND, 5 );
	
	m_scrolledWindow21->SetSizer( bSizer6 );
	m_scrolledWindow21->Layout();
	bSizer20->Add( m_scrolledWindow21, 1, wxALL|wxEXPAND, 5 );
	
	fgSizer3->Add( bSizer20, 1, wxEXPAND, 5 );
	
	bSizer15->Add( fgSizer3, 0, 0, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_scrolledWindow2 = new wxScrolledWindow( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow2->SetScrollRate( 5, 5 );
	m_scrolledWindow2->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	bSizer18->Add( m_scrolledWindow2, 1, wxEXPAND | wxALL, 5 );
	
	bSizer15->Add( bSizer18, 1, wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer15 );
	m_panel2->Layout();
	bSizer15->Fit( m_panel2 );
	bSizer3->Add( m_panel2, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer3 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemNew;
	m_menuItemNew = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("&New") ) + wxT('\t') + wxT("Alt-N"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemNew );
	
	wxMenuItem* m_menuItemOpen;
	m_menuItemOpen = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("&Open") ) + wxT('\t') + wxT("Alt-O"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemOpen );
	
	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("&Save") ) + wxT('\t') + wxT("Alt-O"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSave );
	
	wxMenuItem* m_menuItemSaveAs;
	m_menuItemSaveAs = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("Save &As...") ) + wxT('\t') + wxT("Alt-A"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSaveAs );
	
	m_menubar1->Append( m_menuFile, wxT("&File") );
	
	m_menuEdit = new wxMenu();
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("&Copy") ) + wxT('\t') + wxT("Ctrl-C"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem11 );
	
	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("&Paste") ) + wxT('\t') + wxT("Ctrl-V"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem12 );
	
	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("C&ut") ) + wxT('\t') + wxT("Ctrl-X"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem15 );
	
	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("Select &all") ) + wxT('\t') + wxT("Ctrl-A"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem13 );
	
	wxMenuItem* m_menuItemCancelSelection;
	m_menuItemCancelSelection = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("&Unselect all") ) + wxT('\t') + wxT("Ctrl-U"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemCancelSelection );
	
	wxMenuItem* m_menuItem131;
	m_menuItem131 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("Delete Se&lected") ) + wxT('\t') + wxT("Ctrl-L"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem131 );
	
	m_menuEdit->AppendSeparator();
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("&Edit mode") ) + wxT('\t') + wxT("Ctrl-E"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem8 );
	
	wxMenuItem* m_menuItem9;
	m_menuItem9 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("&Selection mode") ) + wxT('\t') + wxT("Ctrl-S"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem9 );
	
	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("&Deletion mode") ) + wxT('\t') + wxT("Ctrl-D"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem10 );
	
	wxMenuItem* m_menuItem101;
	m_menuItem101 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( wxT("&Frame Selection mode") ) + wxT('\t') + wxT("Ctrl-F"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem101 );
	
	m_menuEdit->AppendSeparator();
	
	m_menubar1->Append( m_menuEdit, wxT("&Edit") );
	
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Show &Objects") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem16 );
	
	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Show &Tiles") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem17 );
	
	m_menubar1->Append( m_menu4, wxT("&View") );
	
	m_menu5 = new wxMenu();
	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("Clone") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem18 );
	
	wxMenuItem* m_menuItem20;
	m_menuItem20 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("Configure Grid") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem20 );
	
	wxMenuItem* m_menuItem19;
	m_menuItem19 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("ShowGrid") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem19 );
	
	wxMenuItem* m_menuItem22;
	m_menuItem22 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("Dock on grid") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem22 );
	
	wxMenuItem* m_menuItem151;
	m_menuItem151 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("&Magnetize") ) + wxT('\t') + wxT("Ctrl-M"), wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem151 );
	
	m_menubar1->Append( m_menu5, wxT("Tools") );
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem14 );
	
	m_menubar1->Append( m_menu3, wxT("&Help") );
	
	this->SetMenuBar( m_menubar1 );
	
	m_statusBarMode = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_toolBar1->AddTool( ToolDelete, wxT("tool"), wxBitmap( wxT("graphics/deleter.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( ToolEdit, wxT("edit"), wxBitmap( wxT("graphics/props.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->Realize();
	
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	m_treeCtrl3->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( GUIFrame::OnTreeCtrlItemActivated ), NULL, this );
	m_spinCtrlXCoord->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnSpinCtrlXCoord ), NULL, this );
	m_spinCtrlYCoord->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnSpinCtrlYCoord ), NULL, this );
	m_textCtrlEnergy->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextEnergy ), NULL, this );
	m_textCtrlAttack->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextAttack ), NULL, this );
	m_textCtrlLifetime->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextLifetime ), NULL, this );
	m_textCtrlVelocity->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextInitVelocity ), NULL, this );
	m_checkBoxCollDetection->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrame::OnCheckBoxCollDetection ), NULL, this );
	m_choiceInitialState->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrame::OnChoiceInitialState ), NULL, this );
	m_textCtrlInitialAnimFrame->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextInitialAnimFrame ), NULL, this );
	m_textCtrlAnimLifetime->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextAnimLifetime ), NULL, this );
	m_checkBoxAnimated->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrame::OnCheckBoxAnimated ), NULL, this );
	m_textCtrlGfxFile->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextGfxFile ), NULL, this );
	m_scrolledWindow2->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GUIFrame::OnScrWindowClick ), NULL, this );
	m_scrolledWindow2->Connect( wxEVT_MOTION, wxMouseEventHandler( GUIFrame::OnScrWindowMotion ), NULL, this );
	m_scrolledWindow2->Connect( wxEVT_PAINT, wxPaintEventHandler( GUIFrame::OnScrWindowPaint ), NULL, this );
	m_scrolledWindow2->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GUIFrame::OnScrWindowRClick ), NULL, this );
	this->Connect( m_menuItemNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemNew ) );
	this->Connect( m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemOpen ) );
	this->Connect( m_menuItemSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSave ) );
	this->Connect( m_menuItemSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSaveAs ) );
	this->Connect( m_menuItem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemCopy ) );
	this->Connect( m_menuItem12->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemPaste ) );
	this->Connect( m_menuItem15->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemCut ) );
	this->Connect( m_menuItem13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSelectAll ) );
	this->Connect( m_menuItemCancelSelection->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemCancelSelection ) );
	this->Connect( m_menuItem131->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemDeleteSelected ) );
	this->Connect( m_menuItem8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemEditMode ) );
	this->Connect( m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSelectionMode ) );
	this->Connect( m_menuItem10->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemDeletionMode ) );
	this->Connect( m_menuItem101->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemFrameMode ) );
	this->Connect( m_menuItem16->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemShowObjects ) );
	this->Connect( m_menuItem17->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemShowTiles ) );
	this->Connect( m_menuItem18->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemClone ) );
	this->Connect( m_menuItem20->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemConfigGrid ) );
	this->Connect( m_menuItem19->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemShowGrid ) );
	this->Connect( m_menuItem22->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemDockOnGrid ) );
	this->Connect( m_menuItem151->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemMagnetize ) );
	this->Connect( m_menuItem14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemAbout ) );
	this->Connect( ToolEdit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnMenuItemEditMode ) );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	m_treeCtrl3->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( GUIFrame::OnTreeCtrlItemActivated ), NULL, this );
	m_spinCtrlXCoord->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnSpinCtrlXCoord ), NULL, this );
	m_spinCtrlYCoord->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnSpinCtrlYCoord ), NULL, this );
	m_textCtrlEnergy->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextEnergy ), NULL, this );
	m_textCtrlAttack->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextAttack ), NULL, this );
	m_textCtrlLifetime->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextLifetime ), NULL, this );
	m_textCtrlVelocity->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextInitVelocity ), NULL, this );
	m_checkBoxCollDetection->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrame::OnCheckBoxCollDetection ), NULL, this );
	m_choiceInitialState->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrame::OnChoiceInitialState ), NULL, this );
	m_textCtrlInitialAnimFrame->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextInitialAnimFrame ), NULL, this );
	m_textCtrlAnimLifetime->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextAnimLifetime ), NULL, this );
	m_checkBoxAnimated->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrame::OnCheckBoxAnimated ), NULL, this );
	m_textCtrlGfxFile->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrame::OnTextGfxFile ), NULL, this );
	m_scrolledWindow2->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GUIFrame::OnScrWindowClick ), NULL, this );
	m_scrolledWindow2->Disconnect( wxEVT_MOTION, wxMouseEventHandler( GUIFrame::OnScrWindowMotion ), NULL, this );
	m_scrolledWindow2->Disconnect( wxEVT_PAINT, wxPaintEventHandler( GUIFrame::OnScrWindowPaint ), NULL, this );
	m_scrolledWindow2->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GUIFrame::OnScrWindowRClick ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemNew ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSave ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemCopy ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemPaste ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemCut ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSelectAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemCancelSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemDeleteSelected ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemEditMode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemSelectionMode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemDeletionMode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemFrameMode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemShowObjects ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemShowTiles ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemClone ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemConfigGrid ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemShowGrid ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemDockOnGrid ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemMagnetize ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuItemAbout ) );
	this->Disconnect( ToolEdit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnMenuItemEditMode ) );
}

DialogAbout::DialogAbout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Test"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetFont( wxFont( 50, 72, 90, 90, false, wxT("Arial") ) );
	
	bSizer19->Add( m_staticText12, 0, wxALL, 5 );
	
	this->SetSizer( bSizer19 );
	this->Layout();
}

DialogAbout::~DialogAbout()
{
}

DialogNew::DialogNew( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 2, 0, 0 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Horizontal size."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	gSizer1->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl11 = new wxTextCtrl( this, wxID_ANY, wxT("3000"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_textCtrl11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Vertical size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	gSizer1->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl12 = new wxTextCtrl( this, wxID_ANY, wxT("3000"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_textCtrl12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer20->Add( gSizer1, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_button6 = new wxButton( this, wxID_ANY, wxT("Create!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	bSizer20->Add( bSizer21, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer20 );
	this->Layout();
	
	// Connect Events
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNew::OnCreate ), NULL, this );
}

DialogNew::~DialogNew()
{
	// Disconnect Events
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNew::OnCreate ), NULL, this );
}

DialogClone::DialogClone( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 3, 3, 0, 0 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl11 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( textCtrl11, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer23, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer231;
	bSizer231 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl21 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer231->Add( textCtrl21, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer231, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2311;
	bSizer2311 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl31 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2311->Add( textCtrl31, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer2311, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2312;
	bSizer2312 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl12 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2312->Add( textCtrl12, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer2312, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23121;
	bSizer23121 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl22 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23121->Add( textCtrl22, 1, wxALL, 5 );
	
	textCtrlAngle = new wxTextCtrl( this, wxID_ANY, wxT("45"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23121->Add( textCtrlAngle, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer23121, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23122;
	bSizer23122 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl32 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23122->Add( textCtrl32, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer23122, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23123;
	bSizer23123 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl13 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23123->Add( textCtrl13, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer23123, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23124;
	bSizer23124 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl23 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23124->Add( textCtrl23, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer23124, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23125;
	bSizer23125 = new wxBoxSizer( wxHORIZONTAL );
	
	textCtrl33 = new wxTextCtrl( this, wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23125->Add( textCtrl33, 1, wxALL, 5 );
	
	gSizer2->Add( bSizer23125, 1, wxEXPAND, 5 );
	
	bSizer22->Add( gSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT("Do cloning ... times:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer47->Add( m_staticText15, 0, wxALL, 5 );
	
	textCtrlTimes = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer47->Add( textCtrlTimes, 0, wxALL, 5 );
	
	buttonGo = new wxButton( this, wxID_ANY, wxT("Go!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer47->Add( buttonGo, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer22->Add( bSizer47, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
	
	// Connect Events
	buttonGo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogClone::OnButtonClickGo ), NULL, this );
}

DialogClone::~DialogClone()
{
	// Disconnect Events
	buttonGo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogClone::OnButtonClickGo ), NULL, this );
}
