#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include <ctime>
#include <thread>
#include <mutex>

using namespace std;
std::mutex g_mutex;

double pie_seq()
{
    static double k = 0;
    const int N = 100000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0,1);

    for(auto i = 0; i < N; ++i) {
        double x = dist(gen);
        double y = dist(gen);
        if( (x*x + y*y) <= 1.0) {
            ++k;
        }
    }
    return 4 * k / N;
}

void pie(double xm, const int N, double &K)
{
    double k = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0,1);
    std::uniform_real_distribution<> dist_2(xm, xm + 0.25);
    for(auto i = 0; i < N; ++i) {
        double x = dist_2(gen);
        double y = dist(gen);
        if( x*x + y*y <= 1) {
            ++k;
        }
    }

    g_mutex.lock();
    K += k;
    g_mutex.unlock();
}

double pie_par()
{
    const int N = 100000;
    int n = N / 4;
    double k = 0;

    thread thr1(pie, 0.0, n, std::ref(k));
	thread thr2(pie, 0.25, n, std::ref(k));
    thread thr3(pie, 0.5, n, std::ref(k));
    thread thr4(pie, 0.75, n, std::ref(k));

    thr1.join();
	thr2.join();
    thr3.join();
	thr4.join();

    return 4 * k / N;
}


int main()
{
    cout << "sequential: p = " << pie_seq() << endl;
    cout << "parallel: p = " << pie_par() << endl;

    return EXIT_SUCCESS;
}
