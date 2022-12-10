#include "../numbered_files.cpp"
#include "../timer.cpp"
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
    for(int i = 0; i < n; ++i) sampler.add(i, mt());
    for(int i = 0; i < n; ++i) sampler.update_weight(i, 358.0);

    unordered_map<int, double> counts;
    const int T = 10000000;
    for(int i = 0 ; i < T; ++i) counts[sampler.get(rand_double(mt))] += 1;
    for(int i = 0; i < n; ++i) assert_equals(counts[i] / T, (double) 1.0 / n, 1e-3);
  }
}

double recur(int i)
{
  if(i == 0) return 1;
  double res = 0;
  for(int j = 0; j < i; ++j) res += recur(j);
  return res;
}

void test_timer()
{
  Timer timer_0;

  double begin = clock();
  auto get_elapsed = [&]() 
  {
    return (clock() - begin) * 1e-6;
  };

  Timer timer_1;

  const double wait_time = 1.0;
  while(timer_0.get_elapsed() < wait_time);

  double elapsed_1 = timer_1.get_elapsed();
  double medium = get_elapsed();
  double elapsed_0 = timer_0.get_elapsed();
  
  assert(elapsed_1 - wait_time * 3e-3 < medium);
  assert(elapsed_0 + wait_time * 3e-3 > medium);
}

void create_dir(string path)
{
  int status_0 = mkdir(path.c_str(), 777);
  usleep(100000);
}

void test_numbered_files_0()
{
  const int n = 20;
  const int m = 10;
  {
    NumberedFiles numbered_files("./numbered_files/main_0", "./numbered_files/tmp_0", 3);

    {
      vector<string> paths = numbered_files.get_latest_file_paths(100);
      assert(paths.empty());
    }

    for(int i = 0; i < n; ++i){
      NumberedFile f = numbered_files.create();
      {
        ofstream ofs(f.get_buffer_path());
        ofs << i << endl;
      }
      f.flush();
    }

    {
      vector<string> paths = numbered_files.get_latest_file_paths(m);

      for(int i = 0; i < m; ++i){
        ifstream ifs(paths[i]);
        int value;
        ifs >> value;
        assert(value == n - 1 - i);
      }
    }

    {
      vector<string> paths = numbered_files.get_latest_file_paths(100);
      assert(paths.size() == n);

      for(int i = 0; i < n; ++i){
        ifstream ifs(paths[i]);
        int value;
        ifs >> value;
        assert(value == n - 1 - i);
      }
    }
  }
  
  {
    NumberedFiles numbered_files("./numbered_files/main_0", "./numbered_files/tmp_0_2", 3);

    for(int i = 0; i < n; ++i){
      NumberedFile f = numbered_files.create();
      {
        ofstream ofs(f.get_buffer_path());
        ofs << i << endl;
      }
      f.flush();
    }

    {
      vector<string> paths = numbered_files.get_latest_file_paths(100);
      assert(paths.size() == n * 2);

      for(int i = 0; i < n * 2; ++i){
        ifstream ifs(paths[i]);
        int value;
        ifs >> value;
        assert(value == (2 * n - 1 - i) % n);
      }
    }
  }
}


int main()
{
  run_test(test_weighted_random_element_sampler_0);
  run_test(test_weighted_random_element_sampler_1);
  run_test(test_timer);
  run_test(test_numbered_files_0);
}

