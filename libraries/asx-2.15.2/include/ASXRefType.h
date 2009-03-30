/***
 * asx: ASXRefType.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXREFTYPE
#define H_ASX_ASXREFTYPE

template <typename T>
class ASXRefType
{
	//#################### NESTED CLASSES ####################
protected:
	struct Releaser
	{
		void operator()(T *p)
		{
			p->release();
		}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	int m_refCount;

	//#################### CONSTRUCTORS ####################
protected:
	ASXRefType()
	:	m_refCount(1)
	{}

	//#################### PUBLIC METHODS ####################
public:
	void add_ref()
	{
		++m_refCount;
	}

	void release()
	{
		if(--m_refCount == 0) delete this;
	}
};

#endif
