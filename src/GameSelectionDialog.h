// -*- C++ -*- generated by wxGlade 0.6.1 on Thu Dec 06 10:11:23 2007

#include <wx/wx.h>
#include <wx/image.h>

#ifndef GAMESELECTIONDIALOG_H
#define GAMESELECTIONDIALOG_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade


class GameSelectionDialog: public wxDialog {
public:
    // begin wxGlade: GameSelectionDialog::ids
    // end wxGlade

    GameSelectionDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

    bool startup_gameselection() const { return !m_startup_gameselection->IsChecked(); }

  private:
    void OnBtnCPMA(wxCommandEvent&);
    void OnBtnQ4MAX(wxCommandEvent&);
    

private:
    // begin wxGlade: GameSelectionDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: GameSelectionDialog::attributes
    wxBitmapButton* bitmap_button_2;
    wxBitmapButton* bitmap_button_3;
    wxStaticText* label_2;
    wxStaticText* label_3;
    wxCheckBox* m_startup_gameselection;
    // end wxGlade
   DECLARE_EVENT_TABLE()
}; // wxGlade: end class


#endif // GAMESELECTIONDIALOG_H
