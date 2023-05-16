//
// Created by User on 15-05-2023.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <chrono>
#include <random>

struct Point {
    int x;
    int y;
};

double calculateDistance(Point p1, Point p2) {
    if (p1.x == p2.x && p1.y == p2.y) {
        return 0.0;
    }
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool comparePointsByX(Point p1, Point p2) {
    return p1.x < p2.x;
}

bool comparePointsByY(Point p1, Point p2) {
    return p1.y < p2.y;
}

std::vector<Point> bruteForceClosestPairs(std::vector<Point>& points, int K) {
    int n = points.size();
    double minDist = std::numeric_limits<double>::infinity();
    std::vector<Point> closestPairs;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = calculateDistance(points[i], points[j]);
            if (dist < minDist) {
                minDist = dist;
                closestPairs = { points[i], points[j] };
            }
            else if (dist == minDist) {
                closestPairs.push_back(points[i]);
                closestPairs.push_back(points[j]);
            }
        }
    }

    return closestPairs;
}

std::vector<Point> mergeResults(std::vector<Point>& leftResult, std::vector<Point>& rightResult, std::vector<Point>& stripResult, int K) {
    std::vector<Point> mergedResult = leftResult;
    mergedResult.insert(mergedResult.end(), rightResult.begin(), rightResult.end());
    mergedResult.insert(mergedResult.end(), stripResult.begin(), stripResult.end());

    std::sort(mergedResult.begin(), mergedResult.end(), [&](Point p1, Point p2) {
        return calculateDistance(p1, p2) < calculateDistance(p2, p1);
    });

    return std::vector<Point>(mergedResult.begin(), mergedResult.begin() + K * 2);
}

std::vector<Point> kClosestPairQueryRecursive(std::vector<Point>& sortedPointsX, std::vector<Point>& sortedPointsY, int K);

std::vector<Point> closestPairsInStrip(std::vector<Point>& sortedPointsY, Point midPoint, double minDist, int K) {
    std::vector<Point> stripPointsY;

    for (const auto& point : sortedPointsY) {
        if (std::abs(point.x - midPoint.x) < minDist) {
            stripPointsY.push_back(point);
        }
    }

    return bruteForceClosestPairs(stripPointsY, K);
}

std::vector<Point> kClosestPairQuery(std::vector<Point>& points, int K) {
    int n = points.size();
    if (K <= 0 || K > n) {
        std::cout << "Invalid value of K" << std::endl;
        return {};
    }

    std::vector<Point> sortedPointsX = points;
    std::vector<Point> sortedPointsY = points;
    std::sort(sortedPointsX.begin(), sortedPointsX.end(), comparePointsByX);
    std::sort(sortedPointsY.begin(), sortedPointsY.end(), comparePointsByY);

    return kClosestPairQueryRecursive(sortedPointsX, sortedPointsY, K);
}

std::vector<Point> kClosestPairQueryRecursive(std::vector<Point>& sortedPointsX, std::vector<Point>& sortedPointsY, int K) {
    int n = sortedPointsX.size();
    if (n <= 3) {
        return bruteForceClosestPairs(sortedPointsX, K);
    }

    int mid = n / 2;

    std::vector<Point> leftPointsX(sortedPointsX.begin(), sortedPointsX.begin() + mid);
    std::vector<Point> rightPointsX(sortedPointsX.begin() + mid, sortedPointsX.end());

    Point midPoint = sortedPointsX[mid];

    std::vector<Point> leftPointsY;
    std::vector<Point> rightPointsY;

    for (const auto& point : sortedPointsY) {
        if (point.x <= midPoint.x) {
            leftPointsY.push_back(point);
        }
        else {
            rightPointsY.push_back(point);
        }
    }

    std::vector<Point> leftResult = kClosestPairQueryRecursive(leftPointsX, leftPointsY, K);
    std::vector<Point> rightResult = kClosestPairQueryRecursive(rightPointsX, rightPointsY, K);

    double minDist = std::min(calculateDistance(leftResult[0], leftResult[1]), calculateDistance(rightResult[0], rightResult[1]));

    std::vector<Point> stripResult = closestPairsInStrip(sortedPointsY, midPoint, minDist, K);

    return mergeResults(leftResult, rightResult, stripResult, K);
}


int main() {
    int K = 3;
    //Generate random datasets for testing (100k | 250k | 500k | 1M | 2M)
    //100k
    std::vector<Point> points100k;
    //Generate a random size number between 100k and 2M
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 2000000);
    int size = dis(gen);
    std::cout << "Size of dataset: " << size << std::endl;
    for (int i = 0; i < size; i++) {
        points100k.push_back({rand() % size, rand() % size});
    }
    //Chrono timer
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Point> closestPairs = kClosestPairQuery(points100k, K);

    //Chrono timer
    auto finish = std::chrono::high_resolution_clock::now();

    //print the time it took to run the algorithm
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    //print the closest pairs
    for (int i = 0; i < closestPairs.size(); i += 2) {
        std::cout << "Pair " << (i / 2) + 1 << ": (" << closestPairs[i].x << ", " << closestPairs[i].y << "), ";
        std::cout << "(" << closestPairs[i + 1].x << ", " << closestPairs[i + 1].y << ")" << std::endl;
    }
    return 0;
}
