#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>
#include <random>

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

    std::vector<DistancePair> closestPairs = kClosestPairQuery(points100k, K);

    //Chrono timer
    auto finish = std::chrono::high_resolution_clock::now();

    //print the time it took to run the algorithm
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    //print the closest pairs
    printKCPQ(closestPairs);
    return 0;
}
