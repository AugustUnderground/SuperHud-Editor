#include "cpma_element.h"
#include "hudfilebase.h"
#include "displayctrlbase.h"
#include "prefs.h"
#include "texture.h"
#include "pakmanager.h"

#include <wx/tokenzr.h>

#include <cmath>
#include <list>
using namespace std;

CPMAElement::CPMAElement( const wxString& name, const wxString& desc /*=""*/, int type /*=E_T_UNKNOWN*/, 
  bool enable /*=false*/, int flags /*= E_NONE*/,
  wxInt64 has /*= E_HAS_NONE*/,
  const wxString& text /*=""*/, 
  const wxString& icon /*=""*/, 
  const wxRect& rect /*= E_RECT_DEFAULT*/) :
    ElementBase(name, desc, flags, has, enable, rect),
    m_type(type),
    m_props(),
    m_text(text),
    m_icon(icon),
    m_ptex(0)
{
}

CPMAElement::Properties::Properties() :
    font(CPMA_E_FONT_DEFAULT),
    fontsize_type(CPMA_E_FST_POINT),
    fontsize_pt(CPMA_E_FONTSIZE_DEFAULT_POINT), fontsize_x(CPMA_E_FONTSIZE_DEFAULT_COORDX), fontsize_y(CPMA_E_FONTSIZE_DEFAULT_COORDY),
    textalign(CPMA_E_TEXTALIGN_DEFAULT),
    time(CPMA_E_TIME_DEFAULT),
    textstyle(CPMA_E_TEXTSTYLE_DEFAULT),
    color(CPMA_E_COLOR_DEFAULT),
    bgcolor(CPMA_E_BGCOLOR_DEFAULT),
    fade(CPMA_E_COLOR_DEFAULT), // ?
    image(),
    model(),
    usemodel(false),
    angle_pitch(0), angle_yaw(0), angle_roll(0), angle_pan(0)
{
  offset[0] = 0.f; offset[1] = 0.f; offset[2] = 0.f;
}

CPMAElement::CPMAElement( const hsitem_s& def ) :
    ElementBase(def.name, def.desc, def.flags, def.has, def.enable),
    m_type(def.type),
    m_props(),

    m_text(def.text),
    m_icon(def.icon),
    m_ptex(0)
{
}



CPMAElement::~CPMAElement()
{
  cleanup();
}

void CPMAElement::cleanup()
{
  wxDELETE(m_ptex);
  for( std::vector<Texture*>::iterator it = m_weaponlist_tex.begin(); it != m_weaponlist_tex.end(); ++it )
    delete (*it);
  m_weaponlist_tex.clear();
}



