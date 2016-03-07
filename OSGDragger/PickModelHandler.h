#pragma once
#include "OSGCommon.h"

class PickModelHandler:public osgGA::GUIEventHandler
{
public:
	PickModelHandler() :_activeDragger(0){}

	bool handle(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa,
		osg::Object*, osg::NodeVisitor*)
	{
		osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
		if (!view)return false;

		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
		{
			_pointer.reset();
			osgUtil::LineSegmentIntersector::Intersections hits;
			if (view->computeIntersections(ea, hits))
			{
				_pointer.setCamera(view->getCamera());
				_pointer.setMousePosition(ea.getX(), ea.getY());

				osgUtil::LineSegmentIntersector::Intersections::iterator;
				for (auto hitr = hits.begin(); hitr != hits.end();++hitr)
				{
					_pointer.addIntersection(hitr->nodePath,
						hitr->getLocalIntersectPoint());
				}

				osg::NodePath::iterator;
				for (auto itr = _pointer._hitList.front().first.begin();
					itr != _pointer._hitList.front().first.end(); ++itr)
				{
					osgManipulator::Dragger* dragger = dynamic_cast<osgManipulator::Dragger*>(*itr);
					if (dragger)
					{
						dragger->handle(_pointer, ea, aa);
						_activeDragger = dragger;
						break;
					}
				}
			}
			break;
		}
		case osgGA::GUIEventAdapter::DRAG:
		case osgGA::GUIEventAdapter::RELEASE:
		{
			if (_activeDragger)
			{
				_pointer._hitIter = _pointer._hitList.begin();
				_pointer.setCamera(view->getCamera());
				_pointer.setMousePosition(ea.getX(), ea.getY());
				_activeDragger->handle(_pointer, ea, aa);
			}

			if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)
			{
				_activeDragger = NULL;
				_pointer.reset();
			}
			break;
		}
		default:
			break;
		}
		return true;
	}

protected:
	osgManipulator::Dragger* _activeDragger;
	osgManipulator::PointerInfo _pointer;
private:
	~PickModelHandler();
};

