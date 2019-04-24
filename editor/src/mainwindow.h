#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

#include <Modules/ComponentFactory.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	 void on_actionNew_World_triggered();

	 void on_actionOpen_World_triggered();

	 void on_controlledEntityCombobox_currentIndexChanged(const QString &arg1);

	 void on_cameraEntityCombobox_currentIndexChanged(const QString &arg1);

	 void on_actionSave_World_As_triggered();

	 void on_actionSave_World_triggered();

	 void on_actionRun_Game_triggered();

	 void on_actionUndo_triggered();

	 void on_actionRedo_triggered();

private:
	 void createWorld();
	 void updateWorldData();
	 void updateSelectedEntityComponents();
	 void updateSelectedComponentData();

private:
	Ui::MainWindow *ui;
	std::unique_ptr<class World> mCurrentWorld;
	ComponentFactory mComponentFactory;
	std::string mOpenedWorldPath;
};

#endif // MAINWINDOW_H
