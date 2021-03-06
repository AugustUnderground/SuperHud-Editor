#include "pakfiledialog.h"

#include "common.h"
#include "artprovider.h"

#include <wx/mstream.h>

#include <algorithm>


// begin wxGlade: ::extracode
// end wxGlade

BEGIN_EVENT_TABLE(PakFileDialog, wxDialog)
  EVT_LIST_ITEM_ACTIVATED(ID_LISTCTRL_PAKFILEDIALOG, PakFileDialog::OnListItemDblClick)
  EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_PAKFILEDIALOG, PakFileDialog::OnListItemSelected)
  EVT_LIST_ITEM_DESELECTED(ID_LISTCTRL_PAKFILEDIALOG, PakFileDialog::OnListItemDeselected)
  EVT_BUTTON(ID_BTN_PAKFILEDLG_GODIRUP, PakFileDialog::OnBtnGodirup)
END_EVENT_TABLE()

PakFileDialog::PakFileDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER),
      m_imglist(16, 16, true)
{
    // begin wxGlade: PakFileDialog::PakFileDialog
    sizer_6_staticbox = new wxStaticBox(this, -1, wxT("Preview"));
    m_godirupbtn = new wxBitmapButton(this, ID_BTN_PAKFILEDLG_GODIRUP, wxNullBitmap);
    m_curpathlabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
    m_list = new wxListCtrl(this, ID_LISTCTRL_PAKFILEDIALOG, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxLC_SINGLE_SEL|wxLC_SORT_ASCENDING|wxSUNKEN_BORDER);
    m_preview = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);
    m_infolabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
    static_line_1 = new wxStaticLine(this, wxID_ANY);
    m_gauge = new wxGauge(this, wxID_ANY, 10, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH);
    button_2 = new wxButton(this, wxID_CANCEL, wxEmptyString);
    button_1 = new wxButton(this, wxID_OK, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    m_gauge->SetValue(0);
    m_godirupbtn->SetBitmapLabel( wxArtProvider::GetBitmap(wxART_GO_DIR_UP, wxART_TOOLBAR, wxSize(16,16)) );
    m_list->SetImageList( &m_imglist, wxIMAGE_LIST_SMALL );
    m_imglist.Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    m_imglist.Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    m_list->Connect( ID_LISTCTRL_PAKFILEDIALOG, wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(PakFileDialog::OnKeyDown) );
}


void PakFileDialog::set_properties()
{
    // begin wxGlade: PakFileDialog::set_properties
    SetTitle(wxT("Pak File Browser"));
    SetSize(wxSize(500, 400));
    m_godirupbtn->SetSize(m_godirupbtn->GetBestSize());
    m_preview->SetMinSize(wxSize(64, 64));
    // end wxGlade
}


void PakFileDialog::do_layout()
{
    // begin wxGlade: PakFileDialog::do_layout
    wxBoxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_6 = new wxStaticBoxSizer(sizer_6_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
    sizer_7->Add(m_godirupbtn, 0, wxALL, 5);
    sizer_7->Add(m_curpathlabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    mainsizer->Add(sizer_7, 0, wxEXPAND, 0);
    sizer_2->Add(m_list, 1, wxALL|wxEXPAND, 5);
    sizer_6->Add(m_preview, 0, 0, 0);
    sizer_5->Add(sizer_6, 0, wxEXPAND, 0);
    sizer_2->Add(sizer_5, 0, wxEXPAND, 0);
    mainsizer->Add(sizer_2, 1, wxEXPAND, 0);
    mainsizer->Add(m_infolabel, 0, wxALL, 5);
    mainsizer->Add(static_line_1, 0, wxEXPAND, 0);
    sizer_4->Add(m_gauge, 1, wxEXPAND, 0);
    sizer_4->Add(button_2, 0, wxALL, 5);
    sizer_4->Add(button_1, 0, wxALL, 5);
    mainsizer->Add(sizer_4, 0, wxEXPAND, 0);
    SetSizer(mainsizer);
    Layout();
    // end wxGlade
}

bool PakFileDialog::Validate()
{
  if( m_selected.empty() )
  {
//    m_infolabel->SetLabel(_("Please select a file"));
    return false; // nothing selected
  }

  if( m_files.find(m_selected) != m_files.end() )
  { // it's a file woohooh
    for( size_t i=0; i < m_exts.Count(); ++i )
    {
      if( m_selected.Matches(wxT("*.") + m_exts[i]) )
        return true;
    }
    // file selected of which we don't accept the extension
    return false; 
  }

  // must be a dir then
  wxASSERT(std::find(m_dirs.begin(), m_dirs.end(), m_selected) != m_dirs.end());
  if( !m_exts.Count() ) // we accept dirs?
    return true;

  open_dir(m_selected);
  return false;
}

PakFileDialog& PakFileDialog::AddExtImages()
{ 
  AddExt(wxT("tga")).AddExt(wxT("png")).AddExt(wxT("jpg")).AddExt(wxT("bmp")).AddExt(wxT("gif")).
    AddExt(wxT("xpm")).AddExt(wxT("ico")).AddExt(wxT("cur"));
  return *this; 
}


bool PakFileDialog::Show(bool show)
{
  bool ret = wxDialog::Show(show);
  if( !show ) return ret;
  update_nolabel();
  update_pakpath(wxT(""));

  return ret;
}

void PakFileDialog::update_nolabel()
{
  if( !m_exts.Count() )
    m_infolabel->SetLabel(_("Select a directory"));
  else
  {
    wxString l = _("Select a file");
    l += wxT(" (");
    for( size_t i=0; i < m_exts.Count(); ++i )
    {
      if( i != 0 ) l += wxT(" ");
      l += m_exts[i];
    }
    l += wxT(")");
    m_infolabel->SetLabel(l);
  }
}

void PakFileDialog::OnListItemDeselected( wxListEvent& ev )
{
  m_selected = wxEmptyString;
  update_nolabel();
  m_preview->SetBitmap(wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(64, 64)));
}

void PakFileDialog::OnListItemSelected( wxListEvent& ev )
{
  m_selected = ev.GetLabel();
  pakbrowser_files_type::iterator fit = m_files.find(m_selected);
  if( fit != m_files.end() )
  { // it's a file
    update_preview(fit->second + wxT("#zip:") + m_curpath + m_selected);
    pakbrowser_files_type::iterator fit = m_files.find(m_selected);
#ifdef NDEBUG
    if( fit != m_files.end() )
#endif
      m_infolabel->SetLabel( _("Archive: ") + fit->second);
    
  }
  else
  { // it's a dir
    m_preview->SetBitmap(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(64, 64)));
    update_nolabel();
  }
}

