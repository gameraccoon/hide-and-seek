#pragma once

#include <QString>
#include <QWidget>

class MainWindow;

namespace ads
{
	class CDockManager;
}

class QListWidgetItem;
class QMenu;
class QPoint;

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
	static const QString ListName;

private:
	void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void showContextMenu(const QPoint &pos);
	void removeSelectedEntity();
	void addComponentToEntity();

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
};
