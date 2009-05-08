/***
 * hesperus: ObjectID.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTID
#define H_HESP_OBJECTID

namespace hesp {

class ObjectID
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_id;

	//#################### CONSTRUCTORS ####################
public:
	ObjectID();
	explicit ObjectID(int id);

	//#################### PUBLIC METHODS ####################
public:
	int value() const;
};

//#################### GLOBAL OPERATORS ####################
bool operator<(const ObjectID& lhs, const ObjectID& rhs);

}

#endif
