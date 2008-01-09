// -*- C++ -*- generated by wxGlade 0.6.1 on Tue Jan 08 19:26:19 2008

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
// end wxGlade


#ifndef PREFSDIALOG_TMP_H
#define PREFSDIALOG_TMP_H


// begin wxGlade: ::extracode
// end wxGlade



class display_prefs: public wxPanel {
public:
    // begin wxGlade: display_prefs::ids
    // end wxGlade

    display_prefs(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: display_prefs::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: display_prefs::attributes
    wxStaticBox* grid_styler_staticbox;
    wxChoice* m_aspectratio;
    wxStaticText* label_1;
    wxTextCtrl* m_aspectratioX;
    wxStaticText* label_2;
    wxTextCtrl* m_aspectratioY;
    wxStaticText* label_3;
    // end wxGlade
}; // wxGlade: end class

class test_prefs: public wxPanel {
public:
// content of this block (ids) not found: did you rename this class?

    test_prefs(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    void set_properties();
    void do_layout();

protected:
// content of this block (attributes) not found: did you rename this class?
}; // wxGlade: end class


#endif // PREFSDIALOG_TMP_H
