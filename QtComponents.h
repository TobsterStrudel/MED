//
// Created by Tobias Dault on 5/3/22.
//

#ifndef MED_QTCOMPONENTS_H
#define MED_QTCOMPONENTS_H

#include <vector>
#include <QApplication>
#include <QPicture>
#include <QPainter>
#include <QLabel>
#include "CGALComponents.h"

void drawMEDUsingQT(const std::vector<Point> &P, const Circle &C, const bool labels) {

    const double pointSize = 4; // adjust this value to change the size of the points
    /***************************************************/
    QPicture pi;
    QPainter canvas(&pi);
    canvas.setRenderHint(QPainter::Antialiasing);
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/


    unsigned id = 0;
    for ( Point p : P ) {
        canvas.setBrush(Qt::black);

        canvas.drawEllipse(QPointF(p.x(), p.y()), pointSize, pointSize);
        if(labels) {
            QPointF textPos(p.x() + 4.0, p.y() + 4.0);
            canvas.drawText(textPos, QString(std::to_string(id).c_str()));
        }
        id++;
    }
    Point center = C.center();
    double radius = sqrt(C.squared_radius());
    std::cout << "Center: " << center.x() << " : " << center.y() << "\nRadius: " << radius << std::endl;
    canvas.setBrush(Qt::blue);
    canvas.setOpacity(0.5);
    canvas.drawEllipse(QPointF(center.x(), center.y()), radius, radius);

    /*********************************************/
    canvas.end();
    auto l = new QLabel;
    l->setStyleSheet("QLabel { background-color : white; color : black; }");
    l->setPicture(pi);
    l->setWindowTitle("MED");
    l->show();
    // l->showMaximized();
    QApplication::exec();
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/
}

#endif //MED_QTCOMPONENTS_H
