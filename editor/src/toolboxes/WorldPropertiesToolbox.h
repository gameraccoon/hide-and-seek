#pragma once

#include <QString>
#include <QWidget>

#include "Core/Delegates.h"

class MainWindow;

namespace ads
{
	class CDockManager;
}

class WorldPropertiesToolbox : public QWidget
{
public:
	WorldPropertiesToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager);
	~WorldPropertiesToolbox();
	void show();

	void updateContent();

	static const QString WidgetName;
	static const QString ToolboxName;
	static const QString ContainerName;
	static const QString ContainerContentName;

private:
	void onControledEntityChanged(const QString &arg1);
	void onCameraEntityChanged(const QString &arg1);

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;

	Delegates::HandleType mOnWorldChangedHandle;
};