void PakFileDialog::update_preview( const wxString& loc )
{
  if( !she::is_image_ext(she::file_ext(loc)) )
  {
    m_preview->SetBitmap(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(64, 64)));
    m_infolabel->SetLabel(wxT(""));
    return;
  }
  char *buf;
  size_t size;
  if( !PakManager::load_from_location( &buf, loc, &size ) )
  {
    wxLogError(_("Failed reading preview: %s"), loc.c_str());
    return;
  }
  
  wxMemoryInputStream mis( buf, size );
  wxImage img;
  if( !img.LoadFile(mis, she::bitmap_type_by_ext(she::file_ext(loc))) )
  {
    wxLogError(_("Failed loading image: %s"), loc.c_str());
    delete [] buf;
    return;
  }
  img.Rescale(64, 64);
  m_preview->SetBitmap(wxBitmap(img));
  delete [] buf;
}

void PakFileDialog::OnListItemDblClick( wxListEvent& ev )
{
  wxString name = ev.GetLabel();
  open_dir(name);
}

void PakFileDialog::open_dir( const wxString& name )
{
  pakbrowser_dirs_type::iterator dirit = std::find( m_dirs.begin(), m_dirs.end(), name );
  if( m_dirs.end() == dirit )
  { // so it must be a file?
    pakbrowser_files_type::iterator fit = m_files.find(name);
#ifdef NDEBUG
    // but still check in release1
    if( fit != m_files.end() )
#endif
    wxLogDebug( name + wxT(" -> ") + fit->second);
  }
  else
    update_pakpath( m_curpath + name + wxT("/") );
}

void PakFileDialog::OnBtnGodirup( wxCommandEvent& )
{
  if( m_curpath.empty() )
    return;
  wxString changepath = wxT("");
  size_t pos = m_curpath.find_last_of( wxT("/"), m_curpath.length()-2 );
  if( pos != wxString::npos )
    changepath = m_curpath.SubString(0, pos);
  update_pakpath(changepath);
}

void PakFileDialog::OnKeyDown( wxKeyEvent& ev )
{
  if( ev.GetKeyCode() == WXK_BACK )
  {
    wxCommandEvent fump(wxEVT_COMMAND_BUTTON_CLICKED, ID_BTN_PAKFILEDLG_GODIRUP);
    AddPendingEvent(fump);
  }
  else
    ev.Skip();
}

bool PakFileDialog::is_valid_ext( const wxString& ext ) const
{
  if( m_exts.Count() == 0 )
    return false;
  for( size_t i=0; i<m_exts.Count(); ++i )
    if( m_exts[i] == ext )
      return true;
  return false;
}

void PakFileDialog::update_pakpath( const wxString& pakpath )
{
  //m_gauge->Show();
  wxBusyCursor schaffe;

  wxLogDebug( wxT("change dir to ") + pakpath );
  m_dirs.clear();
  m_files.clear();
  m_list->ClearAll();
  PakManager::get().enumerate_pakdircontents(pakpath, &m_dirs, &m_files, m_gauge);
  int i = 0;
  for(pakbrowser_dirs_type::const_iterator cit = m_dirs.begin(); cit != m_dirs.end(); ++cit )
  {
    long idx = m_list->InsertItem(i, *cit);
    m_list->SetItemImage(idx, 0);
    ++i;
  }
  for(pakbrowser_files_type::const_iterator cit = m_files.begin(); cit != m_files.end(); ++cit )
  {
    if( !is_valid_ext(she::file_ext(cit->first)) )
      continue;
    long idx = m_list->InsertItem(i, cit->first);
    m_list->SetItemImage(idx, 1);
    ++i;
  }
  m_curpath = pakpath;
  m_curpathlabel->SetLabel( wxT("/") + pakpath);
  m_gauge->SetValue(0);
}


