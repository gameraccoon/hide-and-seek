#pragma once

#include <QString>
#include <QWidget>

class MainWindow;

namespace ads
{
	class CDockManager;
}

class QListWidgetItem;

class ComponentsListToolbox : public QWidget
{
public:
	ComponentsListToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager);
	void show();
	void updateSelectedComponentData(QListWidgetItem* selectedItem);

	static const QString WidgetName;
	static const QString ToolboxName;
	static const QString ContainerName;
	static const QString ContainerContentName;
	static const QString ListName;

private:
	void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
};
