#include "TransformEditorToolbox.h"

#include "src/mainwindow.h"

#include "src/editorcommands/changeentitygrouplocationcommand.h"
#include "src/editorcommands/addentitygroupcommand.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QApplication>
#include <QAction>
#include <QMenu>

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"

const QString TransformEditorToolbox::WidgetName = "TransformEditor";
const QString TransformEditorToolbox::ToolboxName = TransformEditorToolbox::WidgetName + "Toolbox";

static bool IsCtrlPressed()
{
	return (QApplication::keyboardModifiers() & Qt::ControlModifier) != 0;
}

TransformEditorToolbox::TransformEditorToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager)
	: mMainWindow(mainWindow)
	, mDockManager(dockManager)
{
	mOnWorldChangedHandle = mMainWindow->OnWorldChanged.bind([this]{updateWorld();});
	mOnSelectedEntityChangedHandle = mMainWindow->OnSelectedEntityChanged.bind([this](const auto& entityRef){onEntitySelected(entityRef);});
	mOnCommandEffectHandle = mMainWindow->OnCommandEffectApplied.bind([this](EditorCommand::EffectType effect, bool originalCall, bool forceUpdateLayout){updateContent(effect, originalCall, forceUpdateLayout);});
}

TransformEditorToolbox::~TransformEditorToolbox()
{
	mMainWindow->OnWorldChanged.unbind(mOnWorldChangedHandle);
	mMainWindow->OnSelectedEntityChanged.unbind(mOnSelectedEntityChangedHandle);
	mMainWindow->OnCommandEffectApplied.unbind(mOnCommandEffectHandle);
}

void TransformEditorToolbox::show()
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

	mContent = new TransformEditorWidget(mMainWindow);
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("Transform Editor"));
	dockWidget->setObjectName(ToolboxName);
	dockWidget->setWidget(mContent);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(mMainWindow->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget);
	dockWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(dockWidget, &QListWidget::customContextMenuRequested, this, &TransformEditorToolbox::showContextMenu);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addStretch();
	mContent->setLayout(layout);
	QCheckBox* freeMoveCheckbox = new QCheckBox();
	freeMoveCheckbox->setText("Free Move");
	freeMoveCheckbox->setChecked(mContent->mFreeMove);
	QObject::connect(freeMoveCheckbox, &QCheckBox::stateChanged, this, &TransformEditorToolbox::onFreeMoveChanged);
	layout->addWidget(freeMoveCheckbox);
	mContent->OnEntitiesMoved.assign([this](std::vector<SpatialEntity> entities, const Vector2D& shift){onEntitiesMoved(entities, shift);});
}

void TransformEditorToolbox::updateWorld()
{
	if (mContent == nullptr)
	{
		return;
	}

	mContent->mWorld = mMainWindow->getCurrentWorld();
	mContent->repaint();
}

void TransformEditorToolbox::updateContent(EditorCommand::EffectType effect, bool /*originalCall*/, bool /*forceUpdateLayout*/)
{
	if (effect == EditorCommand::EffectType::ComponentAttributes || effect == EditorCommand::EffectType::Entities)
	{
		mContent->repaint();
	}
}

void TransformEditorToolbox::onEntitySelected(const std::optional<EntityReference>& entityRef)
{
	if (mContent == nullptr)
	{
		return;
	}

	World* world = mMainWindow->getCurrentWorld();
	if (world == nullptr)
	{
		return;
	}

	mContent->mSelectedEntities.clear();
	if (entityRef.has_value() && entityRef->cellPos.has_value())
	{
		WorldCell* cell = world->getSpatialData().getCell(*entityRef->cellPos);
		if (cell != nullptr && cell->getEntityManager().doesEntityHaveComponent<TransformComponent>(entityRef->entity))
		{
			SpatialEntity spatialEntity(entityRef->entity, *entityRef->cellPos);
			mContent->mSelectedEntities.push_back(spatialEntity);
		}
	}
	mContent->repaint();
}

void TransformEditorToolbox::onEntitiesMoved(std::vector<SpatialEntity> entities, const Vector2D& shift)
{
	World* world = mMainWindow->getCurrentWorld();
	if (world == nullptr)
	{
		return;
	}

	mMainWindow->getCommandStack().executeNewCommand<ChangeEntityGroupLocationCommand>(world, entities, shift);
}

void TransformEditorToolbox::onFreeMoveChanged(int newValue)
{
	if (mContent)
	{
		mContent->mFreeMove = (newValue != 0);
	}
}