bool CPMAElement::parse_property( const wxString& cmd, wxString args )
{ 
  // let parent check on the property
  if (ElementBase::parse_property( cmd, args ))
    return true;

  //m_has = E_PROPERTIES_DEFAULT;

  if( cmd.CmpNoCase(wxT("doublebar")) == 0 )
  {
    if( m_type != E_T_BAR )
      wxLogWarning( _T("Found command `doublebar' which the element `%s' does not support."), m_name.c_str() );
    else
    {
      m_has |= CPMA_E_HAS_DOUBLEBAR;
    }
  }
  else if( cmd.CmpNoCase(wxT("font")) == 0 )
  {
    m_props.font = args;
    m_has |= CPMA_E_HAS_FONT;
  }
  else if( cmd.CmpNoCase(wxT("fontsize")) == 0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    switch( targ.CountTokens() )
    {
    case 1:
      if( 1 != sscanf( targ.GetNextToken().mb_str(), "%i", &m_props.fontsize_pt ) )
      {
        wxLogWarning( _("Unknown `fontsize' argument in element `%s', should be one (pointsize) or two (xy-size) numbers. (defaultvalues set)"), m_name.c_str() );
        m_props.fontsize_pt = CPMA_E_FONTSIZE_DEFAULT_POINT;
      }
      m_props.fontsize_x = CPMA_E_FONTSIZE_DEFAULT_COORDX;
      m_props.fontsize_y = CPMA_E_FONTSIZE_DEFAULT_COORDY;
      m_props.fontsize_type = CPMA_E_FST_POINT;
      m_has |= CPMA_E_HAS_FONTSIZE;
      break;

    case 2:
      if( 1 != sscanf( targ.GetNextToken().mb_str(), "%i", &m_props.fontsize_x ) ||
        1 !=sscanf( targ.GetNextToken().mb_str(), "%i", &m_props.fontsize_y ) )
      {
        wxLogWarning( _("Unknown `fontsize' argument in element `%s', should be one (pointsize) or two (xy-size) numbers. (defaultvalues set)"), m_name.c_str() );
        m_props.fontsize_x = CPMA_E_FONTSIZE_DEFAULT_COORDX;
        m_props.fontsize_y = CPMA_E_FONTSIZE_DEFAULT_COORDY;
      }
      m_props.fontsize_pt = CPMA_E_FONTSIZE_DEFAULT_POINT;
      m_props.fontsize_type = CPMA_E_FST_COORD;
      m_has |= CPMA_E_HAS_FONTSIZE;
      break;

    default:
      wxLogWarning( _("Unknown `fontsize' argument in element `%s', should be one (pointsize) or two (xy-size) numbers. (ignored)"), m_name.c_str() );
      break;
    }  
  }
  else if( cmd.CmpNoCase(wxT("color"))==0 )
  {
    if( !m_props.color.from_string( args ) )
      wxLogWarning( _("Unknown `%s' argument: %s"), "color", args.c_str() );
    m_has |= CPMA_E_HAS_COLOR;
  }
  else if( cmd.CmpNoCase(wxT("bgcolor"))==0 )
  {
    if( !m_props.bgcolor.from_string( args ) )
      wxLogWarning( _("Unknown `%s' argument: %s"), "bgcolor", args.c_str() );
    m_has |= CPMA_E_HAS_BGCOLOR;
  }
  else if( cmd.CmpNoCase(wxT("fade"))==0 )
  {
    if( !m_props.fade.from_string( args ) )
      wxLogWarning( _("Unknown `%s' argument: %s"), "fade", args.c_str() );
    m_has |= CPMA_E_HAS_FADE;
  }
  else if( cmd.CmpNoCase(wxT("time"))==0 )
  {
    sscanf( args.mb_str(), "%i", &m_props.time );
    m_has |= CPMA_E_HAS_TIME;
  }
  else if( cmd.CmpNoCase(wxT("textalign"))==0 )
  {
    sscanf( args.mb_str(), "%c", &m_props.textalign );
    m_props.textalign = toupper( m_props.textalign );
    if( m_props.textalign != 'C' && m_props.textalign != 'L' && m_props.textalign != 'R')
    {
      wxLogWarning( _("Unknown `%c' argument: %s"), "textalign", m_props.textalign );
      m_props.textalign = 'L';
    }
    m_has |= CPMA_E_HAS_TEXTALIGN;
  }
  else if( cmd.CmpNoCase(wxT("textstyle"))==0 )
  {
    sscanf( args.mb_str(), "%i", &m_props.textstyle );
    m_has |= CPMA_E_HAS_TEXTSTYLE;
  }
  else if( cmd.CmpNoCase(wxT("monospace"))==0 )
  {
    m_has |= CPMA_E_HAS_MONOSPACE;
  }
  else if( cmd.CmpNoCase(wxT("fill"))==0 )
  {
    m_has |= CPMA_E_HAS_FILL;
  }
  else if( cmd.CmpNoCase(wxT("draw3d"))==0 )
  {
    m_has |= CPMA_E_HAS_DRAW3D;
  }
  else if( cmd.CmpNoCase(wxT("image"))==0 )
  {
    m_props.image = args;
    she::wxTrim(m_props.image, wxT("\"'"));
    m_has |= CPMA_E_HAS_IMAGE;
  }
  else if( cmd.CmpNoCase(wxT("model"))==0 )
  {
    m_props.model = args;
    if( m_type == E_T_ICON )
    { // model implies draw3d (and there is no model drawn, see data/cpma/docs/README.superhud)
      m_has |= CPMA_E_HAS_DRAW3D;
      m_props.model = wxT("");
    }
    else
    {
      she::wxTrim(m_props.model, wxT("\"'"));
      m_has |= CPMA_E_HAS_MODEL;
    }
    set_usemodel();
  }
  else if( cmd.CmpNoCase(wxT("offset"))==0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    if( targ.CountTokens() != 3 ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_props.offset[E_OFFSET_X]) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_props.offset[E_OFFSET_Y]) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &m_props.offset[E_OFFSET_Z])
        )
      wxLogWarning( _("Unknown `offset' argument in element `%s', should be three numbers (z x y). (ignored)"), m_name.c_str() );
    m_has |= CPMA_E_HAS_OFFSET;
  }
  else if( cmd.CmpNoCase(wxT("angles"))==0 )
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    float pitch = 0.f, yaw = 0.f, roll = 0.f, pan = 0.f;
    if( targ.CountTokens() != 3 && targ.CountTokens() != 4 )
    {
      wxLogWarning( _("Unknown `angles' argument in element `%s', should be three numbers (z x y). (ignored)"), m_name.c_str() );
    }
    else if (
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &pitch) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &yaw) ||
        1 != sscanf(targ.GetNextToken().mb_str(), "%f", &roll)
        )
    {
      wxLogWarning( _("Unknown `angles' argument in element `%s', should be three numbers (z x y). (ignored)"), m_name.c_str() );
    }
    else if( targ.CountTokens() == 1 )
      sscanf(targ.GetNextToken().mb_str(), "%f", &pan); // optional
    m_props.angle_pitch = static_cast<int>(pitch);
    m_props.angle_yaw = static_cast<int>(yaw);
    m_props.angle_roll = static_cast<int>(roll);
    m_props.angle_pan = static_cast<int>(pan);

      
    m_has |= CPMA_E_HAS_ANGLES;
  }
  else
  {
    return false; // not found
  }

  return true;
}


