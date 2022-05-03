//
// Created by tobias on 5/2/22.
//

#ifndef MED_MAIN_H
#define MED_MAIN_H


#include <iostream>
#include <QApplication>
#include <QPicture>
#include <QPainter>
#include <QLabel>
#include "MED.h"

void drawMEDUsingQT(const std::vector<Point> &P, const Circle &C, bool labels);

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    std::vector<Point> P, copyOfP;
    generatePointsInsideASquare(10,200,P);

    for(const Point &p : P)
        copyOfP.emplace_back(p);

    Circle C1 = miniDiskNaive(P);
    drawMEDUsingQT(P, C1, true);
    if(isCoveredby(P,C1)){
        std::cout << "success";
    }else{
        std::cout << "failure";
    }
}
void drawMEDUsingQT(const std::vector<Point> &P, const Circle &C, const bool labels) {

    const double pointSize = 4; // adjust this value to change the size of the points
    /***************************************************/
    QPicture pi;
    QPainter canvas(&pi);
    canvas.setRenderHint(QPainter::Antialiasing);
    //canvas.setFont(QFont("Times", 12));
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
    canvas.drawEllipse(QPointF(center.x(), center.y()), radius, radius);

    /*********************************************/
    canvas.end();
    auto l = new QLabel;
    l->setStyleSheet("QLabel { background-color : white; color : black; }");
    l->setPicture(pi);
    l->setWindowTitle("Polygon ConvexHull");
    l->show();
    // l->showMaximized();
    QApplication::exec();
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/
}


#endif //MED_MAIN_H
