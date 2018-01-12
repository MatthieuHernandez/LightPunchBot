#ifndef OPTIMIZATION_CUH
#define OPTIMIZATION_CUH

#include <time.h>
#include <cuda.h>
#include <builtin_types.h>
#include <cuda_runtime_api.h>

#include <thrust/host_vector.h>

#include <thrust/device_ptr.h>
#include <thrust/device_malloc.h>
#include <thrust/device_free.h>
#include <thrust/device_vector.h>

//using namespace thrust;

namespace CUDA
{

//=====================================================================
// Don't forget to modify initialize() and returnNetworkOnCPU()
//=====================================================================

extern "C"
{
    void calculOutput(float* weightsInInputLayerGPU,
                      float* weightsInHiddenLayersGPU,
                      float* weightsInOutputLayerGPU,
                      float* outputsInInputLayerGPU,
                      float* outputsInHiddenLayersGPU,
                      float* outputsInOutputLayerGPU,
                      const int numberOfHiddenLayers,
                      const int numberOfNeuronInHiddenLayers,
                      const int numberOfInput,
                      const int numberOfOutput,
                      float* inputsCPU);

    void backpropagation(float* weightsInInputLayerGPU,
                         float* weightsInHiddenLayersGPU,
                         float* weightsInOutputLayerGPU,
                         float* outputsInInputLayerGPU,
                         float* outputsInHiddenLayersGPU,
                         float* outputsInOutputLayerGPU,
                         float* errorsInInputLayerGPU,
                         float* errorsInHiddenLayersGPU,
                         float* errorsInOutputLayerGPU,
                         const int numberOfHiddenLayers,
                         const int numberOfNeuronInHiddenLayers,
                         const int numberOfInput,
                         const int numberOfOutput,
                         float learningRate,
                         const float* inputsCPU,
                         const float* desiresCPU);

    void TrainWithAllDatabase(float* weightsInInputLayer,
                              float* weightsInHiddenLayers,
                              float* weightsInOutputLayer,
                              float* outputsInInputLayer,
                              float* outputsInHiddenLayers,
                              float* outputsInOutputLayer,
                              float* errorsInInputLayer,
                              float* errorsInHiddenLayers,
                              float* errorsInOutputLayer,
                              const int numberOfHiddenLayers,
                              const int numberOfNeuronInHiddenLayers,
                               const int numberOfInput,
                              const int numberOfOutput,
                              float learningRate,
                              const float* inputsLearningCPU,
                              const float* inputsTestingCPU,
                              const float* desiresLearningCPU,
                              const float* desiresTestingCPU,
                              const int sizeOfLearningSet,
                              const int sizeOfTestingSet,
                              const float clusteringRate,
                              const int numberOfTrain,
                              const int frequencyOfClusteringRateVerification);

    void returnNetworkOnCPU(float* weightsInInputLayerCPU,
                            float* weightsInHiddenLayersCPU,
                            float* weightsInOutputLayerCPU,
                            float* weightsInInputLayerGPU,
                            float* weightsInHiddenLayersGPU,
                            float* weightsInOutputLayerGPU,
                            int numberOfHiddenLayers,
                            int numberOfNeuronsInHiddenLayers,
                            int numberOfInput,
                            int numberOfOutput);

    void returnOutputOnCPU(float* outputsInOutputLayerCPU, float* outputsInOutputLayerGPU, int numberOfOutput);

}

__forceinline__ __host__ void free()
{
    /*cudaFree(nn->weightsInInputLayer);
    cudaFree(nn->weightsInHiddenLayers);
    cudaFree(nn->weightsInOutputLayer);
    cudaFree(nn->outputsInInputLayer);
    cudaFree(nn->outputsInHiddenLayers);
    cudaFree(nn->outputsInOutputLayer);
    cudaFree(nn->errorsInInputLayer);
    cudaFree(nn->errorsInHiddenLayers);
    cudaFree(nn->errorsInOutputLayer);
    cudaFree(nn);*/
}

__forceinline__ __host__ void debugError(bool showOnlyEchec = true)
{
    #ifdef _DEBUG // TO KEEP
    cudaError_t err = cudaGetLastError();
    if(err == cudaSuccess)
    {
        if(showOnlyEchec == false)
            std::cout << "Success" << std::endl;
    }
    else
        std::cout << "Echec : " << cudaGetErrorString(err) << std::endl;
    #endif
}

//=====================================================================
//  The weights GPU
//=====================================================================

__forceinline__ __device__ float getWeightInInputLayer(const float* array, const int numberOfInput, const int n, const int w)
{
    return array[n * numberOfInput + w];
}

__forceinline__ __device__ float getWeightInHiddenLayers(const float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const int w)
{
    return array[(l-1) * numberOfNeuronInHiddenLayers * numberOfNeuronInHiddenLayers +
                                    n * numberOfNeuronInHiddenLayers + w];
}

__forceinline__ __device__ float getWeightInOutputLayer(const float* array, const int numberOfNeuronInHiddenLayers, const int n, const int w)
{
    return array[n * numberOfNeuronInHiddenLayers + w];
}

__forceinline__ __device__ void setWeightInInputLayer(float* array, const int numberOfInput, const int n, const int w, const float v)
{
    array[n * numberOfInput + w] = v;
}

__forceinline__ __device__ void setWeightInHiddenLayers(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const int w, const float v)
{
    array[(l-1) * numberOfNeuronInHiddenLayers * numberOfNeuronInHiddenLayers +
                             n * numberOfNeuronInHiddenLayers + w] = v;
}

__forceinline__ __device__ void setWeightInOutputLayer(float* array, const int numberOfNeuronInHiddenLayers, const int n, const int w, const float v)
{
    array[n * numberOfNeuronInHiddenLayers + w] = v;
}

__forceinline__ __device__ void addWeightInInputLayer(float* array, const int numberOfInput, const int n, const int w, const float v)
{
    array[n * numberOfInput + w] += v;
}

__forceinline__  __device__ void addWeightInHiddenLayers(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const int w, const float v)
{
    array[(l-1) * numberOfNeuronInHiddenLayers * numberOfNeuronInHiddenLayers +
                                        n * numberOfNeuronInHiddenLayers + w] += v;
}

__forceinline__ __device__ void addWeightInOutputLayer(float* array, const int numberOfNeuronInHiddenLayers, const int n, const int w, const float v)
{
    array[n * numberOfNeuronInHiddenLayers + w] += v;
}

//=====================================================================
//  The Outputs GPU
//=====================================================================

__forceinline__ __device__ float getOutputInInputLayer(const float* array, const int n)
{
    return array[n];
}

__forceinline__ __device__ float getOutputInHiddenLayers(const float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n)
{
    return array[(l-1) * numberOfNeuronInHiddenLayers + n];
}

__forceinline__ __device__ float getOutputInOutputLayer(const float* array, const int n)
{
    return array[n];
}

__forceinline__ __device__ void setOutputInInputLayer(float* array, const int n, const float v)
{
    array[n] = v;
}

__forceinline__ __device__ void setOutputInHiddenLayers(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const float v)
{
    array[(l-1) * numberOfNeuronInHiddenLayers + n] = v;
}

__forceinline__ __device__ void setOutputInOutputLayer(float* array, const int n, const float v)
{
    array[n] = v;
}

__forceinline__ __device__ void addOutputInInputLayer(float* array, const int n, const float v)
{
    array[n] += v;
}

__forceinline__ __device__ void addOutputInHiddenLayers(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const float v)
{
    array[(l-1) * numberOfNeuronInHiddenLayers + n] += v;
}

__forceinline__ __device__ void addOutputInOutputLayer(float* array, const int n, const float v)
{
    array[n] += v;
}

//=====================================================================
//  The Errors GPU
//=====================================================================

__forceinline__ __device__ float getErrorInInputLayer(const float* array, const int n)
{
    return array[n];
}

__forceinline__ __device__ float getErrorInHiddenLayers(const float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n)
{
    return array[(l-1) * numberOfNeuronInHiddenLayers + n];
}

__forceinline__ __device__ float getErrorInOutputLayer(const float* array, const int n)
{
    return array[n];
}

__forceinline__ __device__ void setErrorInInputLayer(float* array, const int n, const float v)
{
    array[n] = v;
}

__forceinline__ __device__ void setErrorInHiddenLayers(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const float v)
{
    array[(l-1) * numberOfNeuronInHiddenLayers + n] = v;
}

__forceinline__ __device__ void setErrorInOutputLayer(float* array, const int n, const float v)
{
    array[n] = v;
}

__forceinline__ __device__ void addErrorInInputLayer(float* array, const int n, const float v)
{
    array[n] += v;
}

__forceinline__ __device__ void addErrorInHiddenLayers(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const float v)
{
    array[(l-1) * numberOfNeuronInHiddenLayers + n] += v;
}

__forceinline__ __device__ void addErrorInOutputLayer(float* array, const int n, const float v)
{
    array[n] += v;
}

__forceinline__ __device__ void divideErrorInInputLayer(float* array, const int n, const float v)
{
    array[n] /= v;
}

__forceinline__ __device__ void divideErrorInHiddenLayers(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const float v)
{
    array[(l-1) * numberOfNeuronInHiddenLayers + n] /= v;
}

__forceinline__ __device__ void divideErrorInOutputLayer(float* array, const int n, const float v)
{
    array[n] /= v;
}

//=====================================================================
//  The CPU access function
//=====================================================================

__forceinline__ __host__ void setWeightInInputLayerFromCPU(float* array, const int numberOfInput, const int n, const int w, const float v)
{
     cudaMemcpy(&array[n * numberOfInput + w], &v , sizeof(float), cudaMemcpyHostToDevice);
     debugError(true);
}

__forceinline__ __host__ void setWeightInHiddenLayersFromCPU(float* array, const int numberOfNeuronInHiddenLayers, const int l, const int n, const int w, const float v)
{
    cudaMemcpy(&array[(l-1) * numberOfNeuronInHiddenLayers * numberOfNeuronInHiddenLayers +
               n * numberOfNeuronInHiddenLayers + w], &v, sizeof(float), cudaMemcpyHostToDevice);
    debugError(true);
}

__forceinline__ __host__ void setWeightInOutputLayerFromCPU(float* array, const int numberOfNeuronInHiddenLayers, const int n, const int w, const float v)
{
    cudaMemcpy(&array[n * numberOfNeuronInHiddenLayers + w], &v, sizeof(float), cudaMemcpyHostToDevice);
    debugError(true);
}

__forceinline__ __host__ void setIntegerFromCPU(int* pointer, const int v)
{
    cudaMemcpy(pointer, &v , sizeof(float), cudaMemcpyHostToDevice);
    debugError(true);
}

__forceinline__ __host__ void setFloatFromCPU(float* pointer, const float v)
{
    cudaMemcpy(pointer, &v , sizeof(float), cudaMemcpyHostToDevice);
    debugError(true);
}

}
#endif // OPTIMIZATION_CUH
