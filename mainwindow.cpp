#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "facedata.h"
#include "facewindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mFaceData = new FaceData;
    mFaceWindow = new FaceWindow(mFaceData);
    mFaceWindow->setVisible(false);
    mFaceWindow->resize(960, 720);
    mFaceWindow->setAnimating(false);

    ui->setupUi(this);
    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(loadFaceData()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mFaceData;
    delete mFaceWindow;
}

void MainWindow::loadFaceData()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Face data (*.mat)"));

    if (path.isEmpty()) {
        return;
    }

    mFaceData->load_data(path.toStdString().c_str(), 80);

    mFaceWindow->show();

    int posx = x() + (width() - mFaceWindow->width()) / 2;
    int posy = y() + (height() - mFaceWindow->height()) / 2;

    mFaceWindow->setPosition(posx, posy);
}
