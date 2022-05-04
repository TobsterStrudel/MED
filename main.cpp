//
// Created by tobias on 5/2/22.
//

#ifndef MED_MAIN_H
#define MED_MAIN_H


#include <iostream>
#include <QApplication>
#include <QPicture>
#include "MED.h"
#include "QtComponents.h"


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

#endif //MED_MAIN_H
