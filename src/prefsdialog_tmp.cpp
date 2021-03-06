// -*- C++ -*- generated by wxGlade 0.6.1 on Tue Jan 08 19:26:19 2008

#include "prefsdialog_tmp.h"

// begin wxGlade: ::extracode
// end wxGlade




display_prefs::display_prefs(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: display_prefs::display_prefs
    grid_styler_staticbox = new wxStaticBox(this, -1, wxT("Grid"));
    ar_styler_staticbox = new wxStaticBox(this, -1, wxT("Aspect ratio"));
    display_title = new wxStaticText(this, wxID_ANY, wxT("Display"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_3 = new wxStaticText(this, wxID_ANY, wxT("Proportion of your ingame resolution."));
    const wxString m_view_aspectratio_choices[] = {
        wxT("Standard (4:3)"),
        wxT("Widescreen (16:10)"),
        wxT("Custom")
    };
    m_view_aspectratio = new wxChoice(this, ID_COMBO_ASPECTRATIO, wxDefaultPosition, wxDefaultSize, 3, m_view_aspectratio_choices, 0);
    label_1 = new wxStaticText(this, wxID_ANY, wxT("Custom:"));
    m_view_aspectratio_custom = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_2 = new wxStaticText(this, wxID_ANY, wxT("width:height"));
    label_7 = new wxStaticText(this, wxID_ANY, wxT("NOTE: If you want to convert the current HUD to this new ratio as well\nyou shouldn't change it here but convert the hud in Tools->Convert HUD..."));
    label_4 = new wxStaticText(this, wxID_ANY, wxT("Distance X:"));
    m_view_gridX = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_5 = new wxStaticText(this, wxID_ANY, wxT("Distance Y:"));
    m_view_gridY = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);

    set_properties();
    do_layout();
    // end wxGlade
}


void display_prefs::set_properties()
{
    // begin wxGlade: display_prefs::set_properties
    display_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_3->SetForegroundColour(wxColour(0, 0, 85));
    m_view_aspectratio->SetSelection(0);
    m_view_aspectratio_custom->SetMinSize(wxSize(70, -1));
    label_2->SetForegroundColour(wxColour(0, 0, 85));
    label_7->SetForegroundColour(wxColour(0, 0, 85));
    m_view_gridX->SetMinSize(wxSize(70, -1));
    m_view_gridY->SetMinSize(wxSize(70, -1));
    // end wxGlade
}


void display_prefs::do_layout()
{
    // begin wxGlade: display_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* grid_styler = new wxStaticBoxSizer(grid_styler_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_el_sizer = new wxFlexGridSizer(2, 4, 0, 0);
    wxStaticBoxSizer* ar_styler = new wxStaticBoxSizer(ar_styler_staticbox, wxVERTICAL);
    wxBoxSizer* ar_el_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_8 = new wxBoxSizer(wxVERTICAL);
    sizer_8->Add(display_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    container->Add(sizer_8, 0, wxEXPAND, 0);
    ar_styler->Add(label_3, 0, wxALL|wxEXPAND, 3);
    ar_el_sizer->Add(m_view_aspectratio, 0, wxALL, 3);
    ar_el_sizer->Add(label_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    ar_el_sizer->Add(m_view_aspectratio_custom, 0, wxALL, 3);
    ar_el_sizer->Add(label_2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    ar_styler->Add(ar_el_sizer, 1, wxALL|wxEXPAND, 0);
    ar_styler->Add(label_7, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    container->Add(ar_styler, 0, wxALL|wxEXPAND, 3);
    grid_el_sizer->Add(label_4, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridX, 0, wxALL, 3);
    grid_el_sizer->Add(label_5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridY, 0, wxALL, 3);
    grid_styler->Add(grid_el_sizer, 1, wxEXPAND, 0);
    container->Add(grid_styler, 0, wxALL|wxEXPAND, 3);
    top_sizer->Add(container, 1, wxALL|wxEXPAND, 5);
    SetSizer(top_sizer);
    top_sizer->Fit(this);
    // end wxGlade
}










cpma_prefs::cpma_prefs(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
    // begin wxGlade: cpma_prefs::cpma_prefs
    sizer_5_staticbox = new wxStaticBox(this, -1, wxT("Background image"));
    sizer_3_staticbox = new wxStaticBox(this, -1, wxT("Quake3 path"));
    cpma_title = new wxStaticText(this, wxID_ANY, wxT("CPMA"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_13 = new wxStaticText(this, wxID_ANY, wxT("Where you installed Quake3 at."));
    m_q3_gamedir = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    button_2 = new wxButton(this, ID_BTN_Q3_GAMEDIR_BROWSE, wxT("..."));
    label_12 = new wxStaticText(this, wxID_ANY, wxT("Preferrably of width and height as power of two."));
    m_q3_background = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    button_2_copy = new wxButton(this, ID_BTN_Q3_BACKGROUND_BROWSE, wxT("..."));
    m_q3_background_usedefault = new wxCheckBox(this, ID_CHECK_Q3_BACKGROUND_USEDEFAULT, wxT("Use default"));

    set_properties();
    do_layout();
    // end wxGlade
}


void cpma_prefs::set_properties()
{
    // begin wxGlade: cpma_prefs::set_properties
    SetTitle(wxT("cpma_prefs"));
    cpma_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_13->SetForegroundColour(wxColour(0, 0, 85));
    button_2->SetMinSize(wxSize(40, -1));
    label_12->SetForegroundColour(wxColour(0, 0, 85));
    button_2_copy->SetMinSize(wxSize(40, -1));
    // end wxGlade
}


void cpma_prefs::do_layout()
{
    // begin wxGlade: cpma_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_5 = new wxStaticBoxSizer(sizer_5_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_10 = new wxBoxSizer(wxVERTICAL);
    sizer_10->Add(cpma_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    container->Add(sizer_10, 0, wxEXPAND, 0);
    sizer_3->Add(label_13, 0, wxALL, 3);
    sizer_4->Add(m_q3_gamedir, 1, wxALL|wxEXPAND, 3);
    sizer_4->Add(button_2, 0, wxALL, 3);
    sizer_3->Add(sizer_4, 1, wxEXPAND, 0);
    container->Add(sizer_3, 0, wxALL|wxEXPAND, 3);
    sizer_5->Add(label_12, 0, wxALL, 3);
    sizer_6->Add(m_q3_background, 1, wxALL|wxEXPAND, 3);
    sizer_6->Add(button_2_copy, 0, wxALL, 3);
    sizer_5->Add(sizer_6, 1, wxEXPAND, 0);
    sizer_5->Add(m_q3_background_usedefault, 0, wxALL, 3);
    container->Add(sizer_5, 0, wxALL|wxEXPAND, 3);
    top_sizer->Add(container, 1, wxALL|wxEXPAND, 5);
    SetSizer(top_sizer);
    top_sizer->Fit(this);
    Layout();
    // end wxGlade
}






misc_prefs::misc_prefs(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
    // begin wxGlade: misc_prefs::misc_prefs
    sizer_2_staticbox = new wxStaticBox(this, -1, wxT("Message dialogs"));
    sizer_7_staticbox = new wxStaticBox(this, -1, wxT("Elements list"));
    sizer_1_staticbox = new wxStaticBox(this, -1, wxT("Load / Save"));
    misc_title = new wxStaticText(this, wxID_ANY, wxT("Misc"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    m_save_backup = new wxCheckBox(this, wxID_ANY, wxT("Create backup before saving"));
    m_startup_load = new wxCheckBox(this, wxID_ANY, wxT("Reload previously open HUD on startup"));
    label_14 = new wxStaticText(this, wxID_ANY, wxT("On several occassions you are prompted a message dialog that\nyou can disable to pop up. If you want to enable them all\nagain, click the button below."));
    button_1 = new wxButton(this, ID_BTN_RESETDIALOGS, wxT("Reset dialogs"));
    m_elements_collections = new wxCheckBox(this, wxID_ANY, wxT("Use collections"));
    label_6 = new wxStaticText(this, wxID_ANY, wxT("Collections are a mean to gather similar named elements together."));

    set_properties();
    do_layout();
    // end wxGlade
}


void misc_prefs::set_properties()
{
    // begin wxGlade: misc_prefs::set_properties
    SetTitle(wxT("misc_prefs"));
    misc_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_14->SetForegroundColour(wxColour(0, 0, 85));
    label_6->SetForegroundColour(wxColour(0, 0, 85));
    // end wxGlade
}


void misc_prefs::do_layout()
{
    // begin wxGlade: misc_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_7 = new wxStaticBoxSizer(sizer_7_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_2 = new wxStaticBoxSizer(sizer_2_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_1 = new wxStaticBoxSizer(sizer_1_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_9 = new wxBoxSizer(wxVERTICAL);
    sizer_9->Add(misc_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    container->Add(sizer_9, 0, wxEXPAND, 0);
    sizer_1->Add(m_save_backup, 0, wxALL, 3);
    sizer_1->Add(m_startup_load, 0, wxALL, 3);
    container->Add(sizer_1, 0, wxALL|wxEXPAND, 3);
    sizer_2->Add(label_14, 0, wxALL, 3);
    sizer_2->Add(button_1, 0, wxALL, 3);
    container->Add(sizer_2, 0, wxALL|wxEXPAND, 3);
    sizer_7->Add(m_elements_collections, 0, wxALL, 3);
    sizer_7->Add(label_6, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
    container->Add(sizer_7, 0, wxALL|wxEXPAND, 3);
    top_sizer->Add(container, 1, wxALL|wxEXPAND, 5);
    SetSizer(top_sizer);
    top_sizer->Fit(this);
    Layout();
    // end wxGlade
}


advanced_prefs::advanced_prefs(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
    // begin wxGlade: advanced_prefs::advanced_prefs

    set_properties();
    do_layout();
    // end wxGlade
}


void advanced_prefs::set_properties()
{
    // begin wxGlade: advanced_prefs::set_properties
    SetTitle(wxT("advanced_prefs"));
    // end wxGlade
}


void advanced_prefs::do_layout()
{
    // begin wxGlade: advanced_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    top_sizer->Add(container, 1, wxEXPAND, 0);
    SetSizer(top_sizer);
    top_sizer->Fit(this);
    Layout();
    // end wxGlade
}






























advanced2_prefs::advanced2_prefs(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
// content of this block not found: did you rename this class?
}


void advanced2_prefs::set_properties()
{
// content of this block not found: did you rename this class?
}


void advanced2_prefs::do_layout()
{
// content of this block not found: did you rename this class?
}
















network_prefs::network_prefs(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
    // begin wxGlade: network_prefs::network_prefs
    sizer_13_staticbox = new wxStaticBox(this, -1, wxT("Proxy"));
    sizer_12_staticbox = new wxStaticBox(this, -1, wxT("Startup"));
    network_title = new wxStaticText(this, wxID_ANY, wxT("Network"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    m_startup_checkforupdate = new wxCheckBox(this, wxID_ANY, wxT("Check for new version"));
    m_net_proxy = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_8 = new wxStaticText(this, wxID_ANY, wxT("host:port"));

    set_properties();
    do_layout();
    // end wxGlade
}


void network_prefs::set_properties()
{
    // begin wxGlade: network_prefs::set_properties
    SetTitle(wxT("network_prefs"));
    network_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_8->SetForegroundColour(wxColour(0, 0, 85));
    // end wxGlade
}


void network_prefs::do_layout()
{
    // begin wxGlade: network_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_13 = new wxStaticBoxSizer(sizer_13_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_12 = new wxStaticBoxSizer(sizer_12_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_11 = new wxBoxSizer(wxVERTICAL);
    sizer_11->Add(network_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    container->Add(sizer_11, 0, wxEXPAND, 0);
    sizer_12->Add(m_startup_checkforupdate, 0, wxALL, 3);
    container->Add(sizer_12, 0, wxALL|wxEXPAND, 3);
    sizer_13->Add(m_net_proxy, 0, wxALL|wxEXPAND, 3);
    sizer_13->Add(label_8, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);
    container->Add(sizer_13, 0, wxALL|wxEXPAND, 3);
    top_sizer->Add(container, 0, wxALL|wxEXPAND, 5);
    SetSizer(top_sizer);
    top_sizer->Fit(this);
    Layout();
    // end wxGlade
}

