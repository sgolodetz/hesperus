/***
 * hesperus: IComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICOMPONENT
#define H_HESP_ICOMPONENT

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/util/Properties.h>
#include "MessageHandler.h"
#include "ObjectID.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectManager;

class IComponent : public virtual MessageHandlerBase
{
	//#################### FRIENDS ####################
	friend class ObjectManager;

	//#################### PROTECTED VARIABLES ####################
protected:
	ObjectID m_objectID;
	ObjectManager *m_objectManager;

	//#################### DESTRUCTOR ####################
public:
	virtual ~IComponent();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::string group_type() const = 0;
	virtual std::string own_type() const = 0;
	virtual std::pair<std::string,Properties> save() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	virtual void check_dependencies() const;

	//#################### PROTECTED METHODS ####################
protected:
	void set_object_id(const ObjectID& objectID);
	void set_object_manager(ObjectManager *objectManager);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IComponent> IComponent_Ptr;
typedef shared_ptr<const IComponent> IComponent_CPtr;

}

#endif
