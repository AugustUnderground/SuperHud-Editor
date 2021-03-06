#include "q4max_hudfile.h"
#include "common.h"
#include "elementsctrlbase.h"
#include "pakmanager.h"
#include "hudspecs.h"

#include "q4max_element.h"

#include <wx/txtstrm.h>
#include <wx/datetime.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/tokenzr.h>


//#include <wx/stdpaths.h>
//#include <wx/dir.h>
//#include <algorithm>


Q4MAXHudFile::Q4MAXHudFile() : 
  HudFileBase()
{
}


/*
bool Q4MAXHudFile::load( const wxString& filename )
{
  wxLogDebug(wxT("Loading HUD: ") + filename);

  wxString content;
  char *buf;
  size_t size;

  if( !PakManager::get().load( &buf, filename, PM_SEARCH_EVERYWHERE, &size ) )
  {
    wxLogError(_("Couldn't find/load HUD: %s"), filename.c_str());
    return false;
  }


  wxMemoryInputStream mis( buf, size );

  m_load_prevel = 0;

  load_default_elements();
  wxTextInputStream tis( mis );
  wxString line;
  size_t pos;
  // default opts

  // read file line by line and remove `#' comments
  while(!mis.Eof())
  {
    line = tis.ReadLine();
    she::wxTrim(line);
    if( 0 == line.length() || line[0] == '#' )
    { // check for options
      if( (pos = line.find(wxT("="))) != wxString::npos )
      {
        wxString optname = line.Mid(1, pos-1);
        wxString optval = line.Mid(pos+1);
        she::wxTrim(optname);
        she::wxTrim(optval);
        wxLogDebug(wxT("Q4MAXHudFile::load - have found option: ") + optname + wxT(" = ") + optval);
        if( optname == wxT("version") )
          m_opt_version = optval;
        else if( optname == wxT("view_aspectratio") )
          m_opt_aspectratio = optval;
        else
          wxLogDebug(wxT("Q4MAXHudFile::load - WARNING: invalid option ") + optname);
      }
      continue;
    }
    if( (pos = line.find( wxT("#") )) != wxString::npos )
      line = line.substr( 0, pos );
    content += line;
    content += wxT("\n");
  }

  she::wxTrim( content ); 
  try
  {
    wxStringTokenizer tok( content, wxT("}") );
    while(tok.HasMoreTokens())
      parse_item( tok.GetNextToken() );
  }
  catch( hudfile_parse_error& err )
  {
    wxString str = wxString::Format(_("ERROR while parsing `%s'"), filename.c_str());
    str += wxString(err.what(), wxConvUTF8);
    wxLogError( str );
    load_default_elements();
    return false;
  }
  // removeo all non-unique elements that aren't enabled
  for( it_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( ((*it)->flags() & E_NOTUNIQ) && !(*it)->is_enabled() )
      m_els.erase( it );
  }

  // remove E_DRAWNEVER and E_PARENT from end
  ElementBase *el;
  for( int i = m_els.size()-1; i >= 0; --i )
  {
    el = m_els[i];
    if( el->flags() & E_DRAWNEVER && el->flags() & E_PARENT )
    {
      m_els.erase(m_els.begin() + i);
      delete el;
    }
    else
      break;
  }
  
  m_filename = filename;
  wxGetApp().mainframe()->update_elementsctrl();
  m_modified = false;
  return true;
}
*/

/// extracts a word with regard of doublequotes are surrounding a word
wxString extract_word( const wxString& s, size_t *start = 0 )
{
  size_t pos = (start ? *start : 0);
  wxString word;
  size_t i = pos;
  bool quoting = false;
  for( ; i < s.Length(); ++i )
  {
    if( '"' == s[i] )
    {
      if( quoting )
        break;
      quoting = true;
      continue;
    }
    if( !quoting && (s[i] == '\n' || s[i] == '\t' || s[i] == ';' || s[i] == ' ') )
    {
      if( word.empty() )
        continue; // skip whitespace at start
      break;
    }
    word += s[i];
  }
  if( start )
    *start = i+1;

  return word;
}

bool Q4MAXHudFile::read_properties( ElementBase *hi, const wxString& props )
{
  wxString okprop;
  size_t pos = 0;
  wxString propname;
  wxString propval;
  do
  {
    propname = extract_word(props, &pos);
    propval = extract_word(props, &pos);
    okprop += propname + wxT(" ") + propval + wxT("; ");
  }
  while( !propname.empty() );

  return HudFileBase::read_properties( hi, okprop );

  /*
  // add dividing semilicon between elements (q4max allows to omit it)
  // but it also has the requirement that each property can have only one
  // param (if there are several arguments they have to be enclosed in doublequotes)
  bool quoting = false;
  bool name = true; // we start with a name
  bool arg = false;
  fixedprop.Replace(wxT("\n"), wxT(" "));
  she::wxTrim(fixedprop, wxT(" "));
  for( wxString::const_iterator cit = props.begin(); cit != props.end(); ++cit )
  {
    if( *cit == ' ' )
    {
      if( name )
      {
        name = false;
        arg = true; // next is arg
      }
    }
    if( *cit == '"' )
    {
      if( quoting )
      { // close
        p += *cit;
        p += ';';
        quoting = false;
        continue;
      }
      else // !quoting
        quoting = true;
    }
    p += *cit;
  }
  return HudFileBase::read_properties( hi, p );
  */
}