void CPMAElement::write_properties( wxTextOutputStream& stream ) const
{
  ElementBase::write_properties(stream);

  list<wxString> lines;
  if( (m_has & CPMA_E_HAS_FONT)  && !m_props.font.empty() )
    lines.push_back( wxT("font ") + m_props.font );
  if( (m_has & CPMA_E_HAS_TIME) && m_props.time >= 0 )
    lines.push_back(wxString::Format( wxT("time %i"), m_props.time ));
  if( (m_has & CPMA_E_HAS_FONTSIZE) && m_props.fontsize_type == CPMA_E_FST_POINT )
    lines.push_back(wxString::Format( wxT("fontsize %i"),  m_props.fontsize_pt));
  if( (m_has & CPMA_E_HAS_FONTSIZE) && m_props.fontsize_type == CPMA_E_FST_COORD )
    lines.push_back(wxString::Format( wxT("fontsize %i %i"),  m_props.fontsize_x, m_props.fontsize_y ));
  if( monospace() )
    lines.push_back(wxT("monospace"));
  if( (m_has & CPMA_E_HAS_TEXTSTYLE) && m_props.textstyle >= 0 )
    lines.push_back(wxString::Format( wxT("textstyle %i"), m_props.textstyle ));
  if( (m_has & CPMA_E_HAS_TEXTALIGN) && m_props.textalign != ' ' )
    lines.push_back(wxString::Format( wxT("textalign %c"),  m_props.textalign));
  if( m_has & CPMA_E_HAS_COLOR )
    lines.push_back(wxT("color ") + m_props.color.to_string());
  if( m_has & CPMA_E_HAS_BGCOLOR )
    lines.push_back(wxT("bgcolor ") + m_props.bgcolor.to_string());
  if( fill() )
    lines.push_back(wxT("fill"));
  if( doublebar() )
    lines.push_back(wxT("doublebar"));
  if( draw3d() )
    lines.push_back(wxT("draw3d"));
  if( m_has & CPMA_E_HAS_FADE )
    lines.push_back(wxT("fade ") + m_props.fade.to_string());
  if( usemodel() ) //m_has & CPMA_E_HAS_MODEL )
  {
    if( m_has & CPMA_E_HAS_MODEL )
      lines.push_back(wxT("model \"") + m_props.model + wxT("\""));
    if( m_has & CPMA_E_HAS_SKIN )
      lines.push_back(wxT("image \"") + m_props.skin + wxT("\""));
    if( m_has & CPMA_E_HAS_ANGLES )
    {
      wxString angles;
      if( m_props.angle_pan == 0 )
        angles = wxString::Format( wxT("angles %d %d %d"), m_props.angle_pitch, m_props.angle_yaw, m_props.angle_roll );
      else
        angles = wxString::Format( wxT("angles %d %d %d %d"), m_props.angle_pitch, m_props.angle_yaw, m_props.angle_roll, m_props.angle_pan );
      lines.push_back( angles );
    }
    if( m_has & CPMA_E_HAS_OFFSET )
    {
      wxString offset = wxT("offset ") + she::pretty_print_float(m_props.offset[E_OFFSET_X]) + wxT(" ") + 
        she::pretty_print_float(m_props.offset[E_OFFSET_Y]) + wxT(" ") +
        she::pretty_print_float(m_props.offset[E_OFFSET_Z]);
      lines.push_back( offset );
    }
  }
  else if( m_has & CPMA_E_HAS_IMAGE )
    lines.push_back(wxT("image \"") + m_props.image + wxT("\""));


  if( m_flags & E_SHORT )
  {
    for( list<wxString>::const_iterator cit = lines.begin(); cit != lines.end(); ++cit )
      stream << *cit << wxT("; ");
  }
  else
  {
    for( list<wxString>::const_iterator cit = lines.begin(); cit != lines.end(); ++cit )
      stream << wxT("\n  ") << *cit;
  }
}


