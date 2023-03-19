// Vectorization.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <execution>

void addVectorByElem()
{
	int sizeCont = 1UL << 25;
	std::cout << "sizeCont = " << sizeCont << " (1UL << 25)" << std::endl;

	std::vector<float> cont1;
	cont1.reserve(sizeCont);

	std::vector<float> cont2;
	cont2.reserve(sizeCont);

	for (int i = 0; i < sizeCont; ++i) {
		cont1.emplace_back(i);
		cont2.emplace_back(i);
	}

	std::vector<float> res;
	res.resize(sizeCont);

	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < sizeCont; ++i) {
			res[i] = cont1[i] + cont2[i];
		}
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (cycle)" << std::endl;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		std::transform(std::begin(cont1), std::end(cont1), std::begin(cont2), std::begin(res),
			[](int a, int b) { return a + b; });
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (transform)" << std::endl;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		std::transform(std::execution::par,
			std::begin(cont1), std::end(cont1), std::begin(cont2), std::begin(res),
			[](int a, int b) { return a + b; });
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (transform execution::par)" << std::endl;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		std::transform(std::execution::unseq,
			std::begin(cont1), std::end(cont1), std::begin(cont2), std::begin(res),
			[](int a, int b) { return a + b; });
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (transform execution::unseq)" << std::endl;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		std::transform(std::execution::par_unseq,
			std::begin(cont1), std::end(cont1), std::begin(cont2), std::begin(res),
			[](int a, int b) { return a + b; });
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (transform execution::par_unseq)" << std::endl;
	}

	{
		alignas(16) float* fm1 = &cont1[0];
		alignas(16) float* fm2 = &cont2[0];
		alignas(16) float* pRes = &res[0];

		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < sizeCont; i += 4) {
			__m128 a = _mm_load_ps(&fm1[i]);
			__m128 b = _mm_load_ps(&fm2[i]);
			__m128 r = _mm_add_ps(a, b);
			_mm_store_ps(&pRes[i], r);
		}
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (SSE)" << std::endl;
	}

	{
		alignas(16) float* fm1 = &cont1[0];
		alignas(16) float* fm2 = &cont2[0];
		alignas(16) float* pRes = &res[0];

		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < sizeCont; i += 8) {
			__m256 a = _mm256_load_ps(&fm1[i]);
			__m256 b = _mm256_load_ps(&fm2[i]);
			__m256 r = _mm256_add_ps(a, b);
			_mm256_store_ps(&pRes[i], r);
		}
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (SSE)" << std::endl;
	}

	{
		alignas(16) float* fm1 = &cont1[0];
		alignas(16) float* fm2 = &cont2[0];
		alignas(16) float* pRes = &res[0];

		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < sizeCont; i += 16) {
			__m512 a = _mm512_load_ps(&fm1[i]);
			__m512 b = _mm512_load_ps(&fm2[i]);
			__m512 r = _mm512_add_ps(a, b);
			_mm512_store_ps(&pRes[i], r);
		}
		auto diff = std::chrono::high_resolution_clock::now() - start;
		std::cout << "execute time duration = " << std::chrono::duration<double, std::milli>(diff).count() << " milliseconds (SSE)" << std::endl;
	}

}

int main()
{
	addVectorByElem();
}