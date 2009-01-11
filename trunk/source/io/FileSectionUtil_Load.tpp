/***
 * hesperus: FileSectionUtil_Load.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

//#################### LOADING METHODS ####################
/**
TODO
*/
template <typename Poly>
void FileSectionUtil::load_polygons_section(std::istream& is, const std::string& sectionName, std::vector<shared_ptr<Poly> >& polygons)
{
	std::string line;
	read_checked_line(is, line, sectionName);
	read_checked_line(is, line, "{");
	load_counted_polygons(is, polygons);
	read_checked_line(is, line, "}");
}

//#################### LOADING SUPPORT METHODS ####################
// TEMPORARY
template <typename Poly>
void FileSectionUtil::load_counted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons)
{
	std::string line;

	read_line(is, line, "polygon count");
	try
	{
		int polyCount = boost::lexical_cast<int,std::string>(line);
		load_polygons(is, polygons, polyCount);
	}
	catch(boost::bad_lexical_cast&) { throw Exception("The polygon count is not an integer"); }
}

}
