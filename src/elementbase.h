#ifndef ELEMENTBASE_H
#define ELEMENTBASE_H

#include "common.h"

#include "varcontainer.h"

#include <wx/string.h>
#include <wx/txtstrm.h>
#include <wx/gdicmn.h>


typedef enum
{
  E_HAS_NONE = 0,
  E_HAS_POS = 1<<0,
  E_HAS_DIM = 1<<1,
  E_HAS_RECT = E_HAS_POS | E_HAS_DIM
} eElementProperties;

typedef enum
{
  E_T_UNKNOWN = 0,
  /// CPMA : the game will give us text to render
  /// Q4MAX:      "
  E_T_TEXT, 
  /// CPMA : not available
  /// Q4MAX: user defined text (Pre/PostDecorate_Text only!)
  E_T_USERTEXT,

  /// the game will override the IMAGE.
  E_T_ICON, 
  /// CPMA : more or less a general element where we can specify everything.
  /// Q4MAX: user defined image
  E_T_USERICON, 
  /// CPMA : ammo bar, health bar, armor bar
  /// Q4MAX: percentage bars
  E_T_BAR, 
  E_T_WEAPONLIST,
} eElementType;
 


typedef enum {
  E_NONE = 0,
  E_NOTUNIQ  = 1<<0,  ///< the name is allowed more than once.
  E_NODEFAULT  = 1<<1, ///< upon File->New this item is not added to the item list.
  E_PARENT = 1<<2, ///< is an inheriter.
  E_NOINHERIT = 1<<3, ///< this item does not inherit from previous !DEFAULT items.
  E_DRAWNEVER = 1<<4, ///< never draw the item no matter if it's enabled or not.
  E_DRAWBACK = 1<<5,
  E_DRAWFRONT = 1<<6,
  E_DRAWALWAYS = 1<<7, ///< NOTINUSE
  E_SHORT = 1<<8, ///< while saving writes a oneliner instead of each property on one line.

  /// cannot be disabled.
  E_ENABLEALWAYS = 1<<9

} eElementFlags;

/// Defaults
/// @{
const int E_PROPERTIES_DEFAULT = E_HAS_NONE;
const wxRect E_RECT_DEFAULT = wxRect( 0, 0, 64, 32 ); // (0,0) verified 1.35
/// @}

/// posible argument delimiters found in the hudfile.
/// NOTE: this meaning here changed... we 'fix' the delimiters 
/// (which can be ' ' or ',' previously) so while parsing we only
/// have single-space as delimiter.
const wxString HF_PROPERTY_ARG_DELIM = wxT(" ");

class ElementBase
{
   friend class HudFileBase; // for proper detection if item has already been read (through m_enabled as is_enabled() is lying)
   friend class CPMAPropertiesCtrl; // TODO still needed?
   friend class CPMAVisibilityPropertiesCtrl; // TODO still needed?
   friend class Q4MAXVisibilityPropertiesCtrl; // TODO still needed?
  public:
    ElementBase( const wxString& name, const wxString& desc = wxT(""), int flags = E_NONE, wxInt64 has = 0, bool enabled = false, 
      const wxRect& rect = E_RECT_DEFAULT ) :
      m_name(name), m_desc(desc), m_flags(flags), m_has(has), m_enabled(enabled), m_rect(rect)
    {}
    virtual ~ElementBase() {}

    /// while reading the hudfile we pass single properties here.
    /// @arg cmd The name of the property.
    /// @arg args The rest of the propertyline, the arguments.
    /// @return True if the property was parsed otherwise false.
    virtual bool    parse_property( const wxString& cmd, wxString args );
    virtual void    write_properties( wxTextOutputStream& stream ) const;

    /// is called after parsing finished for this element
    virtual void    postparse() {}

    /// is called everytime before rendering
    virtual void prerender() {}

    /// draw on view
    virtual void render() const {}

    /// copy properties from foreign element
    virtual void copy_from( const ElementBase * const el );

    /// resets all properties to default
    virtual void reset();

    /// @arg from     Current aspectratio X/Y
    /// @arg to       New aspectratio X/Y
    /// @arg size     Whether to adapt size(width) -> makes squares stay squares
    /// @arg stretchposition Whether to stretch positions to make corner elements stay in the corner
    virtual void convert( double from, double to, bool size, bool stretchposition, bool fontsize);
 
    void            move_to( const wxPoint& p ) { m_rect.SetPosition(p); }
    /// moving by offset
    void            move( const wxPoint& p ) { m_rect.x += p.x; m_rect.y += p.y; }

    void            resize_to( const wxSize& s ) { m_rect.SetSize(s); }
    /// resizing by offset
    void            resize( const wxSize& s ) { m_rect.width += s.GetWidth(); m_rect.height += s.GetHeight(); }

    // get&set
    const wxString& name() const { return m_name; }
    int             flags() const { return m_flags; }
    const wxString& desc() const { return m_desc; }
    wxInt64             has() const { return m_has; }
    /// adds a value (bitmask) to what this element overwrite
    /// @arg bool add If false we actually remove it.
    void            add_has( wxInt64 has, bool add = true ) { if( !add) remove_has(has); else m_has |= has; }
    void            remove_has( wxInt64 has ) { m_has &= ~has; }
    bool            is_enabled() const { return (m_flags & E_ENABLEALWAYS ? true : m_enabled); }
    void            set_enabled(bool en = true) { m_enabled = en; }
    bool            is_selected() const;
    wxRect          rect() const { return m_rect; }
    void            set_rect( const wxRect& r ) { m_rect = r; }
    void            set_pos( const Vec2& p ) { m_rect.x = p.x; m_rect.y = p.y; }
    void            set_dim( const Vec2& d ) { m_rect.width = d.x; m_rect.height = d.y; }

    // get&set properties (with regard to `i'nheritance)
    /// this gives back the space the element occupies on the hud, shouldn't be confused with
    /// the one displayed in properties or written to hudfile (for most it's the same but for
    /// example WeaponList it isn't
    virtual wxRect  iget_hudrect() const { return iget_rect(); }
    wxRect          iget_rect() const;
    Vec2            iget_pos() const;
    Vec2            iget_dim() const;
    virtual bool    is_rendered() const;
    virtual bool    is_removable() const { return (flags() & E_NOTUNIQ) != 0; }

    static wxString type2string( int type );
  protected:
    wxString  m_name; ///< is not unique
    wxString  m_desc;
    int       m_flags;
    wxInt64       m_has;

    // props for all items
    bool     m_enabled;
    wxRect   m_rect;
};

#endif // ELEMENTBASE_H

