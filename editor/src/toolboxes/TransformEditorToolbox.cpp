#include "TransformEditorToolbox.h"

#include "src/mainwindow.h"

#include "src/editorcommands/changeentitycommand.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QPainter>
#include <QMouseEvent>

#include "Components/TransformComponent.generated.h"

const QString TransformEditorToolbox::WidgetName = "TransformEditor";
const QString TransformEditorToolbox::ToolboxName = TransformEditorToolbox::WidgetName + "Toolbox";

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
	if (mContent)
	{
		mContent->mSelectedEntity = entity;
		mContent->repaint();
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
	mPressMousePos = mLastMousePos;
	mIsMoved = false;
	mIsCatchedSelectedEntity = false;

	if (mSelectedEntity.isValid())
	{
		Vector2D worldPos = deproject(mLastMousePos);
		auto [transform] = mWorld->getEntityManger().getEntityComponents<TransformComponent>(mSelectedEntity.getEntity());
		Vector2D location = transform->getLocation();
		if (location.x - 10 < worldPos.x && location.x + 10 > worldPos.x
			&&
			location.y - 10 < worldPos.y && location.y + 10 > worldPos.y)
		{
			mIsCatchedSelectedEntity = true;
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
		auto [transform] = mWorld->getEntityManger().getEntityComponents<TransformComponent>(mSelectedEntity.getEntity());
		transform->setLocation(deproject(pos));
	}

	if (!mIsCatchedSelectedEntity)
	{
		mPosShift -= QVector2D(mLastMousePos - pos);
	}
	mLastMousePos = pos;

	repaint();
}

void TransformEditorWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (!mIsMoved)
	{
		onClick(event->pos());
	}
}

void TransformEditorWidget::paintEvent(QPaintEvent*)
{
	if (mWorld == nullptr)
	{
		return;
	}

	QPainter painter(this);

	std::vector<std::tuple<TransformComponent*>> components = mWorld->getEntityManger().getComponents<TransformComponent>();

	for (auto [component] : components)
	{
		Vector2D location = component->getLocation();
		QRectF rectangle((project(location) - QVector2D(10.0f, 10.0f)).toPoint(), QSize(10, 10));
		painter.drawEllipse(rectangle);
	}

	if (mSelectedEntity.isValid())
	{
		auto [transform] = mWorld->getEntityManger().getEntityComponents<TransformComponent>(mSelectedEntity.getEntity());
		QRectF rectangle((project(transform->getLocation()) - QVector2D(10.0f, 10.0f)).toPoint(), QSize(10, 10));
		QBrush brush = painter.brush();
		brush.setColor(Qt::GlobalColor::blue);
		painter.setBrush(brush);
		painter.drawRect(rectangle);
	}
}

void TransformEditorWidget::resizeEvent(QResizeEvent*)
{

}

void TransformEditorWidget::onClick(const QPoint& pos)
{
	if (mWorld == nullptr)
	{
		return;
	}

	Vector2D worldPos = deproject(QVector2D(pos));

	NullableEntity findResult;

	mWorld->getEntityManger().forEachEntity2<TransformComponent>([worldPos, &findResult](Entity entity, TransformComponent* transform){
		Vector2D location = transform->getLocation();
		if (location.x - 10 < worldPos.x && location.x + 10 > worldPos.x
			&&
			location.y - 10 < worldPos.y && location.y + 10 > worldPos.y)
		{
			findResult = entity;
		}
	});

	mSelectedEntity = findResult;

	if (findResult.isValid())
	{
		mMainWindow->OnSelectedEntityChanged.broadcast(findResult);
	}

	repaint();
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
