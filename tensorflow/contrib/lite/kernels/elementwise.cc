/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <cmath>
#include "tensorflow/contrib/lite/context.h"
#include "tensorflow/contrib/lite/kernels/internal/tensor.h"
#include "tensorflow/contrib/lite/kernels/kernel_util.h"

namespace tflite {
namespace ops {
namespace builtin {
namespace elementwise {

TfLiteStatus SinPrepare(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);
  // Quantized float is not supported yet.
  TF_LITE_ENSURE_EQ(context, input->type, kTfLiteFloat32);
  return context->ResizeTensor(context, output,
                               TfLiteIntArrayCopy(input->dims));
}

TfLiteStatus SinEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      size_t elements = NumElements(input);
      const float* in = GetTensorData<float>(input);
      const float* in_end = in + elements;
      float* out = output->data.f;
      for (; in < in_end; in++, out++) *out = std::sin(*in);
      return kTfLiteOk;
    }
    default: {
      context->ReportError(context, "Only float32 is supported currently");
      return kTfLiteError;
    }
  }
}

}  // namespace elementwise

TfLiteRegistration* Register_SIN() {
  static TfLiteRegistration r = {nullptr, nullptr, elementwise::SinPrepare,
                                 elementwise::SinEval};
  return &r;
}

}  // namespace builtin
}  // namespace ops
}  // namespace tflite
