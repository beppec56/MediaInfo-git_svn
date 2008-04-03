//---------------------------------------------------------------------------
#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <wx/file.h>
#include <ZenLib/File.h>
#include <ZenLib/ZtringListListF.h>
#include "PreRelease/Ressources.h"
using namespace ZenLib;
using namespace std;
#define FILE_MAX 20000
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Open a file
ZenLib::Ztring Ressources_Create_Load(Ztring FileName, Ztring &Contents)
{
    wxFile F;
    if (F.Open(FileName.c_str(), wxFile::read)==false)
    {
        Ztring ToReturn=L"Problems to open ";
        ToReturn+=FileName;
        ToReturn+=L"\r\n";
        return ToReturn;
    }

    char C[FILE_MAX];
    size_t Size=F.Read(C, FILE_MAX-1);
    Contents.From_Local(C, Size);
    return L"";
}

//---------------------------------------------------------------------------
// Write a file
ZenLib::Ztring Ressources_Create_Save(Ztring FileName, Ztring &Contents)
{
    File F;
    if (F.Create(FileName.c_str(), true)==false)
    {
        Ztring ToReturn=L"Problems to create ";
        ToReturn+=FileName;
        ToReturn+=L"\r\n";
        return ToReturn;
    }

    std::string S1=Contents.To_UTF8();
    size_t Size=F.Write((const int8u*)S1.c_str(), S1.size());
    Contents.From_Number(Size);
    Contents+=L" bytes written";
    return L"";
}

//---------------------------------------------------------------------------
// Open an item
ZenLib::Ztring Ressources_Create_Item(const Ztring &Directory, const Ztring &Name, Ztring &Contents)
{
    Contents.clear();
    Ztring Result;

    //Load header
    Ztring Partial;
    Result=Ressources_Create_Load(L"../Source/Ressource/Text/_.2.txt", Partial);
    if (!Result.empty())
        return Result;
    Partial.FindAndReplace(L"%Name%", Ztring(L"File__Base_")+Name);
    if (Name==_T("Codec"))
        Partial.FindAndReplace(L"%Class%", L"InfoMap");
    else if (Name==_T("DefaultLanguage"))
        Partial.FindAndReplace(L"%Class%", L"Translation");
    else if (Name==_T("Iso639"))
        Partial.FindAndReplace(L"%Class%", L"InfoMap");
    else if (Name==_T("Encoder"))
        Partial.FindAndReplace(L"%Class%", L"InfoMap");
    else if (Name==_T("Format"))
        Partial.FindAndReplace(L"%Class%", L"InfoMap");
    else
        Partial.FindAndReplace(L"%Class%", L"ZtringListList");
    Contents+=Partial;

    //Load line template
    Ztring Line;
    Result=Ressources_Create_Load(L"../Source/Ressource/Text/_.5.txt", Line);
    if (!Result.empty())
        return Result;

    //Read input file
    ZtringListListF ZLL;
    ZLL.Load(Ztring(L"../Source/Ressource/Text/")+Directory+L"/"+Name+L".csv");
    for (size_t Pos=0; Pos<ZLL.size(); Pos++)
    {
        Ztring Line_Temp=Line;
        Line_Temp.FindAndReplace(L"%Line%", ZLL.Read(Pos));
        Contents+=Line_Temp;
    }

    //Load footer template
    Result=Ressources_Create_Load(L"../Source/Ressource/Text/_.8.txt", Partial);
    if (!Result.empty())
        return Result;
    Contents+=Partial;

    return L"";
}

//---------------------------------------------------------------------------
// Main
ZenLib::Ztring Ressources_Create()
{
    Ztring Out;
    Ztring Result;
    Ztring Contents;

    //Load header
    Result=Ressources_Create_Load(L"../Source/Ressource/Text/_.1.txt", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Load datas
    Result=Ressources_Create_Item(L"Language", L"DefaultLanguage", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"DataBase", L"Format", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"DataBase", L"Codec", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"Stream", L"General", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"Stream", L"Video", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"Stream", L"Audio", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"Stream", L"Text", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"Stream", L"Chapters", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"Stream", L"Image", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"Stream", L"Menu", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"DataBase", L"Iso639", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"DataBase", L"Encoder", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Ressources_Create_Item(L"MediaInfo", L"Summary", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Load footer
    Result=Ressources_Create_Load(L"../Source/Ressource/Text/_.9.txt", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Write file
    Result=Ressources_Create_Save(L"../Source/MediaInfo/File__Base_Automatic.cpp", Out);
    if (!Result.empty())
        return Result;

    return Out;

}
//---------------------------------------------------------------------------

