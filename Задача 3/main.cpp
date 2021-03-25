#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <future>

//функция, возводящая аргумент в квадрат
void make_sqr (int &n) {
    n = n * n;
}

template < typename Iterator, class Compare >
int parallel_each(Iterator first, Iterator last, Compare fun)
{
    const std::size_t length = std::distance(first, last);
    const std::size_t max_size = 25;

    if (length <= max_size)
	{
		std::for_each(first, last, make_sqr);
	}
    else
	{
		Iterator middle = first;
		std::advance(middle, length / 2);

		std::future < int > first_half_result =
			std::async(parallel_each < Iterator, Compare >, first, middle, fun);

		int second_half_result = parallel_each(middle, last, fun);

		int a = first_half_result.get() + second_half_result;
	}

    return 0;

}

int main(int argc, char ** argv)
{
	std::vector < int > v(100);
	std::iota(v.begin(), v.end(), 1);

    std::cout << "vector:" << std::endl;
    for(size_t i = 0; i < v.size(); i += 5) {
        std::cout << "v[" << i << "]: " << v[i] << std::endl;
    }

    parallel_each(v.begin(), v.end(), make_sqr);
    std::cout << "\nafter parallel_each:" << std::endl;
    for(size_t i = 0; i < v.size(); i += 5)
    {
        std::cout << "v[" << i << "] = " << v[i]   << std::endl;

    }

	system("pause");

	return EXIT_SUCCESS;
}
