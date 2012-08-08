// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "easyviewwidget.h"
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QComboBox>

#include <ZenLib/Ztring.h>
using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

const size_t Title_Pos=94;

EasyViewWidget::EasyViewWidget(Core* C, QWidget *parent) : QWidget(parent)
{
    this->C=C;
    FilePos=0;
    C->Menu_View_Easy();

    refreshDisplay();
}

void EasyViewWidget::refreshDisplay() {
    if(this->layout()) {
        qDeleteAll(Boxes);
        Boxes.clear();
        delete this->layout();
    }

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QComboBox* fileChoice = new QComboBox();
    layout->addWidget(fileChoice);
    for (size_t Pos=0; Pos<C->Count_Get(); Pos++)
        fileChoice->addItem( wstring2QString(C->Get(Pos, Stream_General, 0, __T("CompleteName"))), (int)Pos);

    fileChoice->setCurrentIndex(FilePos);

    connect(fileChoice,SIGNAL(currentIndexChanged(int)),SLOT(changeFilePos(int)));

    QFrame *box;
    QGroupBox *subBox;
    for (size_t StreamPos=0; StreamPos<Stream_Max; StreamPos++) {
        bool addBox = false;
        box = new QFrame();
        QHBoxLayout* boxLayout = new QHBoxLayout();
        box->setLayout(boxLayout);
        for (size_t Pos=0; Pos<Boxes_Count_Get(StreamPos); Pos++) {
            subBox = createBox((stream_t)StreamPos,(int)Pos);
            if(subBox!=NULL) {
                boxLayout->addWidget(subBox);
                addBox = true;
            }
        }
        if(addBox) {
            layout->addWidget(box);
            Boxes.push_back(box);
        }

    }
    layout->addStretch();
}

QGroupBox* EasyViewWidget::createBox(stream_t StreamKind, int StreamPos) {

    QString Temp="";
    Temp.append(wstring2QString(C->Summary_Get(FilePos,StreamKind, StreamPos)));

    //Depend of StreamKind
    if (C->Count_Get(FilePos,Stream_General)==0)
    {
        //Empty box
        Temp+="\n";
        Temp+="\n";
    }
    else if (StreamKind==Stream_General)
    {
        //Stream_General
        size_t Lines=1;
        for (int KindOfStream=1; KindOfStream<Stream_Max; KindOfStream++)
        {
            if (C->Count_Get(FilePos, (stream_t)KindOfStream)>0)
            {
                String Z1=String(C->Get(FilePos, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Measure))+__T("StreamCount");
                String Z2=String(__T(" "))+C->Get(FilePos, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text);
                if (C->Count_Get(FilePos, (stream_t)KindOfStream)==1)
                    Z2+=__T(" stream");//__T(" stream1");
                else
                    Z2+=__T(" streams");//__T(" stream2");
                String Z3=String(C->Get(FilePos, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text)+__T("_Format_WithHint_List"));
                Temp+="\n";
                Temp+=wstring2QString((C->Get(FilePos, (stream_t)KindOfStream, 0, Z1)+Z2+__T(": ")+C->Get(FilePos, Stream_General, 0, Z3)));
                Lines++;
            }

            if (Lines>=Lines_Count_Get(StreamKind))
                KindOfStream=Stream_Max;
        }
    }
    else if (Lines_Count_Get(StreamKind)>1)
    {
        //Other than Stream_General
        QString Title = QString(wstring2QString(C->Get(FilePos, StreamKind, StreamPos, __T("Title"))));
        if (!Title.isEmpty())
        {
            Temp+="\n";
            Temp+=Title;
        }
    }

    if(Temp.isEmpty())
        return NULL;

    QGroupBox* box = new QGroupBox(Title_Get(StreamKind));
    if(StreamKind==Stream_General)
        box->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    else
        box->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
    QHBoxLayout* boxLayout = new QHBoxLayout();
    box->setLayout(boxLayout);
    QLabel* label = new QLabel(Temp);
    label->setWordWrap(true);
    boxLayout->addWidget(label);

    if(StreamKind==Stream_General) {
        QLabel* labelTags = new QLabel(Tags_Get_General());
        labelTags->setWordWrap(true);
        boxLayout->addWidget(labelTags);
    }

    return box;
}

//---------------------------------------------------------------------------
QString EasyViewWidget::Tags_Get_General()
{
    QString Temp;

    size_t MI_Pos=Title_Pos; //Should begin with "Title"
    size_t MI_Max=C->Count_Get(FilePos, Stream_General, 0);
    size_t Lines=0;

    while(MI_Pos<MI_Max)
    {
        String Z1=C->Get(FilePos, Stream_General, 0, MI_Pos);
        String Options=C->Get(FilePos, Stream_General, 0, MI_Pos, Info_Options);
        if (Options.size()>InfoOption_ShowInInform && Options[InfoOption_ShowInInform]==__T('Y') && Z1.size()>0)
        {
            String Z2=C->Get(FilePos, Stream_General, 0, MI_Pos, Info_Name_Text);
            if (Z2.size()==0)
                Z2=C->Get(FilePos, Stream_General, 0, MI_Pos, Info_Name);
            Z2+=__T(": ");
            Z2+=Z1;
            if (Lines>0)
                Temp+="\n";
            Temp.append(wstring2QString(Z2));

            Lines++;
            if (Lines>=Lines_Count_Get(Stream_General))
                MI_Pos=MI_Max;
        }
        MI_Pos++;
    }

    return Temp;
}

//---------------------------------------------------------------------------
size_t EasyViewWidget::Lines_Count_Get(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case 0 : return 5;
        case 1 : return 2;
        case 2 : return 2;
        case 3 : return 2;
        default: return 0;
    }
}

//---------------------------------------------------------------------------
size_t EasyViewWidget::Boxes_Count_Get(size_t StreamKind)
{
    switch (StreamKind)
    {
        case 0 : return 1;
        case 1 : return 1;
        case 2 : return 2;
        case 3 : return 3;
        default: return 0;
    }
}

QString EasyViewWidget::Title_Get(stream_t StreamKind)
{
    QString title;
    switch (StreamKind)
    {
        case Stream_General : title="General"; break;
        case Stream_Video   : title="Video";   break;
        case Stream_Audio   : title="Audio";   break;
        case Stream_Text    : title="Text";    break;
        default             : title="";
    }
    return title;
}

void EasyViewWidget::changeFilePos(int newFilePos) {
    FilePos = newFilePos;
    refreshDisplay();
}
