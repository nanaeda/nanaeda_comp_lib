#include "../weighted_random_element_sampler.cpp"
#include <bits/stdc++.h>

using namespace std;

#define run_test(test_func) { \
  cout << "  running: " << (#test_func) << endl; \
  test_func(); \
} 

double rand_double(mt19937 &mt)
{
  const int MASK = (1 << 27) - 1;
  return (double) (mt() & MASK) / MASK;
}

void assert_equals(double d0, double d1, double eps)
{
  if(eps < abs(d0 - d1)){
    cout << "d0: " << d0 << endl;
    cout << "d1: " << d1 << endl;
    assert(false);
  }
}

void test_weighted_random_element_sampler_0()
{
  class Element
  {
  public:
    Element(int i) : i(i) {}
  
    int i;
  };

  mt19937 mt(1234);
  WeightedRandomElementSampler<Element> sampler;
  
  unordered_map<int, double> weights;
  weights[100] = 23.0;
  weights[115] = 20.0;
  weights[123] = 5.0;
  weights[200] = 2.0;
  weights[300] = 50.0;

  sampler.add(Element(100), weights[100]);
  sampler.add(Element(200), weights[200]);
  sampler.add(Element(123), weights[123]);
  sampler.add(Element(115), weights[115]);
  sampler.add(Element(300), weights[300]);

  {
    const int N = 10000000;
    unordered_map<int, double> counts;
    unordered_map<int, int> debug;
    for(int i = 0; i < N; ++i) counts[sampler.get(rand_double(mt)).i] += 1.0;
    for(auto p: weights) assert_equals(counts[p.first] / N, p.second / 100.0, 1e-3);
  }

  sampler.update_weight(1, weights[123]);
  sampler.update_weight(2, weights[200]);
  swap(weights[123], weights[200]);

  {
    const int N = 10000000;
    unordered_map<int, double> counts;
    unordered_map<int, int> debug;
    for(int i = 0; i < N; ++i) counts[sampler.get(rand_double(mt)).i] += 1.0;
    for(auto p: weights) assert_equals(counts[p.first] / N, p.second / 100.0, 1e-3);
  }
}

void test_weighted_random_element_sampler_1()
{
  mt19937 mt(1234);

  for(int n: {1, 3, 127, 128, 129}){
    WeightedRandomElementSampler<int> sampler;
    for(int i = 0; i < n; ++i) sampler.add(i, 1);

    unordered_map<int, double> counts;
    const int T = 10000000;
    for(int i = 0 ; i < T; ++i) counts[sampler.get(rand_double(mt))] += 1;
    for(int i = 0; i < n; ++i) assert_equals(counts[i] / T, (double) 1.0 / n, 1e-3);
  }
}


int main()
{
  run_test(test_weighted_random_element_sampler_0);
  run_test(test_weighted_random_element_sampler_1);
}

