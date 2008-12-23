/***
 * hesperus: RepresentativeTree.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_DATASTRUCTURES_REPRESENTATIVETREE
#define H_HESP_DATASTRUCTURES_REPRESENTATIVETREE

#include <list>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

template <typename T, typename Pred>
class RepresentativeNode
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<RepresentativeNode> RepresentativeNode_Ptr;
	typedef std::list<T> TList;
	typedef shared_ptr<TList> TList_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	const T m_representative;
	std::vector<T> m_bucket;
	RepresentativeNode_Ptr m_left, m_right;

	//#################### CONSTRUCTORS ####################
public:
	RepresentativeNode(const T& representative)
	:	m_representative(representative)
	{}

	//#################### PUBLIC METHODS ####################
public:
	void insert(const T& t, const Pred& pred)
	{
		switch(pred(t, m_representative))
		{
			case -1:
			{
				if(m_left) m_left->insert(t, pred);
				else m_left.reset(new RepresentativeNode(t));
				break;
			}
			case 0:
			{
				m_bucket.push_back(t);
				break;
			}
			case 1:
			{
				if(m_right) m_right->insert(t, pred);
				else m_right.reset(new RepresentativeNode(t));
				break;
			}
		}
	}

	TList_Ptr representatives() const
	{
		TList_Ptr ret(new TList);
		if(m_left) ret->splice(ret->end(), *m_left->representatives());
		ret->push_back(m_representative);
		if(m_right) ret->splice(ret->end(), *m_right->representatives());
		return ret;
	}
};

template <typename T, typename Pred>
class RepresentativeTree
{
	//#################### TYPEDEFS ####################
private:
	typedef RepresentativeNode<T,Pred> RepresentativeNodeT;
	typedef shared_ptr<RepresentativeNodeT> RepresentativeNode_Ptr;
	typedef std::list<T> TList;
	typedef shared_ptr<TList> TList_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	Pred m_pred;
	RepresentativeNode_Ptr m_root;

	//#################### CONSTRUCTORS ####################
public:
	RepresentativeTree(const Pred& pred)
	:	m_pred(pred)
	{}

	//#################### PUBLIC METHODS ####################
public:
	void insert(const T& t)
	{
		if(m_root) m_root->insert(t, m_pred);
		else m_root.reset(new RepresentativeNodeT(t));
	}

	TList_Ptr representatives() const
	{
		if(m_root) return m_root->representatives();
		else return TList_Ptr();
	}
};

}

#endif
