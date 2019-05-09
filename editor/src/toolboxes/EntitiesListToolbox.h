#pragma once

#include <QString>
#include <QWidget>

#include "Core/Delegates.h"

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
	~EntitiesListToolbox();
	void show();

	void onWorldUpdated();

	static const QString WidgetName;
	static const QString ToolboxName;
	static const QString ContainerName;
	static const QString ContainerContentName;
	static const QString ListName;

private:
	void updateContent();
	void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void showContextMenu(const QPoint& pos);
	void removeSelectedEntity();
	void onAddComponentToEntityRequested();
	void addComponentToEntity(const QString& typeName);
	void bindEvents();
	void unbindEvents();

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;

	Delegates::HandleType mOnEntityAddedHandle;
	Delegates::HandleType mOnEntityRemovedHandle;
	Delegates::HandleType mOnWorldChangedHandle;
};
