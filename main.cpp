#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

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

int main() {
    // Example usage
    std::vector<Point> points = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
    int K = 3;

    std::vector<DistancePair> closestPairs = kClosestPairQuery(points, K);

    for (const auto& pair : closestPairs) {
        std::cout << "Points: (" << pair.points.point1.x << ", " << pair.points.point1.y << ") and ("
                  << pair.points.point2.x << ", " << pair.points.point2.y << "), Distance: " << pair.distance << std::endl;
    }

    return 0;
}
