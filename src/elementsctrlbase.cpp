// -*- C++ -*- generated by wxGlade 0.6.1 on Wed Dec 05 16:09:56 2007

#include "elementsctrlbase.h"
#include "elementbase.h"
#include "misc.h"
#include "mainframe.h"

#include <wx/sstream.h>
#include <wx/txtstrm.h>

#include <list>
using namespace std;

#include "xpm/icons/predecorate_insert.xpm"
#include "xpm/icons/postdecorate_insert.xpm"
#include "xpm/icons/element_copy.xpm"
#include "xpm/icons/element_paste.xpm"

BEGIN_EVENT_TABLE(ElementsCtrlBase, wxPanel)
  EVT_BUTTON(ID_BTN_COPY, ElementsCtrlBase::OnBtnCopy)
  EVT_BUTTON(ID_BTN_PASTE, ElementsCtrlBase::OnBtnPaste)

END_EVENT_TABLE()

// begin wxGlade: ::extracode

// end wxGlade





ElementsCtrlBase::ElementsCtrlBase(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
  /* FIXMEHERE
  m_listctrl = new ElementsListCtrl(this);
  m_btn_copy = new wxBitmapButton(this, ID_BTN_COPY, wxBitmap(element_copy_xpm));
  m_btn_paste = new wxBitmapButton(this, ID_BTN_PASTE, wxBitmap(element_paste_xpm));
  */
    // begin wxGlade: ElementsCtrlBase::ElementsCtrlBase
    m_btn_insertdefault = new wxButton(this, ID_BTN_INSERTDEFAULT, wxT("+ !DEFAULT"));
    m_btn_insertpredecorate = new wxButton(this, ID_BTN_INSERTPREDECORATE, wxT("+ PreDecorate"));
    m_btn_insertpostdecorate = new wxButton(this, ID_BTN_INSERTPOSTDECORATE, wxT("+ PostDecorate"));
    m_listctrl = new ElementsListCtrl(this);
    m_btn_copy = new wxBitmapButton(this, ID_BTN_COPY, wxBitmap(element_copy_xpm));
    m_btn_paste = new wxBitmapButton(this, ID_BTN_PASTE, wxBitmap(element_paste_xpm));

    set_properties();
    do_layout();
    // end wxGlade
}

    void ElementsCtrlBase::set_properties()
{
    // begin wxGlade: ElementsCtrlBase::set_properties
    m_btn_insertdefault->SetToolTip(wxT("Insert a !DEFAULT element"));
    m_btn_insertpredecorate->SetToolTip(wxT("Insert a PreDecorate element"));
    m_btn_insertpostdecorate->SetToolTip(wxT("Insert a PostDecorate element"));
    m_btn_copy->SetToolTip(wxT("Copy element properties"));
    m_btn_copy->SetSize(m_btn_copy->GetBestSize());
    m_btn_paste->SetToolTip(wxT("Paste element properties"));
    m_btn_paste->SetSize(m_btn_paste->GetBestSize());
    // end wxGlade
    m_btn_paste->Disable();
}


void ElementsCtrlBase::do_layout()
{
    // begin wxGlade: ElementsCtrlBase::do_layout
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    sizer_1->Add(m_btn_insertdefault, 0, 0, 0);
    sizer_1->Add(m_btn_insertpredecorate, 0, 0, 0);
    sizer_1->Add(m_btn_insertpostdecorate, 0, 0, 0);
    sizer_3->Add(sizer_1, 0, wxEXPAND, 0);
    sizer_3->Add(m_listctrl, 1, wxEXPAND, 0);
    sizer_4->Add(m_btn_copy, 0, 0, 0);
    sizer_4->Add(m_btn_paste, 0, 0, 0);
    sizer_3->Add(sizer_4, 0, wxEXPAND, 0);
    SetSizer(sizer_3);
    sizer_3->Fit(this);
    // end wxGlade
}



void ElementsCtrlBase::append( ElementBase *el )
{
  long idx = m_listctrl->InsertItem(m_listctrl->GetItemCount(), wxEmptyString, -1);
  m_listctrl->SetItem( idx, 1, el->name(), (el->is_enabled() ? E_LIST_IMG_ENABLED : E_LIST_IMG_DISABLED));
  m_listctrl->SetItemData( idx, (int)(el) );
}

void ElementsCtrlBase::clear()
{
  m_listctrl->DeleteAllItems();
}

