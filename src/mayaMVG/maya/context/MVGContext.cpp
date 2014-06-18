#include <QWidget>
#include "mayaMVG/maya/context/MVGContext.h"
#include "mayaMVG/maya/context/MVGBuildFaceManipulator.h"
#include "mayaMVG/maya/MVGMayaUtil.h"
#include "mayaMVG/qt/MVGMayaEvent.h"

#include "mayaMVG/core/MVGLog.h"
#include <maya/MCursor.h>

using namespace mayaMVG;

MVGContext::MVGContext()
	: m_eventFilter(NULL)
{
	setTitleString("MVG tool");
}

void MVGContext::toolOnSetup(MEvent & event)
{
	updateManipulators(this);
	// install context event filter
	QWidget* leftViewport = MVGMayaUtil::getMVGLeftViewportLayout();
	QWidget* rightViewport = MVGMayaUtil::getMVGRightViewportLayout();
	if(!leftViewport || !rightViewport)
		return;
	m_eventFilter = new MVGContextEventFilter(this, leftViewport);
	leftViewport->installEventFilter(m_eventFilter);
	rightViewport->installEventFilter(m_eventFilter);
}

void MVGContext::toolOffCleanup()
{
	// remove context event filter
	QWidget* leftViewport = MVGMayaUtil::getMVGLeftViewportLayout();
	QWidget* rightViewport = MVGMayaUtil::getMVGRightViewportLayout();
	if(leftViewport)
	{
		leftViewport->removeEventFilter(m_eventFilter);
		//leftViewport->setCursor(QCursor(Qt::ArrowCursor));
	}		
	if(rightViewport)
	{
		rightViewport->removeEventFilter(m_eventFilter);
		//rightViewport->setCursor(QCursor(Qt::ArrowCursor));
	}
		
	MPxContext::toolOffCleanup();
}

MStatus MVGContext::doPress(MEvent & event)
{
	return MPxContext::doPress(event);
}

MStatus MVGContext::doDrag(MEvent & event)
{
	return MS::kSuccess;
}

MStatus MVGContext::doRelease(MEvent & event)
{
	return MPxContext::doRelease(event);
}

void MVGContext::getClassName(MString & name) const
{
	name.set("mayaMVGTool");
}

void MVGContext::updateManipulators(void * data)
{
	// delete all manipulators
	MVGContext* ctxPtr = static_cast<MVGContext*>(data);
	ctxPtr->deleteManipulators();

	// then add a new one
	MString manipName("MVGBuildFaceManipulator");
	MObject manipObject;
	MPxManipulatorNode * manipNode = MPxManipulatorNode::newManipulator(manipName, manipObject);
	MVGBuildFaceManipulator* manipulator = dynamic_cast<MVGBuildFaceManipulator*>(manipNode);
	manipulator->_context = this;
	
	ctxPtr->addManipulator(manipObject);
}

void MVGContext::setCursor(MCursor cursor)
{
	MPxContext::setCursor(cursor);
}
