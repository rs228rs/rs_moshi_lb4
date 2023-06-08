#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
using namespace std;

const int sim = 10; // кількість симуляцій
const int maxcity = 35; // максимум міст
const int mincity = 25; // мінімум міст
const int maxdist = 100; // максимальна дистанція
const int mindist = 10; // мінімальна дистанція
const double a = 1.0;  // вага феромону
const double b = 5.0;  // вага видимості
const double e = 0.5;  // швидкість випаровування феромону

struct citystruct {
    double x;
    double y;
};

class antalgoritm {
private:
    vector<vector<double>> dmatrix;
    vector<citystruct> city;
    vector<int> best;
    double dbest;

    double dfind(const citystruct& city1, const citystruct& city2) {
        double dX = city1.x - city2.x;
        double dY = city1.y - city2.y;
        return sqrt(dX * dX + dY * dY);
    }

    void generatematrix() {
        int ncity = city.size();
        dmatrix.resize(ncity, vector<double>(ncity, 0.0));
        for (int i = 0; i < ncity; i++) {
            for (int j = 0; j < ncity; j++) {
                if (i != j) {
                    double d = dfind(city[i], city[j]);
                    dmatrix[i][j] = d;
                    dmatrix[j][i] = d;
                }
            }
        }
    }

    int nextcity(int ant, const vector<bool>& visited, const vector<double>& chance) {
        double r = ((double)rand() / (RAND_MAX));
        double t = 0.0;
        for (int i = 0; i < city.size(); i++) {
            if (!visited[i]) {
                t += chance[i];
                if (r <= t) {
                    return i;
                }
            }
        }
        for (int i = 0; i < city.size(); i++) {
            if (!visited[i]) {
                return i;
            }
        }
        return -1;
    }

    void optimization() {
        int ncity = city.size();
        vector<vector<double>> pmatrix(ncity, vector<double>(ncity, 1.0));

        for (int i = 0; i < sim; i++) {
            vector<vector<double>> dpmatrix(ncity, vector<double>(ncity, 0.0));

            for (int t = 0; t < ncity; t++) {
                int start = rand() % ncity;
                vector<bool> visited(ncity, false);
                vector<int> tour;
                double l = 0.0;

                tour.push_back(start);
                visited[start] = true;

                for (int j = 0; j < ncity - 1; j++) {
                    int thiscity = tour.back();
                    vector<double> chance(ncity, 0.0);
                    double chancesum = 0.0;

                    for (int k = 0; k < ncity; k++) {
                        if (!visited[k]) {
                            chance[k] = pow(pmatrix[thiscity][k], a) *
                                pow(1.0 / dmatrix[thiscity][k], b);
                            chancesum += chance[k];
                        }
                    }

                    for (int k = 0; k < ncity; k++) {
                        chance[k] /= chancesum;
                    }

                    int next = nextcity(t, visited, chance);
                    tour.push_back(next);
                    visited[next] = true;
                    l += dmatrix[thiscity][next];
                }

                l += dmatrix[tour.back()][start];

                if (l < dbest) {
                    best = tour;
                    dbest = l;
                }
                for (int j = 0; j < ncity - 1; j++) {
                    int city1 = tour[j];
                    int city2 = tour[j + 1];
                    dpmatrix[city1][city2] += 1.0 / l;
                    dpmatrix[city2][city1] += 1.0 / l;
                }
            }
            for (int j = 0; j < ncity; j++) {
                for (int k = 0; k < ncity; k++) {
                    pmatrix[j][k] = (1.0 - e) * pmatrix[j][k] +
                        dpmatrix[j][k];
                }
            }
        }
    }

public:
    antalgoritm(const vector<citystruct>& city) : city(city), dbest(numeric_limits<double>::max()) {}
    void solve() {
        generatematrix();
        optimization();
    }
    vector<int> getbest() const {
        return best;
    }
    double getdbest() const {
        return dbest;
    }
};

int main() {
    srand(time(nullptr));
    int ncity = rand() % (maxcity - mincity + 1) + mincity;
    vector<citystruct> city(ncity);
    for (int i = 0; i < ncity; i++) {
        city[i].x = static_cast<double>(rand() % 100);
        city[i].y = static_cast<double>(rand() % 100);
    }

    ofstream File("C:\\Users\\user\\Desktop\\m.txt");
    for (const auto& city : city) {
        File << city.x << " " << city.y << endl;
    }
    File.close();

    vector<double> ab(ncity, 0.0);
    for (int i = 0; i < ncity; i++) {
        for (int j = 0; j < ncity; j++) {
            if (i != j) {
                ab[i] += 1.0 / static_cast<double>(rand() % (maxdist - mindist + 1) + mindist);
            }
        }
    }
    double totalPheromone = 0.0;
    double totalVisibility = 0.0;
    for (int i = 0; i < ncity; i++) {
        totalPheromone += ab[i];
        totalVisibility += 1.0 / ab[i];
    }
    double antCountAverage = totalPheromone / static_cast<double>(ncity);
    double econst = 1.0 / (totalVisibility * antCountAverage);
    double bestsize;
    for (int i = 0; i < 10; i++) {
        antalgoritm antalgoritm(city);
        antalgoritm.solve();
        double dbest = antalgoritm.getdbest();
        vector<int> best = antalgoritm.getbest();
        cout << " sim " << i + 1 << " total way " << dbest << endl;
        bestsize = best.size();
    }
    cout << endl;

    cout << " a - number of cities " << bestsize << endl;
    cout << " b - evaporation of pheromone " << econst << endl;
    cout << " c - pheromone visibility " << ab[0] << endl;
    return 0;
}