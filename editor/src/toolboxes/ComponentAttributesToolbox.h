#pragma once

#include <QString>
#include <QWidget>

#include "Core/Delegates.h"

class MainWindow;

namespace ads
{
	class CDockManager;
}

class ComponentAttributesToolbox : public QWidget
{
public:
	ComponentAttributesToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager);
	~ComponentAttributesToolbox();
	void show();

	static const QString WidgetName;
	static const QString ToolboxName;
	static const QString ContainerName;
	static const QString ContainerContentName;

private:
	void updateContent();
	void clearContent();
	void onSelectedComponentChange(const QString& componentTypeName);

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
	QString mLastSelectedComlonent;

	Delegates::HandleType mOnComponentChangedHandle;
	Delegates::HandleType mOnComponentContentChangedHandle;
};
