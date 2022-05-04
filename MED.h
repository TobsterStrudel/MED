//
// Created by Ghosh, Anirban on 4/27/22.
//

#ifndef CODE_MED_H
#define CODE_MED_H

#include <random>

#include "CGALComponents.h"

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
    if(D.squared_radius() > 0){
        return D;
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
    std::cout << "made it to function 2" << std::endl;
    unsigned n = P.size();
    std::vector<Circle> D;
    D.reserve(n);
    D[0] = Circle(q1, q2);
    for(unsigned k = 0; k < n; k++){
        std::cout << "function 2 for" << std::endl;
        if(D[k-1].has_on_bounded_side(P[k])){
            std::cout << "function 2 if" << std::endl;
            D[k] = D[k-1];
        }else{
            std::cout << "function 2 else" << std::endl;
            D[k] = Circle(q1, q2, P[k]);
            std::cout << "after function 2 else" << std::endl;
        }
    }
    return D[n-1];
}
Circle miniDiskWithPoint(std::vector<Point> &P, Point q){
    std::cout << "made it to function" << std::endl;
    unsigned n = P.size();
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));
    std::vector<Circle> D;
    D.reserve(n);
    D[1] = Circle(q, P[0]);
    for(unsigned j = 1; j < n; j++){
        std::cout << "function for" << std::endl;
        if(D[j-1].has_on_bounded_side(P[j])){
            std::cout << "function if" << std::endl;
            D[j] = D[j-1];
        }else{
            std::cout << "function else" << std::endl;
            D[j] = miniDiskWithTwoPoints(P, P[j], q);
        }
    }
    return D[n-1];
}
Circle miniDiskIncremental(std::vector<Point> &P) {
    unsigned n = P.size();

    std::vector<Circle> D;
    D.reserve(n);
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));
    D[2] = Circle(P[0], P[1]);
    std::cout << "D2" << D[2] << std::endl;

    for(unsigned i = 2; i < n; i++){
        std::cout << "for" << std::endl;
        if(D[i-1].has_on_bounded_side(P[i])){
            std::cout << "if" << std::endl;
            D[i] = D[i-1];
        }else{
            std::cout << "else" << std::endl;
            D[i] = miniDiskWithPoint(P, P[i]);
        }
    }
    return D[n-1]; /////////////////////////////////// CHECK AT THE END FOR BUGS
}

bool isCoveredby(const std::vector<Point> &P, const Circle &C) {
    for(const Point &p : P)
        if( CGAL::squared_distance(p,C.center() ) > C.squared_radius() + 0.001 )
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

            if(isCoveredby(P,C1) && isCoveredby(copyOfP,C2) && C1.center() == C2.center() && C1.squared_radius() == C2.squared_radius() )
                testsPassed++;
        }
    }

    std::cout << "Tests passed: " << testsPassed << " out of " << totalNumberOfTests << std::endl;
}

#endif //CODE_MED_H
