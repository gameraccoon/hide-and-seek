#pragma once

#include <QString>
#include <QWidget>

class MainWindow;

namespace ads
{
	class CDockManager;
}

class QListWidgetItem;

class EntitiesListToolbox : public QWidget
{
public:
	EntitiesListToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager);
	void show();

	void updateContent();

	static const QString WidgetName;
	static const QString ToolboxName;
	static const QString ContainerName;
	static const QString ContainerContentName;

private:
	void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
};
