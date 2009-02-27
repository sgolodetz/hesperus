/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <source/exceptions/Exception.h>
#include <source/game/Game.h>
#if 0
#include <source/xml/XMLParser.h>
#endif
using namespace hesp;

int main(int argc, char* argv[])
try
{
	// FIXME: Remove this test code (and the include above).
#if 0
	XMLLexer lexer("Cube.mesh.xml");
	XMLToken_Ptr token;
	while(token = lexer.next_token())
	{
		switch(token->type())
		{
		case XMLT_EQUALS:	std::cout << "EQUALS"; break;
		case XMLT_IDENT:	std::cout << "IDENT"; break;
		case XMLT_LBRACKET:	std::cout << "LBRACKET"; break;
		case XMLT_LSLASH:	std::cout << "LSLASH"; break;
		case XMLT_RBRACKET:	std::cout << "RBRACKET"; break;
		case XMLT_RSLASH:	std::cout << "RSLASH"; break;
		case XMLT_VALUE:	std::cout << "VALUE"; break;
		}
		std::cout << ' ' << token->value() << std::endl;
	}
#endif
#if 0
	XMLLexer_Ptr lexer(new XMLLexer("Cube.mesh.xml"));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	// TODO
#endif

	Game game;
	game.run();

	return 0;
}
catch(Exception& e)
{
	std::cerr << e.cause() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << "Error: Something strange and unexpected happened" << std::endl;
	return EXIT_FAILURE;
}
