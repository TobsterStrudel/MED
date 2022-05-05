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
Circle miniDiskWithTwoPoints(std::vector<Point> &P, Point q1, Point q2, std::vector<Circle> &D){
    unsigned n = P.size();
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));
    D.insert(D.begin(), Circle(q1, q2));

    for(unsigned i = 1; i < n + 1; i++){
        if(D[i-1].has_on_bounded_side(P[i-1]) || D[i-1].has_on_boundary(P[i-1])){
            D.insert(D.begin()+i, D[i-1]);
        }else{
            D.insert(D.begin()+i, Circle(q1, q2, P[i-1]));
        }
    }
    return D[n];
}
Circle miniDiskWithOnePoint(std::vector<Point> &P, Point q, std::vector<Circle> &D){
    unsigned n = P.size();
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));
    D.insert(D.begin()+1, Circle(q, P[0]));

    for(unsigned i = 2; i < n + 1; i++){
        if(D[i-1].has_on_bounded_side(P[i-1]) || D[i-1].has_on_boundary(P[i-1])){
            D.insert(D.begin()+i, D[i-1]);
        }else{
            std::vector<Point> temp = P;
            temp.erase(temp.begin()+(i-1), temp.end());
            D.insert(D.begin()+i, miniDiskWithTwoPoints(temp, P[i-1], q, D));
        }
    }
    return D[n];
}
Circle miniDiskIncremental(std::vector<Point> &P) {
    unsigned n = P.size();
    if(n <= 3){
        return smallCircle(P, n);
    }
    std::shuffle(P.begin(), P.end(), std::mt19937(std::random_device()()));
    std::vector<Circle> D;
    D.reserve(n);
    D.insert(D.begin() + 2, Circle(P[0], P[1]));

    for(unsigned i = 3; i < n + 1; i++){
        if(D[i-1].has_on_bounded_side(P[i-1]) || D[i-1].has_on_boundary(P[i-1])){
            D.insert(D.begin()+i, D[i-1]);
        }else{
            std::vector<Point> temp = P;
            temp.erase(temp.begin()+(i-1), temp.end());
            D.insert(D.begin()+i, miniDiskWithOnePoint(temp, P[i-1], D));
        }
    }
    return D[n];
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
    int num = 1;
    for(unsigned n = 100; n <= 1500; n = n + 100) {
        for(unsigned i = 0; i < numberOfSamples; ++i) {

            std::vector<Point> P, copyOfP;
            generatePointsInsideASquare(n,200,P);

            for(const Point &p : P)
                copyOfP.emplace_back(p);

            Circle C1 = miniDiskNaive(P);
            Circle C2 = miniDiskIncremental(copyOfP);

            if(isCoveredby(P,C1) && isCoveredby(copyOfP,C2) && fabs(C1.center().x() - C2.center().x()) < 0.0001 && fabs(C1.center().y() - C2.center().y()) < 0.0001 && fabs(C1.squared_radius() - C2.squared_radius()) < 0.0001 ){
                testsPassed++;
                std::cout << "Passed #" << testsPassed << std::endl;
            }

        }
        std::cout << "Testing finished: " << num++ << std::endl;
    }

    std::cout << "Tests passed: " << testsPassed << " out of " << totalNumberOfTests << std::endl;
}
Circle smallCircle(std::vector<Point> &P, unsigned n){
    switch (n) {
        case 0:
            return Circle(Point(0.0, 0.0));
        case 1:
            return Circle(P[0]);
        case 2:
            return Circle(P[0], P[1]);
        case 3:
            return Circle(P[0], P[1], P[2]);
    }
}
void plot() {
    unsigned testsPassed = 0;
    std::ofstream file ("a.txt");
    file << "n-value" << "\t" << "miniDiskNaive" << "\t" << "miniDiskIncremental" << std::endl;

    for(int n = 100; n <= 1500; n+=100){
        double naiveTime;
        double incrementTime;
        for(int i = 0; i < 5; i++) {
            std::vector<Point> P, copyOfP;
            generatePointsInsideASquare(n,200,P);

            for(const Point &p : P)
                copyOfP.emplace_back(p);

            CGAL::Timer timer;
            timer.start();
            Circle C1 = miniDiskNaive(P);
            timer.stop();
            naiveTime += timer.time() * 1000;

            timer.reset();
            timer.start();
            Circle C2 = miniDiskIncremental(P);
            timer.stop();
            incrementTime += timer.time() * 1000;
            if(isCoveredby(P,C1) && isCoveredby(copyOfP,C2) && fabs(C1.center().x() - C2.center().x()) < 0.0001 && fabs(C1.center().y() - C2.center().y()) < 0.0001 && fabs(C1.squared_radius() - C2.squared_radius()) < 0.0001 ){
                testsPassed++;
            }

        }

        file << n << "\t" << naiveTime/5 << "ms" << "\t" << incrementTime/5 << "ms" << std::endl;
    }
    std::cout << "Tests passed: " << testsPassed << " out of " << 75 << std::endl;
    file.close();
}
#endif //CODE_MED_H
