#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include <QString>
#include <QWidget>
#include <QVector2D>

#include "Core/Vector2D.h"
#include "Core/Entity.h"
#include "Core/Delegates.h"

#include "src/editorcommands/editorcommand.h"

class MainWindow;

namespace ads
{
	class CDockManager;
}

class TransformEditorWidget : public QWidget
{
public:
	TransformEditorWidget(MainWindow* mainWindow);

	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent* event) override;

	void onClick(const class QPoint& pos);
	NullableEntity getEntityUnderPoint(const QPoint& pos);

	void addEntitiesInRectToSelection(const Vector2D& start, const Vector2D& end);

	QVector2D project(const Vector2D& worldPos);
	Vector2D deproject(const QVector2D& screenPos);

	class World* mWorld = nullptr;
	MainWindow* mMainWindow;

	QVector2D mLastMousePos = QVector2D(0.0f, 0.0f);
	QVector2D mPressMousePos = QVector2D(0.0f, 0.0f);

	QVector2D mPosShift = QVector2D(0.0f, 0.0f);
	Vector2D mMoveShift = Vector2D(0.0f, 0.0f);
	float mScale = 1.0f;
	Vector2D mCursorObjectOffset;

	SinglecastDelegate<std::vector<Entity>, const Vector2D&> OnEntitiesMoved;

	std::vector<Entity> mSelectedEntities;

	bool mFreeMove = true;
	bool mIsMoved = false;
	bool mIsRectangleSelection = false;
	bool mIsCatchedSelectedEntity = false;
};

class TransformEditorToolbox : public QWidget
{
public:
	TransformEditorToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager);
	~TransformEditorToolbox();
	void show();

	static const QString WidgetName;
	static const QString ToolboxName;

private:
	void updateWorld();
	void updateContent(EditorCommand::EffectType effect, bool originalCall, bool forceUpdateLayout);
	void onEntitySelected(NullableEntity entity);
	void onEntitiesMoved(std::vector<Entity> entities, const Vector2D& shift);
	void onFreeMoveChanged(int newValue);
	void showContextMenu(const QPoint& pos);
	void onCopyCommand();
	void onPasteCommand();
	QVector2D getWidgetCenter();

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
	TransformEditorWidget* mContent = nullptr;

	std::vector<nlohmann::json> mCopiedObjects;
	Vector2D mCopiedGroupCenter;

	Delegates::Handle mOnWorldChangedHandle;
	Delegates::Handle mOnSelectedEntityChangedHandle;
	Delegates::Handle mOnCommandEffectHandle;
};