void TransformEditorToolbox::showContextMenu(const QPoint& pos)
{
	QMenu contextMenu(tr("Context menu"), this);

	QAction actionCopyComponent("Copy", this);
	connect(&actionCopyComponent, &QAction::triggered, this, &TransformEditorToolbox::onCopyCommand);
	contextMenu.addAction(&actionCopyComponent);

	QAction actionPasteComponent("Paste", this);
	connect(&actionPasteComponent, &QAction::triggered, this, &TransformEditorToolbox::onPasteCommand);
	contextMenu.addAction(&actionPasteComponent);

	contextMenu.exec(mContent->mapToGlobal(pos));
}

void TransformEditorToolbox::onCopyCommand()
{
	World* world = mMainWindow->getCurrentWorld();
	if (world == nullptr)
	{
		return;
	}

	if (mContent == nullptr)
	{
		return;
	}

	Vector2D center;
	mCopiedObjects.clear();
	for (SpatialEntity spatialEntity : mContent->mSelectedEntities)
	{
		nlohmann::json serializedEntity;
		world->getEntityManager().getPrefabFromEntity(serializedEntity, spatialEntity.entity.getEntity());
		mCopiedObjects.push_back(serializedEntity);
		auto [transform] = world->getEntityManager().getEntityComponents<TransformComponent>(spatialEntity.entity.getEntity());
		if (transform)
		{
			center += transform->getLocation();
		}
	}

	if (mCopiedObjects.size() > 0)
	{
		mCopiedGroupCenter = center / mCopiedObjects.size();
	}
}

void TransformEditorToolbox::onPasteCommand()
{
	World* world = mMainWindow->getCurrentWorld();
	if (world == nullptr)
	{
		return;
	}

	if (mContent == nullptr)
	{
		return;
	}

	if (mCopiedObjects.empty())
	{
		return;
	}

	ComponentFactory& factory = mMainWindow->getComponentFactory();

	mMainWindow->getCommandStack().executeNewCommand<AddEntityGroupCommand>(world,
		mCopiedObjects,
		&factory,
		mContent->deprojectAbsolute(getWidgetCenter()) - mCopiedGroupCenter);

	mContent->repaint();
}

QVector2D TransformEditorToolbox::getWidgetCenter()
{
	QSize size = mContent->size() / 2;
	return QVector2D(size.width(), size.height());
}

TransformEditorWidget::TransformEditorWidget(MainWindow *mainWindow)
	: mMainWindow(mainWindow)
{
	mWorld = mainWindow->getCurrentWorld();
}

void TransformEditorWidget::mousePressEvent(QMouseEvent* event)
{
	mLastMousePos = QVector2D(event->pos());
	mMoveShift = ZERO_VECTOR;
	mPressMousePos = mLastMousePos;
	mIsMoved = false;
	mIsCatchedSelectedEntity = false;
	mIsRectangleSelection = false;

	if (SpatialEntity entityUnderCursor = getEntityUnderPoint(event->pos()); entityUnderCursor.isValid())
	{
		if (std::find(mSelectedEntities.begin(), mSelectedEntities.end(), entityUnderCursor) != mSelectedEntities.end())
		{
			mIsCatchedSelectedEntity = true;
		}
		else if (mFreeMove && !IsCtrlPressed())
		{
			mSelectedEntities.clear();
			mSelectedEntities.push_back(entityUnderCursor);
			mIsCatchedSelectedEntity = true;
		}
	}
	else
	{
		if (IsCtrlPressed())
		{
			mIsRectangleSelection = true;
		}
	}
}

void TransformEditorWidget::mouseMoveEvent(QMouseEvent* event)
{
	QVector2D pos = QVector2D(event->pos());
	if (!mIsMoved && (mPressMousePos - pos).lengthSquared() > 10.0f)
	{
		mIsMoved = true;
	}

	if (mIsMoved && mIsCatchedSelectedEntity)
	{
		mMoveShift = deprojectAbsolute(pos) - deprojectAbsolute(mPressMousePos);
	}

	if (!mIsCatchedSelectedEntity && !mIsRectangleSelection)
	{
		mPosShift -= QVector2D(mLastMousePos - pos);
	}
	mLastMousePos = pos;
	repaint();
}

void TransformEditorWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (mIsMoved)
	{
		if (mIsCatchedSelectedEntity && !mSelectedEntities.empty())
		{
			OnEntitiesMoved.callSafe(mSelectedEntities, mMoveShift);
			mMoveShift = ZERO_VECTOR;
		}
		else if (mIsRectangleSelection)
		{
			addEntitiesInRectToSelection(deprojectAbsolute(mPressMousePos), deprojectAbsolute(QVector2D(event->pos())));
		}
	}
	else
	{
		onClick(event->pos());
	}

	mIsRectangleSelection = false;
	repaint();
}

