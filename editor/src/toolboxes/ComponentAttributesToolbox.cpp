#include "ComponentAttributesToolbox.h"

#include "src/mainwindow.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QVBoxLayout>

const QString ComponentAttributesToolbox::WidgetName = "ComponentAttributes";
const QString ComponentAttributesToolbox::ToolboxName = ComponentAttributesToolbox::WidgetName + "Toolbox";
const QString ComponentAttributesToolbox::ContainerName = ComponentAttributesToolbox::WidgetName + "Container";
const QString ComponentAttributesToolbox::ContainerContentName = ComponentAttributesToolbox::ContainerName + "Content";

ComponentAttributesToolbox::ComponentAttributesToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager)
	: mMainWindow(mainWindow)
	, mDockManager(dockManager)
{
}

void ComponentAttributesToolbox::show()
{
	if (ads::CDockWidget* dockWidget = mDockManager->findDockWidget(ToolboxName))
	{
		if (dockWidget->isVisible())
		{
			return;
		}
		else
		{
			mDockManager->layout()->removeWidget(dockWidget);
		}
	}

	QWidget* containerWidget = new QWidget();
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("Component Attributes"));
	dockWidget->setObjectName(ToolboxName);
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(mMainWindow->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget);

	containerWidget->setObjectName(ContainerName);

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
}
