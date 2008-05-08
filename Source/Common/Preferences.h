// Preferences - Manage preferences of MediaInfo
// Copyright (C) 2002-2004 Jerome Martinez, Zen@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Manage preferences of MediaInfo
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef PreferencesH
#define PreferencesH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <ZenLib/ZtringListListF.h>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// For Unicode management
// Here only because this is useful in all MediaInfo
#ifdef _UNICODE
    static std::wstring String_Void;
    #define GUI_Text(A) ((A).Length()>0?(A).c_bstr():String_Void.c_str())
#else
    static std::string String_Void;
    #define GUI_Text(A) (A?Ztring().From_Unicode(A.c_bstr()).c_str():String_Void.c_str())
#endif //_UNICODE
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#define MediaInfo_Version_GUI _T("0.7.7.0BETA")
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
enum List_t
{
    Language,           //List of words in the good language ("Name;Translation")
    Sheet,              //Customized Sheet ("StreamKind|Name;Percentage of view")
    Tree,               //Customized Tree  ("StreamKind|Name;Y or N")
    Custom,             //Customized Text  ("StreamKind;Text for this streamKind")
    Language_English,   //List of words in English ("Name;Translation")
    Language_List,      //List of language translation in their language("Name;Translation")
    List_Max,
};

//---------------------------------------------------------------------------

class Preferences
{
public:
    Preferences();

    //Infos
    ZenLib::Ztring          BaseFolder; //Folder of the Configuration files
    ZenLib::ZtringListListF Config;     //Configuration file
    ZenLib::ZtringList      FilesList[List_Max];  //List of available files for an Item
    ZenLib::ZtringListListF Details[List_Max];

    //Useful functions about preferences
    int Config_Create       ();
    int Config_Load         (); //Load the configuration, or create it if no exist. 1=Loaded, 2=Created
    int Config_Save         ();
    int Config_Remove       ();
    int RefreshFilesList    (List_t KindOfList);
    int Create              (List_t KindOfList, const ZenLib::Ztring &Name);
    int Copy                (List_t KindOfList, const ZenLib::Ztring &From, const ZenLib::Ztring &To);
    int Load                (List_t KindOfList, const ZenLib::Ztring &Name);
    int Remove              (List_t KindOfList, const ZenLib::Ztring &Name);

    //Related to user preferences - Action only if Preferences are OK
    int InternetCheck       (); //Test if there is new update
    int ExplorerShell       (); //To be availble with right clic in Explorer
    int ShellToolTip        (); //To be available with mouse in Explorer

    //Special things
    ZenLib::Ztring &Translate (ZenLib::Ztring Name); //return in the good language, or in English if not found

    //Info abou Lists
    ZenLib::ZtringList FolderNames;  //List of list_t --> Foldernames
    ZenLib::ZtringList DefaultNames; //List of list_t --> Default Filenames for a kind
};

//General preference for the whole program
extern Preferences* Prefs;

#endif
