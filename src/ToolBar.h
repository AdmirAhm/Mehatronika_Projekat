//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include "TBSliderView.h"

class ToolBar : public gui::ToolBar
{
    using Base = gui::ToolBar;
protected:
    gui::Image _imgExport;
    gui::Image _imgImport;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 2, 2)
    , _imgExport(":export")
    , _imgImport(":import")
    {
        td::WORD i = 0;
        initItem(i++, tr("export"), &_imgExport, tr("exportTT"), 20, 0, 0, 10);
        initItem(i++, tr("import"), &_imgImport, tr("impoetTT"), 20, 0, 0, 20);
    }
    
    
};
