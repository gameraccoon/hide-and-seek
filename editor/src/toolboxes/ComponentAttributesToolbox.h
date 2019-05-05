#pragma once

#include <QString>
#include <QWidget>

class MainWindow;

namespace ads
{
	class CDockManager;
}

class ComponentAttributesToolbox : public QWidget
{
public:
	ComponentAttributesToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager);
	void show();

	static const QString WidgetName;
	static const QString ToolboxName;
	static const QString ContainerName;
	static const QString ContainerContentName;

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
};
