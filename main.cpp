#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>

// Structure to represent a point
struct Point {
    int x;
    int y;
};

// Structure to represent a pair of points
struct PointPair {
    Point point1;
    Point point2;
};

// Structure to represent a pair of points with distance
struct DistancePair {
    double distance;
    PointPair points;
};

// Function to calculate the Euclidean distance between two points
double calculateDistance(Point p1, Point p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return std::sqrt(dx*dx + dy*dy);
}

// Comparator for the max-heap based on distance
struct CompareDistance {
    bool operator()(const DistancePair& p1, const DistancePair& p2) {
        return p1.distance < p2.distance;
    }
};

std::vector<DistancePair> kClosestPairQuery(std::vector<Point>& points, int K) {
    int n = points.size();
    if (K <= 0 || K > n) {
        std::cout << "Invalid value of K" << std::endl;
        return {};
    }

    std::priority_queue<DistancePair, std::vector<DistancePair>, CompareDistance> maxHeap;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if(points[i].x == points[j].x && points[i].y == points[j].y) {
                continue;
            }
            double distance = calculateDistance(points[i], points[j]);
            if (maxHeap.size() < K) {
                maxHeap.push({distance, {points[i], points[j]}});
            } else if (distance < maxHeap.top().distance) {
                maxHeap.pop();
                maxHeap.push({distance, {points[i], points[j]}});
            }
        }
    }

    std::vector<DistancePair> result;

    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    return result;
}

void printKCPQ(std::vector<DistancePair>& result) {
    for (int i = 0; i < result.size(); i++) {
        std::cout << "Distance: " << result[i].distance << " Point1: (" << result[i].points.point1.x << ", " << result[i].points.point1.y << ") Point2: (" << result[i].points.point2.x << ", " << result[i].points.point2.y << ")" << std::endl;
    }
}

int main() {
    int K = 3;
    //Generate random datasets for testing (100k | 250k | 500k | 1M | 2M)
    //100k
    std::vector<Point> points100k;
    //Generate a random size number between 100k and 2M
    int size = rand() % 1900000 + 100000;
    std::cout << "Size of dataset: " << size << std::endl;
    for (int i = 0; i < size; i++) {
        points100k.push_back({rand() % size, rand() % size});
    }
    //Chrono timer
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<DistancePair> closestPairs = kClosestPairQuery(points100k, K);

    //Chrono timer
    auto finish = std::chrono::high_resolution_clock::now();

    //print the time it took to run the algorithm
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    //print the closest pairs
    printKCPQ(closestPairs);

    //empty the closestPairs vector
    closestPairs.clear();
    /*
    //250k
    std::vector<Point> points250k;
    for (int i = 0; i < 250000; i++) {
        points250k.push_back({rand() % 250000, rand() % 250000});
    }
    closestPairs = kClosestPairQuery(points250k, K);

    //print the closest pairs
    printKCPQ(closestPairs);

    //empty the closestPairs vector
    closestPairs.clear();


    //500k
    std::vector<Point> points500k;
    for (int i = 0; i < 500000; i++) {
        points500k.push_back({rand() % 500000, rand() % 500000});
    }

    closestPairs = kClosestPairQuery(points500k, K);

    //print the closest pairs
    printKCPQ(closestPairs);

    //empty the closestPairs vector
    closestPairs.clear();

    //1M
    std::vector<Point> points1M;
    for (int i = 0; i < 1000000; i++) {
        points1M.push_back({rand() % 1000000, rand() % 1000000});
    }

    closestPairs = kClosestPairQuery(points1M, K);

    //print the closest pairs
    printKCPQ(closestPairs);

    //empty the closestPairs vector
    closestPairs.clear();

    //2M
    std::vector<Point> points2M;
    for (int i = 0; i < 2000000; i++) {
        points2M.push_back({rand() % 2000000, rand() % 2000000});
    }

    closestPairs = kClosestPairQuery(points2M, K);

    //print the closest pairs
    printKCPQ(closestPairs);
    */

    return 0;
}
