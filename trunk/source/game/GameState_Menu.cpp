/***
 * hesperus: GameState_Menu.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_Menu.h"

#include <boost/bind.hpp>

#include <source/gui/Button.h>
#include <source/gui/ExplicitLayout.h>
#include <source/gui/Picture.h>
#include <source/gui/Screen.h>
#include <source/io/util/DirectoryFinder.h>
#include "GameState_Load.h"

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Menu::GameState_Menu(const std::string& menu, const std::string& initialLevelFilename)
:	m_menu(menu), m_initialLevelFilename(initialLevelFilename)
{}

//#################### PUBLIC METHODS ####################
void GameState_Menu::enter()
{
	// TODO: Construct the appropriate menu based on the parameter to the constructor.
	set_display(construct_buttons_menu(menu_buttons_main()));
}

void GameState_Menu::leave()
{
	// Nothing to do
}

GameState_Ptr GameState_Menu::update(int milliseconds, UserInput& input)
{
	Screen& screen = Screen::instance();
	screen.handle_input(input);
	return m_newState;
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_Menu::construct_buttons_menu(const std::vector<Button*> menuButtons)
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = determine_images_directory(determine_base_directory_from_game());

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();

	// Add the title image.
	display->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(width/4, 0, width*3/4, width/8));

	// Add the backdrop image.
	const int BUTTON_HEIGHT = 100;
	const int BUTTON_SPACING = 50;								// space between adjacent buttons
	const int BUTTON_STRIDE = BUTTON_HEIGHT + BUTTON_SPACING;	// vertical distance between the centres of adjacent buttons
	int buttonsLeft = width*3/10;
	int buttonsRight = width*7/10;
	int buttonsTop = width/8 + 50;

	int buttonCount = static_cast<int>(menuButtons.size());
	Extents backdropExtents(buttonsLeft - BUTTON_SPACING, buttonsTop - BUTTON_SPACING,
							buttonsRight + BUTTON_SPACING, buttonsTop + buttonCount * BUTTON_STRIDE);
	display->layout().add(new Picture((imagesDir / "menu-backdrop.png").file_string()), backdropExtents);

	// Add the buttons.
	for(int i=0; i<buttonCount; ++i)
	{
		int buttonTop = buttonsTop + i*BUTTON_STRIDE;
		display->layout().add(menuButtons[i], Extents(buttonsLeft, buttonTop, buttonsRight, buttonTop+100));
	}

	return GUIComponent_Ptr(display);
}

std::vector<Button*> GameState_Menu::menu_buttons_main()
{
	std::vector<Button*> ret;
	bf::path imagesDir = determine_images_directory(determine_base_directory_from_game());

	ret.push_back(new Button((imagesDir / "buttons-newgame-inactive.png").file_string(),
							 (imagesDir / "buttons-newgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_Menu::on_released_NewGame, this)));

	ret.push_back(new Button((imagesDir / "buttons-exitgame-inactive.png").file_string(),
							 (imagesDir / "buttons-exitgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_Menu::on_released_ExitGame, this)));

	return ret;
}

//#################### BUTTON HANDLERS ####################
void GameState_Menu::on_released_ExitGame()
{
	set_quit_flag();
}

void GameState_Menu::on_released_NewGame()
{
	m_newState.reset(new GameState_Load(m_initialLevelFilename));
}

}
