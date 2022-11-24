#include "vector"
#include <bits/stdc++.h>

template<class T> class WeightedRandomElementSampler
{
public:
  WeightedRandomElementSampler() 
  {
    reversed_order_nodes.push_back(std::vector<double>());
  }

  T& get(double random_weight_from_0_to_1)
  {
    double rem = random_weight_from_0_to_1 * reversed_order_nodes.back()[0];
    int index = 0;
    for(int depth = reversed_order_nodes.size() - 1; 0 < depth; --depth){
      index *= 2;
      if(reversed_order_nodes[depth - 1][index] <= rem){
        rem -= reversed_order_nodes[depth - 1][index];
        ++index;
      }
    }
    return elements[index];
  }

  void add(T t, double weight)
  {
    elements.push_back(t);
    reversed_order_nodes[0].push_back(weight);
    for(int depth = 1; depth < reversed_order_nodes.size(); ++depth){
      int size1 = reversed_order_nodes[depth - 1].size();
      if(reversed_order_nodes[depth].size() * 2 < size1){
        reversed_order_nodes[depth].push_back(reversed_order_nodes[depth - 1].back());
      }else{
        double total = reversed_order_nodes[depth - 1][size1 - 1] + reversed_order_nodes[depth - 1][size1 - 2];
        reversed_order_nodes[depth].back() = total;
      }
    }
    if(2 <= reversed_order_nodes.back().size()){
      assert(reversed_order_nodes.back().size() == 2);
      double total = reversed_order_nodes.back()[0] + reversed_order_nodes.back()[1];
      reversed_order_nodes.push_back(std::vector<double>({total}));
    }
  }

  void update_weight(int index, double weight)
  {
    reversed_order_nodes[0][index] = weight;
    for(int depth = 1; depth < reversed_order_nodes.size(); ++depth){
      index /= 2;
      reversed_order_nodes[depth][index] = reversed_order_nodes[depth - 1][index * 2 + 0];
      if(index * 2 + 1 < reversed_order_nodes[depth - 1].size()){
        reversed_order_nodes[depth][index] += reversed_order_nodes[depth - 1][index * 2 + 1];
      }
    }
  }

  int size()
  {
    return elements.size();
  }

private:

  std::vector<T> elements;
  std::vector<std::vector<double>> reversed_order_nodes;
};

