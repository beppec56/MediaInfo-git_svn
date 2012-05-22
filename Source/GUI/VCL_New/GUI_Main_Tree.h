// GUI_Main_Tree - VCL GUI for MediaInfo
// Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Borland VCL GUI for MediaInfo, Tree Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Tree_H
#define GUI_Main_Tree_H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <map>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Tree_
//***************************************************************************

class GUI_Main_Tree_ : public TTreeView
{
public:
    //Constructor/Destructor
    GUI_Main_Tree_(Core* C, TComponent* Owner);
    __fastcall ~GUI_Main_Tree_();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();

private:
    GUI_Main_Common_Core* T;
};

//***************************************************************************
// GUI_Main_Tree
//***************************************************************************

class GUI_Main_Tree : public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Tree(Core* C, TWinControl* Owner);
    ~GUI_Main_Tree();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();
    void Item_Show(TTreeNode* Item);

    //Events
    //void OnItemExpanding(wxTreeEvent& event);
    //void OnItemCollapsing(wxTreeEvent& event);

    //Temp
    std::map<TTreeNode*, size_t> TextPos;
    MediaInfoLib::String Text;

private:
    TTreeView* VCL;
};

#endif