int CPMAElement::iget_time() const
{
  int t = m_props.time;
  if( !(m_has & CPMA_E_HAS_TIME) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_TIME ));
    if( parent == 0 ) t = CPMA_E_TIME_DEFAULT;
    else t = parent->iget_time();
  }
  return t; 
}
wxString CPMAElement::iget_font() const
{
  wxString f = m_props.font;
  if( !(m_has & CPMA_E_HAS_FONT) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_FONT ));
    if( parent == 0 ) f = CPMA_E_FONT_DEFAULT;
    else f = parent->iget_font();
  }
  return f; 
}
wxChar CPMAElement::iget_textalign() const
{
  wxChar ta = m_props.textalign;
  if( !(m_has & CPMA_E_HAS_TEXTALIGN) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_TEXTALIGN ));
    if( parent == 0 ) ta = CPMA_E_TEXTALIGN_DEFAULT;
    else ta = parent->iget_textalign();
  }
  return ta; 
}
bool CPMAElement::iget_monospace() const
{
  if( m_has & CPMA_E_HAS_MONOSPACE )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_MONOSPACE ));

  return (parent != 0);
}
bool CPMAElement::iget_doublebar() const
{
  if( m_has & CPMA_E_HAS_DOUBLEBAR )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_DOUBLEBAR ));

  return (parent != 0);
}
bool CPMAElement::iget_draw3d() const
{
  if( m_has & CPMA_E_HAS_DRAW3D )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_DRAW3D ));

  return (parent != 0);
}
int CPMAElement::iget_fontsizetype() const
{
  int fst = m_props.fontsize_type;
  if( !(m_has & CPMA_E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_FONTSIZE ));
    if( parent == 0 ) fst = CPMA_E_FST_POINT;
    else fst = parent->iget_fontsizetype();
  }
  return fst; 
}
int CPMAElement::iget_fontsizept() const
{
  int s = m_props.fontsize_pt;
  if( !(m_has & CPMA_E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_FONTSIZE ));
    if( parent == 0 ) s = CPMA_E_FONTSIZE_DEFAULT_POINT;
    else s = parent->iget_fontsizept();
  }
  return s;
}
int CPMAElement::iget_fontsizex() const
{
  int s = m_props.fontsize_x;
  if( !(m_has & CPMA_E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_FONTSIZE ));
    if( parent == 0 ) s = CPMA_E_FONTSIZE_DEFAULT_COORDX;
    else s = parent->iget_fontsizex();
  }
  return s;
}
int CPMAElement::iget_fontsizey() const
{
  int s = m_props.fontsize_y;
  if( !(m_has & CPMA_E_HAS_FONTSIZE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_FONTSIZE ));
    if( parent == 0 ) s = CPMA_E_FONTSIZE_DEFAULT_COORDY;
    else s = parent->iget_fontsizey();
  }
  return s;
}
int CPMAElement::iget_textstyle() const
{
  int ts = m_props.textstyle;
  if( !(m_has & CPMA_E_HAS_TEXTSTYLE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_TEXTSTYLE ));
    if( parent == 0 ) ts = CPMA_E_TEXTSTYLE_DEFAULT;
    else ts = parent->iget_textstyle();
  }
  return ts;
}
Color4 CPMAElement::iget_color() const
{
  Color4 c = m_props.color;
  if( !(m_has & CPMA_E_HAS_COLOR) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_COLOR ));
    if( parent == 0 ) c = CPMA_E_COLOR_DEFAULT;
    else c = parent->iget_color();
  }
  return c;
}
Color4 CPMAElement::iget_bgcolor() const
{
  Color4 c = m_props.bgcolor;
  if( !(m_has & CPMA_E_HAS_BGCOLOR) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_BGCOLOR ));
    if( parent == 0 ) c = CPMA_E_BGCOLOR_DEFAULT;
    else c = parent->iget_bgcolor();
  }
  return c;
}
bool CPMAElement::iget_fill() const
{
  if( m_has & CPMA_E_HAS_FILL )
    return true;

  const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_FILL ));

  return (parent != 0);
}
Color4 CPMAElement::iget_fade() const
{
  Color4 c = m_props.fade;
  if( !(m_has & CPMA_E_HAS_FADE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_FADE ));
    if( parent == 0 ) c = CPMA_E_FADE_DEFAULT;
    else c = parent->iget_fade();
  }
  return c;
}

