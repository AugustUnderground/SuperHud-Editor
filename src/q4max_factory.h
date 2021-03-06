#ifndef Q4MAXFACTORY_H
#define Q4MAXFACTORY_H

#include "factorybase.h"
#include "prefs.h"

class Q4MAXFactory : public FactoryBase
{
  public:
    ElementsCtrlBase*   create_elementsctrl( wxWindow *parent ) const;
    PropertiesNotebookBase* create_propertiesnotebook( wxWindow *parent ) const;
    DisplayCtrlBase*    create_displayctrl( wxWindow *parent ) const;
    HudFileBase*        create_hudfile() const;

    wxString            gamename() const { return wxT("Quake4"); }
    wxString            modname() const { return wxT("Q4MAX"); }
    wxString            dirname_gamedata() const { return wxT("q4base"); }
    wxString            dirname_moddata() const { return wxT("q4max"); }

    // -- access prefs
    wxString            dir_game() const { return Prefs::get().var(wxT("q4_gamedir")).sval(); }
    void                set_dir_game( const wxString& dir ) const { Prefs::get().set(wxT("q4_gamedir"), dir); }
    wxString            pakfiles() const { return Prefs::get().var(wxT("q4_pakfiles")).sval(); }
    wxString            filedialog_path() const { return Prefs::get().var(wxT("q4_filedialog_path")).sval(); }
    void                set_filedialog_path( const wxString& path ) { Prefs::get().set(wxT("q4_filedialog_path"), path); }
    wxString            startup_loadfile() const { return Prefs::get().var(wxT("q4_startup_loadfile")).sval(); }
    void                set_startup_loadfile( const wxString& file ) { Prefs::get().set(wxT("q4_startup_loadfile"), file); }
    wxString            background() const { return Prefs::get().var(wxT("q4_background")).sval(); }
    wxString            hudspecs() const { return Prefs::get().var(wxT("q4_hudspecs")).sval(); }
#ifndef WIN32
    wxString            unixdirname_userdata() const { return Prefs::get().var(wxT("q4_homedirname")).sval(); }
#endif

#ifdef WIN32
    wxString            filename_gamebin() const { return wxT("quake4.exe"); }
#else // unix
    wxString            filename_gamebin() const { return wxT("quake4.x86"); }
#endif

    virtual wxString wikiurl() const { return wxT("http://q4max.com/wiki/index.php/SuperHud"); }

};

#endif
