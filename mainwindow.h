#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class FaceData;
class FaceWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void loadFaceData();

private:
    FaceData *mFaceData;
    FaceWindow *mFaceWindow;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