bool CPMAElement::iget_has(wxInt64 what) const
{
  bool has = (m_has & what) != 0;
  if( !has )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, has ));
    has = (parent != 0);
  }
  return has;
}
wxRect CPMAElement::iget_hudrect() const
{
  const wxRect baser = ElementBase::iget_rect();
  if( E_T_WEAPONLIST != m_type )
    return baser;

  wxRect r = baser; 
  
  const wxChar ta = iget_textalign();
  switch( toupper(ta) )
  {
  case 'C':
    r.width = (r.width+WEAPONLIST_SPACE) * WEAPONLIST_ITEMCOUNT - WEAPONLIST_SPACE;
    r.x -= r.width/2;
    r.y -= r.height/2;
    break;
  case 'L':
  case 'R':
  default:
    {
      int h = wxGetApp().mainframe()->displayctrl()->height();
      r.height = (baser.height+WEAPONLIST_SPACE) * WEAPONLIST_ITEMCOUNT - WEAPONLIST_SPACE;
      // check for wrapping at bottom
      
      if( r.y + r.height - baser.height >= h  && r.y < h )
      {// wrapwrapwrap
        // how many are invisible and have to be wrapped?
        int outside = (int)std::floor(((r.y + r.height + WEAPONLIST_SPACE)-h)/(double)(baser.height+WEAPONLIST_SPACE));
        // height changes to (TOTAL - wrapcount)# items
        // width multiplies by ceil(TOTAL/wrapcount)# itms
        r.height = (WEAPONLIST_ITEMCOUNT - outside) * (baser.height + WEAPONLIST_SPACE) - WEAPONLIST_SPACE;
        r.width = (int)std::ceil(WEAPONLIST_ITEMCOUNT/(double)(WEAPONLIST_ITEMCOUNT - outside)) * (baser.width + WEAPONLIST_SPACE) - WEAPONLIST_SPACE;
      }
    }
    break;
  }
  return r;
}

