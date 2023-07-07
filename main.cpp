#include <iostream>
#include <thread>
#include <random>
#include <chrono>

#define ARR_SIZE 100000
#define N_PARTS 10

int64_t static summa = 0;

void arr_sum(int arr[], int begin, int end)
{
	int s = summa;
	for (int i = begin; i < end; i++)
	{
		summa += arr[i];
	}

}

 int getRandomNumber(int min, int max)
 {
	 static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

	 return static_cast<int>(rand() * fraction * (max - min + 1) + min);
 }

int main()
{
	int array[ARR_SIZE]{};
	std::thread thread_arr[N_PARTS]{};

	for (int i = 0; i < ARR_SIZE; i++)
	{
		array[i] = getRandomNumber(1, 100);
	}

	std::cout << "multistream  case:" << std::endl;

	auto begin = std::chrono::steady_clock::now();

	int j = 0;
	for (int i = 0; i < ARR_SIZE; i += ARR_SIZE / N_PARTS)
	{
		int begin = i;
		int end = i + ARR_SIZE / N_PARTS;
		thread_arr[j] = std::thread (arr_sum, std::ref(array), std::ref(begin), std::ref(end));

		if (thread_arr[j].joinable())
			thread_arr[j].join();

		j++;
	}

	std::cout << "sum of all array elements = " << summa << std::endl;

	auto end = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
	std::cout << "elapsed time = " << elapsed_time.count() << " ms" << std::endl << std::endl;

	std::cout << "scalar case:" << std::endl;
	summa = 0;

	begin = std::chrono::steady_clock::now();

	for (int i = 0; i < ARR_SIZE; i++)
	{
		summa += array[i];
	}

	std::cout << "sum of all array elements = " << summa << std::endl;

	end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
	std::cout << "elapsed time = " << elapsed_time.count() << " ms" << std::endl;

	return 0;
}