//
// Created by Ghosh, Anirban on 4/27/22.
//

#ifndef CODE_MED_H
#define CODE_MED_H

#include "CGALComponents.h"

Circle miniDiskNaive(std::vector<Point> &P) {
    Circle D;

    // complete this part

    return D;
}

Circle miniDiskIncremental(std::vector<Point> &P) {
    Circle D;

    // complete this part

    return D;
}

bool isCoveredby(const std::vector<Point> &P, const Circle &C) {
    for(const Point &p : P)
        if( CGAL::squared_distance(p,C.center() ) > C.squared_radius() )
            return false;
    return true;
}

void tester() {

    const unsigned totalNumberOfTests = 15 * 5;
    unsigned testsPassed = 0;

    unsigned numberOfSamples = 5;
    for(unsigned n = 100; n <= 1500; n = n + 100) {
        for(unsigned i = 0; i < numberOfSamples; ++i) {

            std::vector<Point> P, copyOfP;
            generatePointsInsideASquare(n,200,P);

            for(const Point &p : P)
                copyOfP.emplace_back(p);

            Circle C1 = miniDiskNaive(P);
            Circle C2 = miniDiskIncremental(copyOfP);

            if(isCoveredby(P,C1) && isCoveredby(copyOfP,C2) &&
               C1.center() == C2.center() && C1.squared_radius() == C2.squared_radius() )
                testsPassed++;
        }
    }

    std::cout << "Tests passed: " << testsPassed << " out of " << totalNumberOfTests << std::endl;
}

#endif //CODE_MED_H
