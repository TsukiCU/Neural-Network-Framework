#pragma once

#include "utils.h"
#include "tensor.h"

namespace snnf {

template <typename T>
class Optimizer {
public:
    Optimizer() = default;
    virtual ~Optimizer() = default;

    ///@brief :  Update parameters
    virtual void step() = 0;

    ///@brief : Zero gradients of all parameters
    virtual void zero_grad();

    ///@brief : Add parameters to the optimizer
    void add_parameters(const std::vector<Tensor<T>*>& params);

protected:
    std::vector<Tensor<T>*> parameters_;
};

template <typename T>
class SGD : public Optimizer<T> {
public:
    SGD(T learning_rate);
    ~SGD() = default;

    void step() override;

private:
    T learning_rate_;
};

} // snnf