wxString CPMAElement::iget_image() const
{
  wxString img = m_props.image;
  if( !(m_has & CPMA_E_HAS_IMAGE) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_IMAGE ));
    if( parent == 0 ) img = wxEmptyString;
    else img = parent->iget_image();
  }
  return img;
}
wxString CPMAElement::iget_model() const
{
  wxString m = m_props.model;
  if( !(m_has & CPMA_E_HAS_MODEL) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_MODEL ));
    if( parent == 0 ) m = wxEmptyString;
    else m = parent->iget_model();
  }
  return m;
}
wxString CPMAElement::iget_skin() const
{
  wxString s = m_props.skin;
  if( !(m_has & CPMA_E_HAS_SKIN) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_SKIN ));
    if( parent == 0 ) s = wxEmptyString;
    else s = parent->iget_skin();
  }
  return s;
}
float CPMAElement::iget_offset(int which) const
{
  float d = m_props.offset[which];
  if( !(m_has & CPMA_E_HAS_OFFSET) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_OFFSET ));
    if( parent == 0 ) d = 0.f;
    else d = parent->iget_offset(which);
  }
  return d;
}
void CPMAElement::set_offset( int which, float val )
{
  if( which < E_OFFSET_X || which > E_OFFSET_Z )
    return;
  m_props.offset[which] = val;
}
int CPMAElement::iget_angle(int which) const
{
  int d = angle(which);
  if( !(m_has & CPMA_E_HAS_ANGLES) )
  {
    const CPMAElement *parent = static_cast<const CPMAElement*>(wxGetApp().hudfile()->get_parent( this, CPMA_E_HAS_ANGLES ));
    if( parent == 0 ) d = 0;
    else d = parent->iget_angle(which);
  }
  return d;
}
void CPMAElement::set_angle( int which, int val )
{
  switch(which)
  {
  case E_ANGLE_YAW: m_props.angle_yaw = val; break;
  case E_ANGLE_PITCH: m_props.angle_pitch = val; break;
  case E_ANGLE_ROLL: m_props.angle_roll = val; break;
  case E_ANGLE_PAN: m_props.angle_pan = val; break;
  }
}
int CPMAElement::angle(int which) const
{
  switch(which)
  {
  case E_ANGLE_YAW: return m_props.angle_yaw;
  case E_ANGLE_PITCH: return m_props.angle_pitch;
  case E_ANGLE_ROLL: return m_props.angle_roll;
  case E_ANGLE_PAN: return m_props.angle_pan;
  default:
    break;
  }
  return 0;
}
void CPMAElement::render() const
{
//  wxLogDebug(wxT("RENDER ") + m_name);
  bool hasownbg = true;
  wxRect r = iget_rect();

  switch( m_type )
  {
  case E_T_BAR:
    {
      Color4 color =  iget_color();
      color.glBind();
      if( iget_doublebar() )
      {
        wxRect top(r), bottom(r);
        top.height = top.height/2-2;
        bottom.height = top.height;
        bottom.y += bottom.height+4;
        she::draw_rect(top);
        she::draw_rect(bottom);
      }
      else
        she::draw_rect(r);
    }
    break;

  case E_T_USERICON:
    { // COLOR_T && FILL = border around ...
      bool fill = iget_fill();
      bool hasimage = (!iget_image().empty() && !m_props.usemodel);
      Color4 bgcolor = iget_bgcolor();
      Color4 color = iget_color();
      if( color.is_special() )
      {
        bgcolor.set_type( color.get_type() );
        color.set_a1( bgcolor.a1() );
      }

      if( fill && bgcolor.a1() != 0.f )
      {
        bgcolor.glBind();
        she::draw_rect(r);
      }
      if( !usemodel() )
      {
        if( m_ptex )
        {
          color.glBind();
          glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
          glEnable(GL_TEXTURE_2D);
          if( m_ptex->is_ok() )
            m_ptex->glBind();
          else
            wxGetApp().mainframe()->displayctrl()->texture_default()->glBind();
          she::draw_rect( r, true );
          glDisable(GL_TEXTURE_2D);
        }
      }
      else // usemodel
      {
        color.glBind();
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glEnable(GL_TEXTURE_2D);
        wxGetApp().mainframe()->displayctrl()->texture_model()->glBind();
        she::draw_rect( r, true );
        glDisable(GL_TEXTURE_2D);
      }
    }
    break;

  case E_T_ICON:
    {
      Color4 bgcolor = iget_bgcolor();
      Color4 color = iget_color();
      if( color.is_special() )
      {
        bgcolor.set_type( color.get_type() );
        color = CPMA_E_COLOR_DEFAULT;
      }
      color.set_a1(1.f); // NB for eg StatusBar_ArmorIcon the color would be completely discarded.

      if( iget_fill() )
      {
        bgcolor.glBind();
        she::draw_rect(r);
      }
      if( m_ptex && m_ptex->is_ok() )
      {
        color.glBind();
        glEnable(GL_TEXTURE_2D);
        m_ptex->glBind();
        she::draw_rect( r, true );
        glDisable(GL_TEXTURE_2D);
      }
    }
    break;


  case E_T_TEXT:
    { // COLOR_T && FILE = border around ...
      int sx = CPMA_E_FONTSIZE_DEFAULT_COORDX, sy = CPMA_E_FONTSIZE_DEFAULT_COORDX; // some safe default values
      int fst = iget_fontsizetype();

      wxString text = m_text;
      bool monospace = iget_monospace();
      IFont *font = wxGetApp().mainframe()->displayctrl()->font( iget_font() );
      if( !font )
        wxLogDebug(wxT("Font not found: ") + iget_font());
      wxRect rr(r);
      Color4 bgcolor = iget_bgcolor();
      Color4 color = iget_color();
      if( color.is_special() )
      {
        bgcolor.set_type( color.get_type() );
        color = CPMA_E_COLOR_DEFAULT;
      }

      switch( fst )
      {
      case CPMA_E_FST_POINT:
        sx = iget_fontsizept();
        
        break;
      case CPMA_E_FST_COORD:
        sx = iget_fontsizex();
        sy = iget_fontsizey();
        break;
      }

      if( !iget_fill() )
      {
        
        int w = (font ? font->get_width(sx, text, monospace) : 0);
        switch( iget_textalign() )
        {
          case 'C':
            rr.x = rr.x + (rr.width - w)/2 ;
            break;
          case 'R':
            rr.x = rr.x + rr.width - w;
            break;
        }
        rr.width = w;
      }

      if( rr.width && rr.height && bgcolor.a1() != 0.f )
      {
        bgcolor.glBind();
        she::draw_rect(rr);
      }

      if( font && !text.empty())
      {
        color.glBind();
        switch( fst )
        {
        case CPMA_E_FST_POINT:
          font->print( r, sx, text, monospace, iget_textalign() );
          break;

        case CPMA_E_FST_COORD:
          // sx is always 12!
          font->print( r, sx, sy, text, monospace, iget_textalign() );
          break;
        }
      }
    }
    break;
  case E_T_WEAPONLIST:
    {
      // te
      // we need the real rectangle, not adapted..
      wxRect rr = iget_rect();
      wxRect hudr = iget_hudrect();
      
      IFont *font = wxGetApp().mainframe()->displayctrl()->font( iget_font() );
      Color4 bgcolor = iget_bgcolor();
      Color4 color = iget_color();
      bool monospace = iget_monospace();
      wxChar textalign = toupper(iget_textalign());

      int x = hudr.x;
      int y = hudr.y;
      wxRect itemr; // the rect of the current item
      wxRect iconr; // the rect of the current icon
      wxRect textr; // the rect of the text to print
      for( size_t i=0; i < WEAPONLIST_ITEMCOUNT && i < m_weaponlist_tex.size(); ++i )
      {
        // no wrapping for center aligned or a weaponlist that is completely out of bounds (outside of hudview)
        int h = wxGetApp().mainframe()->displayctrl()->height();
        if( textalign != 'C' && hudr.y < h-1  )
        { // need some wrapping?
          if( y >= h )
          {
            y = hudr.y;
            x += rr.width + WEAPONLIST_SPACE;
          }
        }
        itemr = rr; // w&h
        itemr.x = x;
        itemr.y = y;
        if( WEAPONLIST_SELECTEDIDX == i )
          color.glBind();
        else
          bgcolor.glBind();
        she::draw_rect(itemr);

        glColor4f(1.f, 1.f, 1.f, 1.f);
        
        iconr = itemr;
        switch(iget_fontsizetype())
        {
        case CPMA_E_FST_COORD:
          iconr.width = iget_fontsizex();
          iconr.height = iget_fontsizey();
          break;
        case CPMA_E_FST_POINT: 
        default:
          iconr.width = iget_fontsizept();
          iconr.height = iconr.width;
          break;
        }
        if( textalign == 'R' )
          iconr.x = itemr.x + itemr.width - iconr.width;

        if( m_weaponlist_tex[i]->is_ok() )
        {
          glEnable(GL_TEXTURE_2D);
          m_weaponlist_tex[i]->glBind();
          she::draw_rect(iconr, true);
          glDisable(GL_TEXTURE_2D);
        }
        
        if( textalign == 'R' )
        {
          textr = itemr;
          textr.width = textr.width - iconr.width - WEAPONLIST_SPACE;
        }
        else // ta == 'C' || ta == 'L'
        {
          textr = itemr;
          textr.x = itemr.x + iconr.width + WEAPONLIST_SPACE;
          textr.width = itemr.x + itemr.width - textr.x;
        }
        if( !font )
          wxLogDebug(wxT("Font not found: ") + iget_font());
        else 
          font->print(textr, iconr.width, iconr.height, wxString::Format(wxT("%d"),100),  monospace, textalign); // rnadom nr ((i*17)%10+20)*3)

        if( textalign == 'C' )
          x += rr.width + WEAPONLIST_SPACE;
        else
          y += rr.height + WEAPONLIST_SPACE;
      }
    }
    break;
  }
}