void TransformEditorWidget::paintEvent(QPaintEvent*)
{
	if (mWorld == nullptr)
	{
		return;
	}

	QPainter painter(this);

	std::vector<WorldCell*> cells = getCellsOnScreen();
	for (WorldCell* cell : cells)
	{
		cell->getEntityManager().forEachComponentSetWithEntity<TransformComponent>([&painter, cell, this](Entity entity, TransformComponent* transform)
		{
			CellPos cellPos = cell->getPos();
			Vector2D location = transform->getLocation() + Vector2D(cellPos.x * SpatialWorldData::CellSize, cellPos.y * SpatialWorldData::CellSize);

			auto [collision] = cell->getEntityManager().getEntityComponents<CollisionComponent>(entity);

			if (std::find(mSelectedEntities.begin(), mSelectedEntities.end(), SpatialEntity(entity, cellPos)) != mSelectedEntities.end())
			{
				// preview the movement
				location += mMoveShift;

				// calc selected entity border
				QVector2D selectionLtShift;
				QVector2D selectionSize;
				if (collision)
				{
					Hull geometry = collision->getGeometry();
					if (geometry.type == HullType::Angular)
					{
						for (Vector2D& point : geometry.points)
						{
							if (point.x < selectionLtShift.x())
							{
								selectionLtShift.setX(point.x);
							}
							if (point.y < selectionLtShift.y())
							{
								selectionLtShift.setY(point.y);
							}
							if (point.x > selectionSize.x())
							{
								selectionSize.setX(point.x);
							}
							if (point.y > selectionSize.y())
							{
								selectionSize.setY(point.y);
							}
						}

						selectionSize -= selectionLtShift;
					}
					else
					{
						float radius = geometry.getRadius();
						selectionLtShift = QVector2D(-radius, -radius);
						selectionSize = QVector2D(radius * 2.0f, radius * 2.0f);
					}
				}

				// draw selected entity border
				selectionLtShift -= QVector2D(5.0f, 5.0f);
				selectionSize += QVector2D(10.0f, 10.0f);
				QRectF rectangle((projectAbsolute(location) + selectionLtShift).toPoint(), QSize(static_cast<int>(selectionSize.x()), static_cast<int>(selectionSize.y())));
				QBrush brush = painter.brush();
				brush.setColor(Qt::GlobalColor::blue);
				painter.setBrush(brush);
				painter.drawRect(rectangle);
			}

			// draw collision
			if (collision)
			{
				Hull geometry = collision->getGeometry();
				if (geometry.type == HullType::Angular)
				{
					QPolygonF polygon;
					for (Vector2D& point : geometry.points)
					{
						polygon.append(projectAbsolute(location + point).toPointF());
					}
					painter.drawPolygon(polygon);
				}
				else
				{
					float radius = geometry.getRadius();
					float halfWorldSize = radius * mScale;
					int worldSizeInt = static_cast<int>(halfWorldSize * 2.0f);
					QRectF rectangle((projectAbsolute(location) - QVector2D(halfWorldSize, halfWorldSize)).toPoint(), QSize(worldSizeInt, worldSizeInt));
					painter.drawEllipse(rectangle);
				}
			}

			// draw entity location cross
			QVector2D screenLocation = projectAbsolute(location);
			QPoint screenPoint(static_cast<int>(screenLocation.x()), static_cast<int>(screenLocation.y()));
			painter.drawLine(QPoint(screenPoint.x() - 5, screenPoint.y()), QPoint(screenPoint.x() + 5, screenPoint.y()));
			painter.drawLine(QPoint(screenPoint.x(), screenPoint.y() - 5), QPoint(screenPoint.x(), screenPoint.y() + 5));
		});
	}

	if (mIsRectangleSelection)
	{
		QVector2D size = QVector2D(mLastMousePos) - mPressMousePos;
		QRect rect(mPressMousePos.toPoint(), QSize(static_cast<int>(size.x()), static_cast<int>(size.y())));
		painter.drawRect(rect);
	}
}

void TransformEditorWidget::onClick(const QPoint& pos)
{
	SpatialEntity findResult = getEntityUnderPoint(pos);

	if (IsCtrlPressed())
	{
		if (findResult.isValid())
		{
			auto it = std::find(mSelectedEntities.begin(), mSelectedEntities.end(), findResult);
			if (it != mSelectedEntities.end())
			{
				mSelectedEntities.erase(it);
			}
			else
			{
				mSelectedEntities.push_back(findResult);
			}
		}
	}
	else
	{
		mSelectedEntities.clear();

		if (findResult.isValid())
		{
			mSelectedEntities.push_back(findResult);
			mMainWindow->OnSelectedEntityChanged.broadcast(EntityReference(findResult));
		}
	}
}

