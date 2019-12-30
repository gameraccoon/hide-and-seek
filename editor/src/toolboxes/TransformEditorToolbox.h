#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include <QString>
#include <QWidget>
#include <QVector2D>

#include "ECS/Entity.h"
#include "ECS/Delegates.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Spatial/SpatialEntity.h"
#include "GameData/Spatial/CellPos.h"

#include "src/editorcommands/editorcommand.h"
#include "src/editorutils/entityreference.h"

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
	std::vector<WorldCell*> getCellsOnScreen();
	SpatialEntity getEntityUnderPoint(const QPoint& pos);

	void addEntitiesInRectToSelection(const Vector2D& start, const Vector2D& end);

	QVector2D projectAbsolute(const Vector2D& worldPos) const;
	Vector2D deprojectAbsolute(const QVector2D& screenPos) const;
	QVector2D project(const CellPos& cellPos, const Vector2D& pos) const;
	std::pair<CellPos, Vector2D> deproject(const QVector2D& screenPos) const;

	class World* mWorld = nullptr;
	MainWindow* mMainWindow;

	QVector2D mLastMousePos = QVector2D(0.0f, 0.0f);
	QVector2D mPressMousePos = QVector2D(0.0f, 0.0f);

	QVector2D mPosShift = QVector2D(0.0f, 0.0f);
	Vector2D mMoveShift = Vector2D(0.0f, 0.0f);
	float mScale = 1.0f;
	Vector2D mCursorObjectOffset;

	SinglecastDelegate<std::vector<SpatialEntity>, const Vector2D&> OnEntitiesMoved;

	std::vector<SpatialEntity> mSelectedEntities;

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
	void onEntitySelected(const std::optional<EntityReference>& entityRef);
	void onEntitiesMoved(std::vector<SpatialEntity> entities, const Vector2D& shift);
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
