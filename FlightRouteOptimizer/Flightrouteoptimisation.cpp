#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class FlightRouteOptimizer {
private:
    unordered_map<string, vector<pair<string, pair<double, double>>>> flights;

public:
    void addAirport(const string& name) {
        flights[name];
    }

    void addFlight(const string& from, const string& to, double cost, double duration) {
        if (flights.count(from) && flights.count(to)) {
            flights[from].push_back({to, {cost, duration}});
        } else {
            cout << "One or both airports not found!" << endl;
        }
    }

    void findShortestRoute(const string& start, const string& end) {
        if (!flights.count(start) || !flights.count(end)) {
            cout << "One or both airports not found!" << endl;
            return;
        }
        unordered_map<string, double> minCost;
        unordered_map<string, string> previous;
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
        for (auto& airport : flights) minCost[airport.first] = numeric_limits<double>::infinity();
        minCost[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [cost, airport] = pq.top(); pq.pop();
            if (airport == end) break;
            for (auto& [dest, flight] : flights[airport]) {
                double newCost = cost + flight.first;
                if (newCost < minCost[dest]) {
                    minCost[dest] = newCost;
                    previous[dest] = airport;
                    pq.push({newCost, dest});
                }
            }
        }

        vector<string> path;
        for (string at = end; at != ""; at = previous[at]) path.push_back(at);
        reverse(path.begin(), path.end());

        if (path.front() == start) {
            cout << "Shortest route from " << start << " to " << end << ": ";
            for (const auto& airport : path) cout << airport << " ";
            cout << "with total cost: " << minCost[end] << endl;
        } else {
            cout << "No route found!" << endl;
        }
    }

    void saveData(const string& filename) {
        ofstream file(filename);
        for (auto& [airport, dests] : flights) {
            file << airport << endl;
            for (auto& [dest, flight] : dests) file << dest << " " << flight.first << " " << flight.second << endl;
        }
    }

    void loadData(const string& filename) {
        ifstream file(filename);
        string airport, line;
        while (getline(file, airport)) {
            addAirport(airport);
            while (getline(file, line) && !line.empty()) {
                stringstream ss(line);
                string dest; double cost, duration;
                ss >> dest >> cost >> duration;
                addFlight(airport, dest, cost, duration);
            }
        }
    }
};

int main() {
    FlightRouteOptimizer optimizer;
    optimizer.addAirport("JFK");
    optimizer.addAirport("LAX");
    optimizer.addAirport("ORD");
    optimizer.addAirport("DFW");
    optimizer.addFlight("JFK", "LAX", 300, 6);
    optimizer.addFlight("JFK", "ORD", 200, 3);
    optimizer.addFlight("ORD", "LAX", 150, 4);
    optimizer.addFlight("JFK", "DFW", 250, 5);
    optimizer.addFlight("DFW", "LAX", 200, 4);
    optimizer.findShortestRoute("JFK", "LAX");
    return 0;
}
