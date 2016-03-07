
#include "OSGCommon.h"
#include "PickModelHandler.h"

osg::ref_ptr<osg::Node> createQuad()
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray(v.get());
	v->push_back(osg::Vec3(-1.f, 0.f, -1.f));
	v->push_back(osg::Vec3(1.f, 0.f, -1.f));
	v->push_back(osg::Vec3(1.f, 0.f, 1.f));
	v->push_back(osg::Vec3(-1.f, 0.f, 1.f));

	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
	geom->setColorArray(c.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	c->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
	c->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
	c->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
	c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));

	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	geom->setNormalArray(n.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	n->push_back(osg::Vec3(0.f, -1.f, 0.f));

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());
	return geode.get();
}

int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	osg::Node* model = osgDB::readNodeFiles(arguments);
	if (!model)model = osgDB::readNodeFile("cow.osg");

	osg::ref_ptr<osgManipulator::Selection> selection = new osgManipulator::Selection;
	selection->addChild(model);

	float scale = model->getBound().radius()*1.6;
	osg::ref_ptr<osgManipulator::TranslateAxisDragger> dragger = new osgManipulator::TranslateAxisDragger;
	dragger->setupDefaultGeometry();
	dragger->setMatrix(osg::Matrix::scale(scale, scale, scale) * osg::Matrix::translate(model->getBound().center()));

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(dragger.get());
	root->addChild(selection.get());
	//root->addChild(osgDB::readNodeFile("axes.osg"));

	osg::ref_ptr<osgManipulator::CommandManager> manager = new osgManipulator::CommandManager;
	manager->connect(*dragger, *selection);

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new PickModelHandler);
	viewer.setSceneData(root.get());
	return viewer.run();

	//osgViewer::Viewer viewer;
	//osg::Group* root = new osg::Group();
	//root->addChild(createQuad().get());
	//viewer.setSceneData(root);
	//viewer.realize();
	//viewer.run();
	//return 0;
}