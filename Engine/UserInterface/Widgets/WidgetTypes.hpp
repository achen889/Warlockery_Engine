//==============================================================================================================
//WidgetTypes.hpp
//by Albert Chen May-8-2016.
//==============================================================================================================

#pragma once

#ifndef _included_WidgetTypes__
#define _included_WidgetTypes__

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///include widget types

#include "ButtonWidget.hpp"
#include "ProgressBarWidget.hpp"
#include "CheckBoxWidget.hpp"
#include "EditLineWidget.hpp"
#include "GroupWidget.hpp"
#include "LayoutWidget.hpp"
#include "CursorWidget.hpp"
#include "DropDownWidget.hpp"
#include "SliderWidget.hpp"


//macro for dynamic initialization of a widget type
#define USING_WIDGET_REGISTRATION(name) WidgetRegistration widgetReg ## name(#name, &name::CreateWidget, &name::CreateWidgetXML)


//===========================================================================================================

#endif //__includedWidgetTypes__

