#include "layer.h"

namespace snnf {

namespace layer {

/**
 * @brief : Initialzie weights randomly and bias to 0.
 * @todo : Support other initialization methods. 
 */
template <typename T>
LinearLayer<T>::LinearLayer(uint32_t in_features, uint32_t out_features)
    : in_features_(in_features),
      out_features_(out_features),
      weights_({in_features, out_features}),
      bias_({1, out_features}),
      input_({1, in_features}),
      grad_weights_({in_features, out_features}),
      grad_bias_({1, out_features})
{
    weights_.random();
    bias_.fill(static_cast<T>(0));
}

/**
 * @brief : Forward propagation
 */
template <typename T>
Tensor<T> LinearLayer<T>::forward(const Tensor<T>& input)
{
    input_ = input; 
    Tensor<T> output = input.matmul(weights_);  // Linear transformation.
    if (output.shape() == bias_.shape()) {
        output = output.add(bias_);
    } else {
        LOG_INFO("Layer::forward : Broadcasting bias to a new shape.");
        Tensor<T> new_bias = bias_.broadcast_to(output.shape());
        output = output.add(new_bias);
    }
    return output;
}

/**
 * @brief : Backward propagation
 */
template <typename T>
Tensor<T> LinearLayer<T>::backward(const Tensor<T>& grad_output)
{
    // Compute gradients w.r.t weights and bias
    grad_weights_ = input_.transpose().matmul(grad_output);
    grad_bias_ = grad_output.sum(0);
    grad_bias_.reshape({1, grad_output.shape()[1]});

    // Compute gradient w.r.t input to pass to previous layer
    Tensor<T> grad_input = grad_output.matmul(weights_.transpose());
    return grad_input;
}

/**
 * @brief : Get weights and bisa
 */
template <typename T>
std::vector<Tensor<T>*> LinearLayer<T>::get_parameters()
{
    return { &weights_, &bias_, &grad_weights_, &grad_bias_ };
}

/**
 * @brief : Clear up grad_weights and grad_bias.
 */
template <typename T>
void LinearLayer<T>::zero_grad()
{
    grad_weights_.fill(static_cast<T>(0));
    grad_bias_.fill(static_cast<T>(0));
}

// Explicit template instatiation
/// @todo : Include more types.

template class LinearLayer<float>;
template class LinearLayer<double>;

} // layer

} // snnf