#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <execution>

using namespace std;

//функция, возводящая аргумент в квадрат для примера
void make_sqr (int &n) {
    n = n * n;
}

int main()
{
    vector <int> vec_1 = {11, 10, 9, 8, 7, 6, 5};
    vector <int> vec_2 = {11, 10, 9, 8, 7, 6, 5};

    cout << "Compare for_each and parallel_for_each work times" << endl;
    auto now = std::chrono::system_clock::now();
    for_each(vec_1.begin(), vec_1.end(), make_sqr);
	auto end = std::chrono::system_clock::now();
	auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	std::cout << "for_each work time: " << elapsed1.count() << std::endl;

    now = std::chrono::system_clock::now();
    for_each(std::execution::par, vec_2.begin(), vec_2.end(), make_sqr);
	end = std::chrono::system_clock::now();
	auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	std::cout << "parallel_for_each work time: " << elapsed2.count() << std::endl;

    cout << "Compare transform and parallel_transform work times" << endl;
    now = std::chrono::system_clock::now();
    transform(vec_1.begin(), vec_1.end(), vec_1.begin(), make_sqr);
	end = std::chrono::system_clock::now();
	auto elapsed3 = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	std::cout << "transform work time: " << elapsed3.count() << std::endl;

    now = std::chrono::system_clock::now();
    transform(std::execution::par, vec_2.begin(), vec_2.end(), vec_2.begin(),make_sqr);
	end = std::chrono::system_clock::now();
	auto elapsed4 = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	std::cout << "parallel_transform work time: " << elapsed4.count() << std::endl;

    cout << "Compare sort and parallel_sort work times" << endl;
    now = std::chrono::system_clock::now();
    sort(vec_1.begin(), vec_1.end());
	end = std::chrono::system_clock::now();
	auto elapsed5 = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	std::cout << "sort work time: " << elapsed5.count() << std::endl;

    now = std::chrono::system_clock::now();
    sort(std::execution::par, vec_2.begin(), vec_2.end());
	end = std::chrono::system_clock::now();
	auto elapsed6 = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	std::cout << "parallel_sort work time: " << elapsed6.count() << std::endl;

    return 0;
}
