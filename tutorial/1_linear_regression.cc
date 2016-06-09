#include "cnn/cnn.h"
#include "cnn/training.h"
#include "cnn/expr.h"

#include <iostream>
#include <random>

using namespace std;
using namespace cnn;
using namespace cnn::expr;

int main(int argc, char** argv) {
  cnn::Initialize(argc, argv);

  default_random_engine rng;
  normal_distribution<float> normal(0.0f, 1.0f);

  vector<float> xs;
  vector<float> ys;
  for (unsigned i = 0; i < 100; ++i) {
    float x = -1 + 2.0 / 100;
    float y = 2 * x + normal(rng) * 0.33f;
    xs.push_back(x);
    ys.push_back(y);
  }

  Model model;
  Parameter pW = model.add_parameters({1});

  SimpleSGDTrainer trainer(&model, 0.1);

  ComputationGraph cg;
  Expression W = parameter(cg, pW);

  for (unsigned i = 0; i < xs.size(); ++i) {
    Expression pred = W * xs[i];
    Expression loss = square(pred - ys[i]);
    cg.forward();
    cg.backward();
    trainer.update();
  }

  cout << as_scalar(W.value()) << endl; // something around 2
}