void CPMAElement::postparse()
{
  if( usemodel() )
  { // we have a model, weeeeh.
    remove_has(CPMA_E_HAS_IMAGE);
    if( !m_props.image.empty() )
    {
      add_has(CPMA_E_HAS_SKIN); 
      m_props.skin = m_props.image; // hence the image is actually the skin    
      m_props.image = wxT(""); // and no longer an image.. :x
    }
  }
  else if( m_type == E_T_WEAPONLIST )
  {
    wxGetApp().mainframe()->displayctrl()->SetCurrent();
    // these give a small memleak
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_gauntlet.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_machinegun.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_shotgun.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_grenade.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_rocket.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_lightning.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_railgun.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_plasma.tga"), PM_SEARCH_HUDFILE) );
    m_weaponlist_tex.push_back( new Texture(wxT("icons/iconw_bfg.tga"), PM_SEARCH_HUDFILE) );
  }
}


void CPMAElement::prerender()
{
  switch(m_type)
  {
  case E_T_ICON:
    wxGetApp().mainframe()->displayctrl()->SetCurrent();
    if( !m_ptex && !m_icon.empty() )
      m_ptex = new Texture(m_icon, PM_SEARCH_HUDFILE);
    break;
  case E_T_USERICON:
    if( !m_props.image.empty() && !usemodel() )
    {
      wxGetApp().mainframe()->displayctrl()->SetCurrent();
      if( !m_ptex )
      {
        m_ptex = new Texture( m_props.image, PM_SEARCH_HUDFILE );
      }
      else if( m_props.image.Cmp(m_ptex->name()) != 0 )
      {
        m_ptex->load( m_props.image, PM_SEARCH_HUDFILE );
      }
    }
    if( m_props.image.empty() && m_ptex )
      wxDELETE(m_ptex);
    break;
  }
}

void CPMAElement::copy_from( const ElementBase * const el )
{
  ElementBase::copy_from(el);

  const CPMAElement * const cel = static_cast<const CPMAElement * const>(el);
  m_props = cel->properties();
}

void CPMAElement::reset()
{
  ElementBase::reset();
  m_props = Properties();
}

void CPMAElement::convert( double from, double to, bool size, bool stretchposition, bool fontsize)
{
  ElementBase::convert( from, to, size, stretchposition, fontsize );

  double ratio = from/to;
  if( fontsize && (m_has & CPMA_E_HAS_FONTSIZE) )
  {
    int sx = CPMA_E_FONTSIZE_DEFAULT_COORDX;
    int sy = CPMA_E_FONTSIZE_DEFAULT_COORDY;

    int fst = fontsizetype();

    switch( fst )
    {
      case CPMA_E_FST_POINT:
        sx = fontsizept();
        sy = fontsizept();
        break;
      case CPMA_E_FST_COORD:
        sx = fontsizex();
        sy = fontsizey();
        break;
    }

    sx  = (int)(sx*ratio);
    set_fontsizetype(CPMA_E_FST_COORD);
    set_fontsizex(sx);
    set_fontsizey(sy);
  }
}