std::vector<WorldCell*> TransformEditorWidget::getCellsOnScreen()
{
	CellPos screenCenterCellPos{0, 0};
	Vector2D screenSize = Vector2D(size().width(), size().height());
	Vector2D screenCenterPos = screenSize*0.5f - Vector2D(mPosShift.x(), mPosShift.y());
	SpatialWorldData::TransformCellPos(screenCenterCellPos, screenCenterPos);
	return mWorld->getSpatialData().getCellsAround(screenCenterCellPos, screenCenterPos, screenSize);
}

SpatialEntity TransformEditorWidget::getEntityUnderPoint(const QPoint& pos)
{
	Vector2D worldPos = deprojectAbsolute(QVector2D(pos));

	SpatialEntity findResult;

	if (mWorld)
	{
		std::vector<WorldCell*> cells = getCellsOnScreen();
		for (WorldCell* cell : cells)
		{
			cell->getEntityManager().forEachComponentSetWithEntity<TransformComponent>([worldPos, cellPos = cell->getPos(), &findResult](Entity entity, TransformComponent* transform)
			{
				Vector2D location = transform->getLocation() + Vector2D(cellPos.x * SpatialWorldData::CellSize, cellPos.y * SpatialWorldData::CellSize);
				if (location.x - 10 < worldPos.x && location.x + 10 > worldPos.x
					&&
					location.y - 10 < worldPos.y && location.y + 10 > worldPos.y)
				{
					findResult.entity = entity;
					findResult.cell = cellPos;
				}
			});
		}
	}

	return findResult;
}

void TransformEditorWidget::addEntitiesInRectToSelection(const Vector2D& start, const Vector2D& end)
{
	Vector2D lt;
	Vector2D rd;

	if (start.x > end.x)
	{
		lt.x = end.x;
		rd.x = start.x;
	}
	else
	{
		lt.x = start.x;
		rd.x = end.x;
	}

	if (start.y > end.y)
	{
		lt.y = end.y;
		rd.y = start.y;
	}
	else
	{
		lt.y = start.y;
		rd.y = end.y;
	}

	if (mWorld)
	{
		std::vector<WorldCell*> cells = getCellsOnScreen();
		for (WorldCell* cell : cells)
		{
			cell->getEntityManager().forEachComponentSetWithEntity<TransformComponent>([this, lt, rd, cellPos = cell->getPos()](Entity entity, TransformComponent* transform)
			{
				Vector2D location = transform->getLocation() + Vector2D(cellPos.x * SpatialWorldData::CellSize, cellPos.y * SpatialWorldData::CellSize);
				if (lt.x < location.x && location.x < rd.x && lt.y < location.y && location.y < rd.y)
				{
					auto it = std::find(mSelectedEntities.begin(), mSelectedEntities.end(), SpatialEntity(entity, cellPos));
					if (it == mSelectedEntities.end())
					{
						SpatialEntity findResult;
						findResult.entity = entity;
						findResult.cell = cellPos;
						mSelectedEntities.push_back(findResult);
					}
				}
			});
		}
	}
}

QVector2D TransformEditorWidget::projectAbsolute(const Vector2D& worldPos) const
{
	return mScale * (QVector2D(worldPos.x, worldPos.y) + mPosShift);
}

Vector2D TransformEditorWidget::deprojectAbsolute(const QVector2D &screenPos) const
{
	QVector2D worldPos = screenPos / mScale - mPosShift;
	return Vector2D(worldPos.x(), worldPos.y());
}

QVector2D TransformEditorWidget::project(const CellPos& cellPos, const Vector2D& pos) const
{
	Vector2D absoluteWorldPos = pos + Vector2D(cellPos.x * SpatialWorldData::CellSize, cellPos.y * SpatialWorldData::CellSize);
	return mScale * (QVector2D(absoluteWorldPos.x, absoluteWorldPos.y) + mPosShift);
}

std::pair<CellPos, Vector2D> TransformEditorWidget::deproject(const QVector2D& screenPos) const
{
	QVector2D worldPos = screenPos / mScale - mPosShift;
	return SpatialWorldData::GetTransformedCellPos(CellPos{0, 0}, Vector2D(worldPos.x(), worldPos.y()));
}
