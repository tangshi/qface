/**
 *  qface
 *
 *  main.cpp
 *
 *  Created by tangshi on 2016/10/20.
 *  Copyright © 2016年 tangshi. All rights reserved.
 */

#include <QApplication>
#include "mainwindow.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}




