#ifndef PAKFILEDIALOG_H
#define PAKFILEDIALOG_H

#include "common.h"

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/imaglist.h>
// begin wxGlade: ::dependencies
#include <wx/listctrl.h>
#include <wx/statline.h>
// end wxGlade

#include "pakmanager.h"





// begin wxGlade: ::extracode
// end wxGlade


class PakFileDialog: public wxDialog {
public:
    // begin wxGlade: PakFileDialog::ids
    // end wxGlade

    PakFileDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

  public:
    virtual bool Show(bool show = true);
    wxString      GetPakPath() { return m_curpath + m_selected; }
    PakFileDialog& AddExt( const wxString& ext) { m_exts.Add(ext); return *this; }
    PakFileDialog& AddExtImages();

  protected:
    wxImageList m_imglist;
    wxString  m_curpath;
    wxString  m_selected;
    pakbrowser_files_type m_files;
    pakbrowser_dirs_type m_dirs;
    wxArrayString m_exts; ///< which extensions we accept, is empty if we accept only directories
    void update_pakpath( const wxString& dir );
    void update_preview( const wxString& loc );
    bool is_valid_ext( const wxString& ext ) const;
    void open_dir( const wxString& name );
    /// updates the infolabel if there is no specific label around
    void update_nolabel();

    void          OnListItemDblClick( wxListEvent& );
    void          OnListItemSelected( wxListEvent& );
    void          OnListItemDeselected( wxListEvent& );
    void          OnBtnGodirup( wxCommandEvent& );
    void          OnKeyDown( wxKeyEvent& );

    virtual bool  Validate();

    

private:
    // begin wxGlade: PakFileDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PakFileDialog::attributes
    wxStaticBox* sizer_6_staticbox;
    wxBitmapButton* m_godirupbtn;
    wxStaticText* m_curpathlabel;
    wxListCtrl* m_list;
    wxStaticBitmap* m_preview;
    wxStaticText* m_infolabel;
    wxStaticLine* static_line_1;
    wxGauge* m_gauge;
    wxButton* button_2;
    wxButton* button_1;
    // end wxGlade

    DECLARE_EVENT_TABLE()
}; // wxGlade: end class


#endif // PAKFILEDIALOG_H