void ElementsCtrlBase::list_refresh( const HudFileBase::elements_type& elements )
{
  clear();
  for( HudFileBase::cit_elements cit = elements.begin(); cit != elements.end(); ++cit )
  {
    append(*cit);
  }
  // now insert collection items
  
  wxString collname;
  int collcount = 0;
  for( size_t i=1; i < elements.size(); ++i )
  {
    if( elements[i]->name().Left(3) == elements[i-1]->name().Left(3) && elements[i]->name().Left(3) != collname)
    { // we found at least two items, that's enough
      collname = elements[i]->name().Left(3);

      // how many items belong to this collection?
      size_t g;
      for( g=i-1; g < elements.size(); ++g )
      {
        if( elements[g]->name().Left(3) != collname )
          break;
      }
      // items [i-1,g-1] have same 3 starting characters
      // maybe they share even more? figeur out
      int minshare = 666;
      m_listctrl->SetItem(i-1+collcount, 0, wxEmptyString, E_LIST_IMG_COLLITEM);
      for( size_t h=i; h <= g-1; ++h )
      {
        m_listctrl->SetItem(h+collcount, 0, wxEmptyString, E_LIST_IMG_COLLITEM);
        minshare = wxMin(common_start(elements[h]->name(), elements[h-1]->name()), minshare);
      }

      // insert collection title
      wxListItem li;
      li.SetMask(wxLIST_MASK_TEXT);
      li.SetId(i-1+collcount);
      li.SetFont(*wxITALIC_FONT);
      li.SetTextColour(wxColour(*wxWHITE));
      li.SetBackgroundColour(wxColour(*wxBLACK));
      
      long idx = m_listctrl->InsertItem(li);
      m_listctrl->SetItem(idx, 0, wxEmptyString, E_LIST_IMG_COLLTITLE);
      collname = elements[i]->name().Left(minshare);
      wxTrim(collname, wxT("_"));
      m_listctrl->SetItem(idx, 1, collname, -1);
      m_listctrl->SetItemData(idx, 0);
      
      ++collcount;
      i = g-1; // skip over values we just put in a collection
    }
  }
     
}


void ElementsCtrlBase::OnBtnCopy( wxCommandEvent& )
{
  wxLogDebug(wxT("copy"));
}

void ElementsCtrlBase::OnBtnPaste( wxCommandEvent& )
{
  wxLogDebug(wxT("paste"));
}

void ElementsCtrlBase::OnSelectionChanged()
{
  // disable/enable copy/paste buttons
  if( -1 == m_listctrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED) )
  { // none selected
    m_btn_copy->Disable();
    m_btn_paste->Disable();
  }
  else
  { // there is a selection
    m_btn_copy->Enable();
    m_btn_paste->Disable();
  }
  

  // update textcontrol
  //wxTextCtrl *txt = wxGetApp().mainframe()->textpreview();
  wxTextCtrl *txt = ((MainFrame*)GetParent())->textpreview();
  wxString out;
  wxStringOutputStream sos(&out);
  wxTextOutputStream tos(sos);
  // get all selected
  long idx = -1;
  list<int> print; // stores all indecies that should be printed
  wxListItem info;
  for ( ;; )
  {
    idx = m_listctrl->GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if ( idx == -1 )
        break;
    // this item is selected
    info.m_mask = wxLIST_MASK_DATA|wxLIST_MASK_TEXT;
    info.m_col = 1;
    info.m_itemId = idx;
    if( m_listctrl->GetItem(info) )
    {
      if( info.GetData() )
      { // a real element
        print.push_back(idx);
      }
      else
      { // collection item, get all following items with same text
        wxString collname = info.GetText();
        int i = idx+1;
        while( i < m_listctrl->GetItemCount() )
        {
          info.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA;
          info.m_col = 1;
          info.m_itemId = i;
          if( !m_listctrl->GetItem(info) )
            break;
          if( info.GetText().StartsWith(collname) )
            print.push_back(i);
          else
            break;
          ++i;
        }
      }
    }
  }
  print.sort();
  print.unique();
  for( list<int>::iterator it = print.begin(); it != print.end(); ++it )
  {
    info.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA;
    info.m_col = 1;
    info.m_itemId = *it;
    if( m_listctrl->GetItem(info) )
    {
<<<<<<< .mine
      wxLogDebug(wxT("%s = %d"), info.GetText().c_str(), info.GetData());
//      ElementBase *el = reinterpret_cast<ElementBase*>(info.GetData());
//      HudFileBase::write_element(tos, *el);
=======
      wxLogDebug(wxT("listelement data `%s' = %d"), info.GetText(), info.GetData());
      //ElementBase *el = reinterpret_cast<ElementBase*>(info.GetData());
      //HudFileBase::write_element(tos, *el);
>>>>>>> .r391
    }
    else
      tos << wxT("\n# ERROR: failed retrieving iteminfo `") << info.GetText() << wxT("'\n");
  }
  
  txt->SetValue(out);
}
