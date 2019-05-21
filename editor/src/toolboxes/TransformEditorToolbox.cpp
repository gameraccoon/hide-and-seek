#include "TransformEditorToolbox.h"

#include "src/mainwindow.h"

#include "src/editorcommands/changeentitygrouplocationcommand.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QApplication>

#include "Components/TransformComponent.generated.h"
#include "Components/CollisionComponent.generated.h"

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
	mOnSelectedEntityChangedHandle = mMainWindow->OnSelectedEntityChanged.bind([this](NullableEntity entity){onEntitySelected(entity);});
}

TransformEditorToolbox::~TransformEditorToolbox()
{
	mMainWindow->OnWorldChanged.unbind(mOnWorldChangedHandle);
	mMainWindow->OnSelectedEntityChanged.unbind(mOnSelectedEntityChangedHandle);
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

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addStretch();
	mContent->setLayout(layout);
	QCheckBox* freeMoveCheckbox = new QCheckBox();
	freeMoveCheckbox->setText("Free Move");
	freeMoveCheckbox->setChecked(mContent->mFreeMove);
	QObject::connect(freeMoveCheckbox, &QCheckBox::stateChanged, this, &TransformEditorToolbox::onFreeMoveChanged);
	layout->addWidget(freeMoveCheckbox);
	mContent->OnEntitiesMoved.assign([this](std::vector<Entity> entities, const Vector2D& shift){onEntitiesMoved(entities, shift);});
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

void TransformEditorToolbox::onEntitySelected(NullableEntity entity)
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
	if (entity.isValid() && world->getEntityManger().doesEntityHaveComponent<TransformComponent>(entity.getEntity()))
	{
		mContent->mSelectedEntities.push_back(entity.getEntity());
	}
	mContent->repaint();
}

void TransformEditorToolbox::onEntitiesMoved(std::vector<Entity> entities, const Vector2D &shift)
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

	if (NullableEntity entityUnderCursor = getEntityUnderPoint(event->pos()); entityUnderCursor.isValid())
	{
		Entity entity = entityUnderCursor.getEntity();
		if (std::find(mSelectedEntities.begin(), mSelectedEntities.end(), entity) != mSelectedEntities.end())
		{
			mIsCatchedSelectedEntity = true;
		}
		else if (mFreeMove && !IsCtrlPressed())
		{
			mSelectedEntities.clear();
			mSelectedEntities.push_back(entity);
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
		mMoveShift = deproject(pos) - deproject(mPressMousePos);
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
			addEntitiesInRectToSelection(deproject(mPressMousePos), deproject(QVector2D(event->pos())));
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

	mWorld->getEntityManger().forEachEntity2<TransformComponent>([&painter, this](Entity entity, TransformComponent* transform)
	{
		Vector2D location = transform->getLocation();

		auto [collision] = mWorld->getEntityManger().getEntityComponents<CollisionComponent>(entity);

		if (std::find(mSelectedEntities.begin(), mSelectedEntities.end(), entity) != mSelectedEntities.end())
		{
			// preview the movement
			location += mMoveShift;

			// calc selected entity border
			QVector2D selectionLtShift;
			QVector2D selectionSize;
			if (collision)
			{
				Hull geometry = collision->getGeometry();
				if (geometry.type == Hull::Type::Angular)
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
			QRectF rectangle((project(location) + selectionLtShift).toPoint(), QSize(static_cast<int>(selectionSize.x()), static_cast<int>(selectionSize.y())));
			QBrush brush = painter.brush();
			brush.setColor(Qt::GlobalColor::blue);
			painter.setBrush(brush);
			painter.drawRect(rectangle);
		}

		// draw collision
		if (collision)
		{
			Hull geometry = collision->getGeometry();
			if (geometry.type == Hull::Type::Angular)
			{
				QPolygonF polygon;
				for (Vector2D& point : geometry.points)
				{
					polygon.append(project(location + point).toPointF());
				}
				painter.drawPolygon(polygon);
			}
			else
			{
				float radius = geometry.getRadius();
				float halfWorldSize = radius * mScale;
				int worldSizeInt = static_cast<int>(halfWorldSize * 2.0f);
				QRectF rectangle((project(location) - QVector2D(halfWorldSize, halfWorldSize)).toPoint(), QSize(worldSizeInt, worldSizeInt));
				painter.drawEllipse(rectangle);
			}
		}

		// draw entity location cross
		QVector2D screenLocation = project(location);
		QPoint screenPoint(static_cast<int>(screenLocation.x()), static_cast<int>(screenLocation.y()));
		painter.drawLine(QPoint(screenPoint.x() - 5, screenPoint.y()), QPoint(screenPoint.x() + 5, screenPoint.y()));
		painter.drawLine(QPoint(screenPoint.x(), screenPoint.y() - 5), QPoint(screenPoint.x(), screenPoint.y() + 5));
	});

	if (mIsRectangleSelection)
	{
		QVector2D size = QVector2D(mLastMousePos) - mPressMousePos;
		QRect rect(mPressMousePos.toPoint(), QSize(static_cast<int>(size.x()), static_cast<int>(size.y())));
		painter.drawRect(rect);
	}
}

void TransformEditorWidget::onClick(const QPoint& pos)
{
	NullableEntity findResult = getEntityUnderPoint(pos);

	if (IsCtrlPressed())
	{
		if (findResult.isValid())
		{
			Entity foundEntity = findResult.getEntity();
			auto it = std::find(mSelectedEntities.begin(), mSelectedEntities.end(), foundEntity);
			if (it != mSelectedEntities.end())
			{
				mSelectedEntities.erase(it);
			}
			else
			{
				mSelectedEntities.push_back(foundEntity);
			}
		}
	}
	else
	{
		mSelectedEntities.clear();

		if (findResult.isValid())
		{
			mSelectedEntities.push_back(findResult.getEntity());
			mMainWindow->OnSelectedEntityChanged.broadcast(findResult);
		}
	}
}

NullableEntity TransformEditorWidget::getEntityUnderPoint(const QPoint& pos)
{
	Vector2D worldPos = deproject(QVector2D(pos));

	NullableEntity findResult;

	if (mWorld)
	{
		mWorld->getEntityManger().forEachEntity2<TransformComponent>([worldPos, &findResult](Entity entity, TransformComponent* transform){
			Vector2D location = transform->getLocation();
			if (location.x - 10 < worldPos.x && location.x + 10 > worldPos.x
				&&
				location.y - 10 < worldPos.y && location.y + 10 > worldPos.y)
			{
				findResult = entity;
			}
		});
	}

	return findResult;
}

void TransformEditorWidget::addEntitiesInRectToSelection(const Vector2D &start, const Vector2D &end)
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
		mWorld->getEntityManger().forEachEntity2<TransformComponent>([this, lt, rd](Entity entity, TransformComponent* transform)
		{
			Vector2D location = transform->getLocation();
			if (lt.x < location.x && location.x < rd.x && lt.y < location.y && location.y < rd.y)
			{
				auto it = std::find(mSelectedEntities.begin(), mSelectedEntities.end(), entity);
				if (it == mSelectedEntities.end())
				{
					mSelectedEntities.push_back(entity);
				}
			}
		});
	}
}

QVector2D TransformEditorWidget::project(const Vector2D& worldPos)
{
	return mScale * (QVector2D(worldPos.x, worldPos.y) + mPosShift);
}

Vector2D TransformEditorWidget::deproject(const QVector2D& screenPos)
{
	QVector2D worldPos = screenPos / mScale - mPosShift;
	return Vector2D(worldPos.x(), worldPos.y());
}
