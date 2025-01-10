#include <stdio.h>
#include <limits.h>

#define MAX_CITIES 4
#define INF 9999

int tsp(int n, int graph[MAX_CITIES][MAX_CITIES]) {
    int subsets = 1 << n; // Total subsets for n cities
    int dp[subsets][n];

    // Initialize DP table
    for (int i = 0; i < subsets; i++) {
        for (int j = 0; j < n; j++) {
            dp[i][j] = INF;
        }
    }

    // Base case: Starting at city A (index 0)
    dp[1][0] = 0;

    // Fill DP table
    for (int mask = 0; mask < subsets; mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue; // If city u is not in the subset

            for (int v = 0; v < n; v++) {
                if (mask & (1 << v)) continue; // If city v is already in the subset
                if (u == v) continue;         // Ignore diagonal values (self-loops)

                int new_mask = mask | (1 << v);
                int new_cost = dp[mask][u] + graph[u][v];
                if (new_cost < dp[new_mask][v]) {
                    dp[new_mask][v] = new_cost;
                }
            }
        }
    }

    // Find the shortest path to complete the tour and return to the starting node
    int min_cost = INF;
    for (int i = 1; i < n; i++) {
        int tour_cost = dp[subsets - 1][i] + graph[i][0];
        if (tour_cost < min_cost) {
            min_cost = tour_cost;
        }
    }

    return min_cost;
}

int main() {
    int graph[MAX_CITIES][MAX_CITIES] = {
        {0, 21, 14, 15},  // Distances from A
        {21, 0, 22, 4},   // Distances from B
        {14, 22, 0, 3},   // Distances from C
        {15, 4, 3, 0}     // Distances from D
    };

    int n = MAX_CITIES;
    int result = tsp(n, graph);
    printf("The shortest distance to solve the TSP is: %d\n", result);
    return 0;
}

