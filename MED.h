//
// Created by Ghosh, Anirban on 4/27/22.
//

#ifndef CODE_MED_H
#define CODE_MED_H

#include <random>

#include "CGALComponents.h"
Circle smallCircle(std::vector<Point> &P, unsigned n);

Circle miniDiskNaive(std::vector<Point> &P) {
    Circle D;
    unsigned n = P.size();
    double min = MAXFLOAT;
    for(unsigned i = 0; i < n; i++){
        for(unsigned j = 0; j < n; j++){
            if(j == i){
                continue;
            }
            auto temp = Circle(P[i], P[j]);
            bool ticket = true;
            for(unsigned k = 0; k < n; k++){
                if(k == j || k == i){
                    continue;
                }
                if(temp.has_on_unbounded_side(P[k])){
                    ticket = false;
                    break;
                }
            }
            if(ticket && (temp.squared_radius()) < min){
                min = temp.squared_radius();
                D = Circle(P[i], P[j]);
            }
        }
    }
    for(unsigned i = 0; i < n; i++){
        for(unsigned j = 0; j < n; j++){
            if(j == i){
                continue;
            }
            for(unsigned l = 0; l < n; l++){
                if(l == i || l == j){
                    continue;
                }
                auto temp = Circle(P[i], P[j], P[l]);
                bool ticket = true;
                for(unsigned k = 0; k < n; k++){
                    if(k == j || k == i || k == l){
                        continue;
                    }
                    if(temp.has_on_unbounded_side(P[k])){
                        ticket = false;
                        break;
                    }
                }
                if(ticket && (temp.squared_radius()) < min){
                    min = temp.squared_radius();
                    D = Circle(P[i], P[j], P[l]);
                }
            }

        }
    }
    return D;
}
Circle miniDiskWithTwoPoints(std::vector<Point> &P, Point q1, Point q2){
    unsigned n = P.size();
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));

    Circle D = Circle(q1, q2);
    for(unsigned i = 0; i < n; i++){
        if(D.has_on_bounded_side(P[i]) || D.has_on_boundary(P[i])){

        }else{
            D = Circle(q1, q2, P[i]);
        }
    }
    return D;
}
Circle miniDiskWithOnePoint(std::vector<Point> &P, Point q){
    unsigned n = P.size();
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));
    Circle D = Circle(q, P[0]);
    for(unsigned i = 1; i < n; i++){
        if(D.has_on_bounded_side(P[i]) || D.has_on_boundary(P[i])){

        }else{
            std::vector<Point> temp = P;
            temp.erase(temp.begin()+(i), temp.end());
            D = miniDiskWithTwoPoints(temp, P[i], q);
        }
    }
    return D;
}
Circle miniDiskIncremental(std::vector<Point> &P) {
    unsigned n = P.size();
    if(n <= 3){
        return smallCircle(P, n);
    }
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));
    Circle D;
    D= Circle(P[0], P[1]);

    for(unsigned i = 2; i < n; i++){
        if(D.has_on_bounded_side(P[i]) || D.has_on_boundary(P[i])){

        }else{
            std::vector<Point> temp = P;
            temp.erase(temp.begin()+(i), temp.end());
            D = miniDiskWithOnePoint(temp, P[i]);
        }
    }

    return D;
}

bool isCoveredby(const std::vector<Point> &P, const Circle &C) {
    for(const Point &p : P)
        if( abs(CGAL::squared_distance(p,C.center() ) - C.squared_radius()) > 0.0001)
            return false;
    return true;
}

void tester() {

    const unsigned totalNumberOfTests = 15 * 5;
    unsigned testsPassed = 0;

    unsigned numberOfSamples = 5;
    int num = 1;
    for(unsigned n = 100; n <= 1500; n = n + 100) {
        for(unsigned i = 0; i < numberOfSamples; ++i) {

            std::vector<Point> P, copyOfP;
            generatePointsInsideASquare(n,200,P);

            for(const Point &p : P)
                copyOfP.emplace_back(p);

            Circle C1 = miniDiskNaive(P);
            Circle C2 = miniDiskIncremental(copyOfP);

            if(isCoveredby(P,C1) && isCoveredby(copyOfP,C2) && C1.center() == C2.center() && C1.squared_radius() == C2.squared_radius() )
                testsPassed++;
        }
        std::cout << "Testing finished: " << num++ << std::endl;
    }

    std::cout << "Tests passed: " << testsPassed << " out of " << totalNumberOfTests << std::endl;
}
Circle smallCircle(std::vector<Point> &P, unsigned n){
    if(n == 3){
        return Circle(P[0], P[1], P[2]);
    }else if(n == 2){
        return Circle(P[0], P[1]);
    }else if(n == 1){
        return Circle(P[0]);
    }else{
        return Circle(Point(0.0, 0.0));
    }
}

#endif //CODE_MED_H
