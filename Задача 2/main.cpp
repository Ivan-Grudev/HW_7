#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>
#include <string>
#include <fstream>

template < typename Iterator, typename T >
struct accumulate_block
{
	void operator()(Iterator first, Iterator last, T & result)
	{
		result = std::accumulate(first, last, result); // !
	}
};

template < typename Iterator, typename T >
T parallel_accumulate(Iterator first, Iterator last, T init, int n)
{
	const std::size_t length = std::distance(first, last);

	if (!length)
		return init;

	const std::size_t num_threads = n;

	const std::size_t block_size = length / num_threads;

	std::vector < T > results(num_threads);
	std::vector < std::thread > threads(num_threads - 1);

	Iterator block_start = first;

	for (std::size_t i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		threads[i] = std::thread(
			accumulate_block < Iterator, T > (),
			block_start, block_end, std::ref(results[i])); // !

		block_start = block_end;
	}

	accumulate_block < Iterator, T > ()(block_start, last, results[num_threads - 1]);

	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));

	return std::accumulate(results.begin(), results.end(), init);
}

int main(int argc, char ** argv)
{
	std::vector < int > v(100);

	std::iota(v.begin(), v.end(), 1);

	int n = 600; //можно взять другое число максимального количества для потоков; 600 мне показалось наглядным

	std::string outFile_1 = "out.dat";
    std::ofstream out_1(outFile_1);

    if (!out_1.is_open()) {
        std::cerr << "Can not open: " << outFile_1 << std::endl;
        return EXIT_FAILURE;
    }

	for (int i = 1; i <= n; i++) {
        std::cout << "result: "; //для проверки одинаковости результата
        auto now = std::chrono::system_clock::now();
        std::cout << parallel_accumulate(v.begin(), v.end(), 0, i) << std::endl;
        auto end = std::chrono::system_clock::now();
        auto elapsed1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - now);
        out_1 << i << "\t" << elapsed1.count() << "\n";
	}

	system("pause");

	return EXIT_SUCCESS;
}
