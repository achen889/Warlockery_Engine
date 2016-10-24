//==============================================================================================================
//UISystem.cpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#include "UISystem.hpp"

#include "WidgetStates\WidgetStateProperties.hpp"
#include "WidgetStyleFactory.hpp"

#include "UICommon.hpp"
#include "..\Core\ObjectFactory.hpp"
#include "..\Core\BaseGameState.hpp"

UISystem* theUISystem = NULL;

ObjectFactory<BaseWidget>* UISystem::s_defaultWidgetFactory = NULL;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

UISystem::UISystem(){
	
	if (!theUISystem) {
		theUISystem = this;
	}

}

UISystem::~UISystem(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

void UISystem::StartUp() {
	SetDefaultNamedProperties();
	SetDefaultStateProperties();
	AllocUIClock();

	//load color scheme from file
	XMLFileParser myFileParser;
	LoadXMLFileToXMLParser(myFileParser, "Data/UI/ui_color.cscheme.xml");
	XMLNode colorSchemeNode = myFileParser.m_rootNode;
	LoadColorSchemeToMapFromXML(colorSchemeNode);

	
	ConsoleLogPrintf("\n===Loading ColorSchemes===\n");
		LoadAllColorSchemeFiles();
	ConsoleLogPrintf("===Loaded All ColorSchemess===\n");


// 	ConsolePrintf("\n===Loading WidgetStyles===\n");
// 	if (WidgetStyleFactory::LoadAllWidgetStyleFactories()) {
// 		ConsolePrintf("===Loaded All WidgetStyles===\n");
// 	}

	ConsoleLogPrintf("\n===Loading WidgetStyles===\n");
	if (ObjectFactory<WidgetStyle>::LoadAllObjectFactories("Data/UI/", "style", "WidgetStyle", true)) {
		ConsoleLogPrintf("===Loaded All WidgetStyles===\n");
	}

	//load widget defs
// 	ConsolePrintf("\n===Loading Widgets===\n");
// 	if (WidgetFactory::LoadAllWidgetFactories()) {
// 		ConsolePrintf("===Loaded All Widgets===\n");
// 	}

	ConsoleLogPrintf("\n===Loading Widgets===\n");
	if (ObjectFactory<BaseWidget>::LoadAllObjectFactories("Data/UI/", "widget", "Widget", true, &AllocWidget)) {
		ConsoleLogPrintf("===Loaded All Widgets===\n");
	}
	
	//UISystem::s_defaultWidgetFactory = WidgetFactory::FindFactoryByName("Default");
	UISystem::s_defaultWidgetFactory = ObjectFactory<BaseWidget>::FindFactoryByName("Default");

	//assumes default.widget.xml exists
	SetDefaultPropertiesUsingTempDefaultWidget();

	//initialize the UI screens for each state
	InitUIScreensFromGameStateRegistry();
	
	LoadXMLFileToXMLParser(myFileParser, "Data/ui.settings.xml");
	XMLNode UISettingsNode = myFileParser.m_rootNode;

	UIScreen::LoadAllUIScreensFromXML(UISettingsNode);

}

//-----------------------------------------------------------------------------------------------------------

void UISystem::LoadAllColorSchemeFiles() {
	Strings colorSchemeFilePaths;

	if (FindAllFilesOfType("Data/UI/", ".cscheme.xml", colorSchemeFilePaths, true)) {

		XMLParsers ColorSchemeFileParsers;
		ColorSchemeFileParsers.reserve(colorSchemeFilePaths.size() * sizeof(XMLParsers));

		XMLFileParser newColorSchemeFileParser;
		int colorSchemeParserIndex = 0;
		for (StringsIterator it = colorSchemeFilePaths.begin(); it != colorSchemeFilePaths.end(); ++it) {
			std::string& filePath = (*it);

			LoadXMLFileToXMLParser(newColorSchemeFileParser, filePath, "ColorScheme");

			ColorSchemeFileParsers.push_back(newColorSchemeFileParser);

			colorSchemeParserIndex++;

		}

		colorSchemeParserIndex = 0;
		for (StringsIterator it = colorSchemeFilePaths.begin(); it != colorSchemeFilePaths.end(); ++it) {
			//std::string& filePath = (*it);

			XMLFileParser& myColorSchemeParser = ColorSchemeFileParsers[colorSchemeParserIndex];

			LoadColorSchemeToMapFromXML(myColorSchemeParser.m_xmlNode);

			colorSchemeParserIndex++;
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

void UISystem::SetDefaultPropertiesUsingTempDefaultWidget() {
	BaseWidget* tempDefaultWidget;
	tempDefaultWidget = CreateWidget("Default", Vector2::ZERO);
	ASSERT_PTR_VALID(tempDefaultWidget);

	m_defaultProperties = tempDefaultWidget->GetDefaultProperties();

	for (int i = 0; i < NUM_WIDGET_STATES; i++) {
		WidgetStateID stateID = (WidgetStateID)i;
		WidgetStateProperties& defaultStateProperties = tempDefaultWidget->GetWidgetStateProperties(stateID);

		m_defaultStateProperties[stateID] = defaultStateProperties.GetStateProperties();
	}
	delete tempDefaultWidget;
}

//-----------------------------------------------------------------------------------------------------------

void UISystem::LoadColorSchemeToMapFromXML(const XMLNode& node) {

	ColorScheme newColorScheme;
	LoadColorSchemeFromXML(newColorScheme, node);
	m_colorSchemeMap.insert(ColorSchemeMapEntry(newColorScheme.name, newColorScheme));

}

//-----------------------------------------------------------------------------------------------------------

void UISystem::ShutDown() {
	//do stuff
	ClearWidgetRegistry();

// 	for (Widgets::iterator it = m_widgetInstances.begin(); it != m_widgetInstances.end();){
// 		BaseWidget* widget = (*it);
// 		if (widget) {
// 			delete widget;
// 			widget = NULL;
// 		}
// 		it = m_widgetInstances.erase(it);
// 	} 

	delete g_UIClock;
	g_UIClock = NULL;
}

///----------------------------------------------------------------------------------------------------------
///start up helpers

void UISystem::SetDefaultNamedProperties() {
	//sets default properties
	m_defaultProperties.Set("dimensions", Vector2(64.0f, 64.0f));
	m_defaultProperties.Set("bgColor", Rgba::BLACK);
	m_defaultProperties.Set("edgeColor", Rgba::WHITE);
	m_defaultProperties.Set("edgeSize", 5.0f);
	m_defaultProperties.Set("opacity", 1.0f);

}

//-----------------------------------------------------------------------------------------------------------

void UISystem::SetDefaultStateProperties() {

	for (int i = 0; i < NUM_WIDGET_STATES; i++) {
		WidgetStateID stateID = (WidgetStateID)i;

		m_defaultStateProperties[stateID] = m_defaultProperties;
	}

	NamedProperties& disabledStateProperties = m_defaultStateProperties[WIDGET_STATE_DISABLED];
	disabledStateProperties.Set("opacity", 0.5f);

}

//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------

//upkeep
void UISystem::Update() {

	double deltaSeconds = GetUIDeltaSeconds();

	for (BaseWidget* widget : m_widgetInstances) {
		widget->Update(deltaSeconds);
	}

}

//-----------------------------------------------------------------------------------------------------------

//render
void UISystem::Render(OpenGLRenderer* renderer) {

	//render UI screens
// 	auto* currentGameState = GetCurrentState();
// 	if (currentGameState) {
// 		currentGameState->RenderUIScreen(renderer);
// 	}

	if (m_widgetInstances.empty()) {
		return;
	}

	for (BaseWidget* widget : m_widgetInstances) {
		widget->Render(renderer);
	}



	

}

///----------------------------------------------------------------------------------------------------------
///helpers

BaseWidget* UISystem::AddWidgetToUIScreen(const std::string& UIScreenName, const std::string& widgetName, const Vector2& offset,
	const WidgetStateID& stateID) {

	BaseWidget* newWidget = CreateWidget(widgetName, offset, stateID);

	if (newWidget) {
		UIScreen& myUIScreen = FindUIScreenByName(UIScreenName);
		myUIScreen.m_widgetInstances.push_back(newWidget);

		return newWidget;
	}
	return NULL;

}

//-----------------------------------------------------------------------------------------------------------

BaseWidget* UISystem::AddWidgetToUIScreen(const std::string& UIScreenName, BaseWidget* widget, const Vector2& offset,
	const WidgetStateID& stateID) {
	
	if (widget) {
		UIScreen& myUIScreen = FindUIScreenByName(UIScreenName);
		myUIScreen.m_widgetInstances.push_back(widget);

		return widget;
	}
	return NULL;

}

//-----------------------------------------------------------------------------------------------------------

BaseWidget* UISystem::AddWidget(const std::string& widgetName, const Vector2& offset, const WidgetStateID& stateID) {

	BaseWidget* newWidget = CreateWidget(widgetName, offset, stateID);

	if (newWidget) {
		m_widgetInstances.push_back(newWidget);

		return newWidget;
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

BaseWidget* UISystem::CreateWidget(const std::string& widgetName, const Vector2& offset, const WidgetStateID& stateID, bool doStartUp) {
	BaseWidget* newWidget = NULL;
	ObjectFactory<BaseWidget>* myWidgetFactory = ObjectFactory<BaseWidget>::FindFactoryByName(widgetName);
	if (!myWidgetFactory) {
		myWidgetFactory = s_defaultWidgetFactory;
	}

 	std::string widgetType = myWidgetFactory->GetType();
	if (widgetType == "Group") {
		newWidget = CreateWidget<GroupWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "Layout") {
		newWidget = CreateWidget<LayoutWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "Cursor") {
		newWidget = CreateWidget<CursorWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if(widgetType == "Button") {
		newWidget = CreateWidget<ButtonWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "ProgressBar") {
		newWidget = CreateWidget<ProgressBarWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "Label") {
		newWidget = CreateWidget<LabelWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "CheckBox") {
		newWidget = CreateWidget<CheckBoxWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "EditLine") {
		newWidget = CreateWidget<EditLineWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "DropDown") {
		newWidget = CreateWidget<DropDownWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else if (widgetType == "Slider") {
		newWidget = CreateWidget<SliderWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}
	else {
		newWidget = CreateWidget<BaseWidget>(myWidgetFactory, offset, stateID, doStartUp);
	}

	return newWidget;

}

//-----------------------------------------------------------------------------------------------------------

void UISystem::SetWidgetStateByName(const std::string& widgetName, const WidgetStateID& stateID) {
	for (BaseWidget* widget : m_widgetInstances) {
		if (widget) {
			std::string myWidgetName = widget->GetName();
			if (myWidgetName == widgetName) {
				if (widget->GetCurrentState() != stateID) {
					widget->SetState(stateID);
					break;
				}
			}
		}//end of outer if
	}
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

