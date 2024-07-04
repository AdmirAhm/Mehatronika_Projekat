//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewMehatronika.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewMehatronika _viewCubeMap;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    {
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
//        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
//    
        setToolBar(_toolBar);
        setCentralView(&_viewCubeMap);
    }
    
protected:
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();
        if (menuID == 20)
        {
            if (actionID == 10)
            {
                //export to image
                _viewCubeMap.saveXML();
                return true;
            }
            if (actionID == 20)
            {
                //export to image
                _viewCubeMap.readXML();
                return true;
            }
        }
        return false;
    }
    
    
};
