#include "optimization.cuh"

#define BIAS 1.0f

using namespace CUDA;

// l = layer
// n = neuron number in layers
// w = weight number and result in neurons

extern "C"
{

//=====================================================================
//  /!\ WARINING /!\
//=====================================================================

// can still be optimized by decomposing the sum
// can still be optimized by using cuDNN for compute tanh(x)
// can still be optimized by suppressig all for loops
// Can we remove "int n = threadIdx.x;"?
// The best is no loop and no new variable
// Know if const take time or not
// use reference to primitive
// check that debugError produce 0 code in release mode
// do not reset error of output
// option compiler for MSVC
// compute brackpropagation in same time of calcul error ????
// view if use shared memory is more quick that pass pointer or value.
// see pragma unroll

//=====================================================================
//  The compute of output
//=====================================================================

__global__ void calculOutputPerceptronForInputLayer(float* weightsInInputLayer,
                                                    float* outputsInInputLayer,
                                                    const int numberOfInput,
                                                    float* inputs)
{
    int n = threadIdx.x;
    setOutputInInputLayer(outputsInInputLayer, n, 0.0f);
    #pragma unroll
    for(int w = 0; w < numberOfInput; w++)
    {
        addOutputInInputLayer(outputsInInputLayer, n, getWeightInInputLayer(weightsInInputLayer, numberOfInput, n, w) * inputs[w]);
    }
    setOutputInInputLayer(outputsInInputLayer, n, tanh(getOutputInInputLayer(outputsInInputLayer, n) + BIAS));
}

__global__ void calculOutputPerceptronForFistHiddenLayer(float* weightsInHiddenLayers,
                                                         float* outputsInInputLayer,
                                                         float* outputsInHiddenLayers,
                                                         const int numberOfNeuronInHiddenLayers)
{
    int n = threadIdx.x;
    setOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n, 0.0f);
    #pragma unroll
    for(int w = 0; w < numberOfNeuronInHiddenLayers; w++)
    {
        addOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n, getWeightInHiddenLayers(weightsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n, w) *
                                getOutputInInputLayer(outputsInInputLayer, w));
    }
    setOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n, tanh(getOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n) + BIAS));
}

__global__ void calculOutputPerceptronForOtherHiddenLayers(float* weightsInHiddenLayers,
                                                           float* outputsInHiddenLayers,
                                                           const int numberOfNeuronInHiddenLayers,
                                                           const int l)
{
    int n = threadIdx.x;
    setOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n, 0.0f);
    #pragma unroll
    for(int w = 0; w < numberOfNeuronInHiddenLayers; w++)
    {
        addOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n, getWeightInHiddenLayers(weightsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n, w)
                                * getOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, l-1, w));
    }
    setOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n, tanh(getOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n) + BIAS));
}

__global__ void calculOutputPerceptronForOutputLayer(float* weightsInOutputLayer,
                                                     float* outputsInHiddenLayers,
                                                     float* outputsInOutputLayer,
                                                     const int numberOfHiddenLayers,
                                                     const int numberOfNeuronInHiddenLayers)
{
    int n = threadIdx.x;
    setOutputInOutputLayer(outputsInOutputLayer, n, 0.0f);
    #pragma unroll
    for(int w = 0; w < numberOfNeuronInHiddenLayers; w++)
    {
        addOutputInOutputLayer(outputsInOutputLayer, n, getWeightInOutputLayer(weightsInOutputLayer, numberOfNeuronInHiddenLayers, n, w) *
                               getOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, numberOfHiddenLayers-1, w));
         //printf("w : %f\n", getOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, numberOfHiddenLayers-1, w));
    }
    //printf("output %i : %f\n", n, getOutputInOutputLayer(outputsInOutputLayer, n));
    setOutputInOutputLayer(outputsInOutputLayer, n, (tanh(getOutputInOutputLayer(outputsInOutputLayer, n) + BIAS))/2.0f + 0.5f);
}

__forceinline__ __host__ void calculOutputWithoutConvertInputs(float* weightsInInputLayer,
                                                               float* weightsInHiddenLayers,
                                                               float* weightsInOutputLayer,
                                                               float* outputsInInputLayer,
                                                               float* outputsInHiddenLayers,
                                                               float* outputsInOutputLayer,
                                                               const int numberOfHiddenLayers,
                                                               const int numberOfNeuronInHiddenLayers,
                                                               const int numberOfInput,
                                                               const int numberOfOutput,
                                                               float* inputs)
{
    calculOutputPerceptronForInputLayer<<<1, numberOfNeuronInHiddenLayers>>>(weightsInInputLayer,
                                                                             outputsInInputLayer,
                                                                             numberOfInput,
                                                                             inputs);
    debugError();

    calculOutputPerceptronForFistHiddenLayer<<<1, numberOfNeuronInHiddenLayers>>>(weightsInHiddenLayers,
                                                                                  outputsInInputLayer,
                                                                                  outputsInHiddenLayers,
                                                                                  numberOfNeuronInHiddenLayers);
    debugError();

    #pragma unroll
    for(int l = 2; l < numberOfHiddenLayers; l++)
    {
        calculOutputPerceptronForOtherHiddenLayers<<<1, numberOfNeuronInHiddenLayers>>>(weightsInHiddenLayers,
                                                                                        outputsInHiddenLayers,
                                                                                        numberOfNeuronInHiddenLayers,
                                                                                        l);
        debugError();
    }

    calculOutputPerceptronForOutputLayer<<<1, numberOfOutput>>>(weightsInOutputLayer,
                                                                outputsInHiddenLayers,
                                                                outputsInOutputLayer,
                                                                numberOfHiddenLayers,
                                                                numberOfNeuronInHiddenLayers);
    debugError();
}

__host__ void calculOutput(float* weightsInInputLayer,
                           float* weightsInHiddenLayers,
                           float* weightsInOutputLayer,
                           float* outputsInInputLayer,
                           float* outputsInHiddenLayers,
                           float* outputsInOutputLayer,
                           const int numberOfHiddenLayers,
                           const int numberOfNeuronInHiddenLayers,
                           const int numberOfInput,
                           const int numberOfOutput,
                           float* inputsCPU)
{
    float* inputs_device;
    cudaMalloc((void**)&inputs_device, sizeof(float) * numberOfInput);
    cudaMemcpy(inputs_device, inputsCPU, sizeof(float) * numberOfInput, cudaMemcpyHostToDevice);

    calculOutputWithoutConvertInputs(weightsInInputLayer,
                                     weightsInHiddenLayers,
                                     weightsInOutputLayer,
                                     outputsInInputLayer,
                                     outputsInHiddenLayers,
                                     outputsInOutputLayer,
                                     numberOfHiddenLayers,
                                     numberOfNeuronInHiddenLayers,
                                     numberOfInput,
                                     numberOfOutput,
                                     inputs_device);


}



//=====================================================================
//  The compute of backpropagation
//=====================================================================

__global__ void resetErrorPerceptronForInputLayer(float* errorsInInputLayer)
{
    setErrorInInputLayer(errorsInInputLayer, threadIdx.x, 0.0f);
}

__global__ void resetErrorPerceptronForHiddenLayers(float* errorsInHiddenLayers,
                                                    const int numberOfNeuronInHiddenLayers,
                                                    const int l)
{
    setErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, l, threadIdx.x, 0.0f);
}

__global__ void resetErrorPerceptronForOutputLayer(float* errorsInOutputLayer)
{
    setErrorInOutputLayer(errorsInOutputLayer, threadIdx.x, 0.0f);
}

__forceinline__ __host__ void resetError(float* errorsInInputLayer,
                                         float* errorsInHiddenLayers,
                                         float* errorsInOutputLayer,
                                         const int numberOfHiddenLayers,
                                         const int numberOfNeuronInHiddenLayers,
                                         const int numberOfOutput)
{
    resetErrorPerceptronForOutputLayer<<<1, numberOfOutput>>>(errorsInOutputLayer);
    debugError();

    #pragma unroll
    for(int l = 0; l < numberOfHiddenLayers; l++)
    {
        resetErrorPerceptronForHiddenLayers<<<1, numberOfNeuronInHiddenLayers>>>(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, l);
        debugError();
    }

    resetErrorPerceptronForInputLayer<<<1, numberOfNeuronInHiddenLayers>>>(errorsInInputLayer);
    debugError();
}


__global__ void calculErrorPerceptronForOutputLayer(float* outputsInOutputLayer,
                                                    float* errorsInOutputLayer,
                                                    float* desires)
{
    int n = threadIdx.x;
    setErrorInOutputLayer(errorsInOutputLayer, n, (desires[n] - getOutputInOutputLayer(outputsInOutputLayer, n)) * abs(desires[n] - getOutputInOutputLayer(outputsInOutputLayer, n)));
    //printf("error GPU (%i,%i) : %f\n", 10, n, getErrorInOutputLayer(errorsInOutputLayer, n));
}

__global__ void calculErrorPerceptronForLastHiddenLayers(float* weightsInOutputLayer,
                                                         float* errorsInHiddenLayers,
                                                         float* errorsInOutputLayer,
                                                         const int numberOfHiddenLayers,
                                                         const int numberOfOutput,
                                                         const int numberOfNeuronInHiddenLayers)
{
    int n = threadIdx.x;
    #pragma unroll
    for(int w = 0; w < numberOfOutput; w++)
    {
        addErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, numberOfHiddenLayers-1, n,
                               getErrorInOutputLayer(errorsInOutputLayer, w) *
                               getWeightInOutputLayer(weightsInOutputLayer, numberOfNeuronInHiddenLayers, w, n));
    }
    divideErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, numberOfHiddenLayers-1, n, numberOfOutput);
    //printf("error GPU (%i,%i) : %f\n", numberOfHiddenLayers-1, n, getErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, numberOfHiddenLayers-1, n));
}

__global__ void calculErrorPerceptronForOtherHiddenLayers(float* weightsInHiddenLayers,
                                                          float* errorsInHiddenLayers,
                                                          const int numberOfNeuronInHiddenLayers,
                                                          const int l)
{
    int n = threadIdx.x;
    #pragma unroll
    for(int w = 0; w < numberOfNeuronInHiddenLayers; w++)
    {
        addErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n,
                               getErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, l+1, w) *
                               getWeightInHiddenLayers(weightsInHiddenLayers, numberOfNeuronInHiddenLayers, l+1, w, n));

        //if(n == 0 && w == 3)
        //    printf("error GPU (%i,%i) : %f\n", l+1, w, getErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, l+1, w));
    }
    divideErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n, numberOfNeuronInHiddenLayers);
    //printf("error GPU : %f\n", getErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n));
}

__global__ void calculErrorPerceptronForInputLayer(float* weightsInHiddenLayers,
                                                   float* errorsInInputLayer,
                                                   float* errorsInHiddenLayers,
                                                   const int numberOfNeuronInHiddenLayers)
{
    int n = threadIdx.x;
    #pragma unroll
    for(int w = 0; w < numberOfNeuronInHiddenLayers; w++)
    {
         addErrorInInputLayer(errorsInInputLayer, n,
                              getErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, w) *
                              getWeightInHiddenLayers(weightsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, w, n));
    }
    divideErrorInInputLayer(errorsInInputLayer, n, numberOfNeuronInHiddenLayers);
    //printf("error GPU il : %f\n", getErrorInInputLayer(errorsInInputLayer, n));
}

__global__ void trainPerceptronForOutputLayer(float* weightsInOutputLayer,
                                              float* outputsInHiddenLayers,
                                              float* errorsInOutputLayer,
                                              const int numberOfHiddenLayers,
                                              const int numberOfNeuronInHiddenLayers,
                                              float learningRate)
{
    int n = threadIdx.x;
    #pragma unroll
    for(int w = 0;w < numberOfNeuronInHiddenLayers; w++) // weights[i] += learningRate * error * inputs[i];
    {
        addWeightInOutputLayer(weightsInOutputLayer, numberOfNeuronInHiddenLayers, n, w,
                               learningRate *
                               getErrorInOutputLayer(errorsInOutputLayer, n) *
                               getOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, numberOfHiddenLayers-1, w));
    }
}

__global__ void trainPerceptronForOtherHiddenLayer(float* weightsInHiddenLayers,
                                                   float* outputsInHiddenLayers,
                                                   float* errorsInHiddenLayers,
                                                   const int numberOfNeuronInHiddenLayers,
                                                   const int l,
                                                   float learningRate)
{
    int n = threadIdx.x;
    #pragma unroll
    for(int w = 0; w < numberOfNeuronInHiddenLayers; w++) // weights[i] += learningRate * error * inputs[i];
    {
        addWeightInHiddenLayers(weightsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n, w,
                                learningRate *
                                getErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, l, n) *
                                getOutputInHiddenLayers(outputsInHiddenLayers, numberOfNeuronInHiddenLayers, l-1, w));
    }
}

__global__ void trainPerceptronForFirtHiddenLayer(float* weightsInHiddenLayers,
                                                  float* outputsInInputLayer,
                                                  float* errorsInHiddenLayers,
                                                  const int numberOfNeuronInHiddenLayers,
                                                  float learningRate)
{
    int n = threadIdx.x;
    #pragma unroll
    for(int w = 0; w < numberOfNeuronInHiddenLayers; w++) // weights[i] += learningRate * error * inputs[i];
    {
        addWeightInHiddenLayers(weightsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n, w,
                                learningRate *
                                getErrorInHiddenLayers(errorsInHiddenLayers, numberOfNeuronInHiddenLayers, 1, n) *
                                getOutputInInputLayer(outputsInInputLayer, w));
    }
}

__global__ void trainPerceptronForInputLayer(float* weightsInInputLayer,
                                             float* errorsInInputLayer,
                                             const int numberOfInput,
                                             float learningRate,
                                             float* inputs)
{
    int n = threadIdx.x;
    #pragma unroll
    for(int w = 0; w < numberOfInput; w++) // weights[i] += learningRate * error * inputs[i];
    {
        addWeightInInputLayer(weightsInInputLayer, numberOfInput, n, w,
                              learningRate *
                              getErrorInInputLayer(errorsInInputLayer, n) *
                              inputs[w]);
    }
}

void backpropagation(float* weightsInInputLayer,
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
                     float* inputsCPU,
                     float* desiresCPU)
{
    float* inputsDevice;
    cudaMalloc((void**)&inputsDevice, sizeof(float) * numberOfInput);
    cudaMemcpy(inputsDevice, inputsCPU, sizeof(float) * numberOfInput, cudaMemcpyHostToDevice);

    float* desiresDevice;
    cudaMalloc((void**)&desiresDevice, sizeof(float) * numberOfInput);
    cudaMemcpy(desiresDevice, desiresCPU, sizeof(float) * numberOfInput, cudaMemcpyHostToDevice);

    calculOutputWithoutConvertInputs(weightsInInputLayer,
                                     weightsInHiddenLayers,
                                     weightsInOutputLayer,
                                     outputsInInputLayer,
                                     outputsInHiddenLayers,
                                     outputsInOutputLayer,
                                     numberOfHiddenLayers,
                                     numberOfNeuronInHiddenLayers,
                                     numberOfInput,
                                     numberOfOutput,
                                     inputsDevice);

    resetError(errorsInInputLayer,
               errorsInHiddenLayers,
               errorsInOutputLayer,
               numberOfHiddenLayers,
               numberOfNeuronInHiddenLayers,
               numberOfOutput);

    calculErrorPerceptronForOutputLayer<<<1, numberOfOutput>>>(outputsInOutputLayer,
                                                               errorsInOutputLayer,
                                                               desiresDevice);
    debugError();

    calculErrorPerceptronForLastHiddenLayers<<<1, numberOfNeuronInHiddenLayers>>>(weightsInOutputLayer,
                                                                                  errorsInHiddenLayers,
                                                                                  errorsInOutputLayer,
                                                                                  numberOfHiddenLayers,
                                                                                  numberOfOutput,
                                                                                  numberOfNeuronInHiddenLayers);
    debugError();

    #pragma unroll
    for(int l = numberOfHiddenLayers-2; l >= 1; l--) // show - 1
    {
        calculErrorPerceptronForOtherHiddenLayers<<<1, numberOfNeuronInHiddenLayers>>>(weightsInHiddenLayers,
                                                                                       errorsInHiddenLayers,
                                                                                       numberOfNeuronInHiddenLayers,
                                                                                       l);
        debugError();
    }

    calculErrorPerceptronForInputLayer<<<1, numberOfNeuronInHiddenLayers>>>(weightsInHiddenLayers,
                                                                            errorsInInputLayer,
                                                                            errorsInHiddenLayers,
                                                                            numberOfNeuronInHiddenLayers);
    debugError();

    trainPerceptronForOutputLayer<<<1, numberOfOutput>>>(weightsInOutputLayer,
                                                         outputsInHiddenLayers,
                                                         errorsInOutputLayer,
                                                         numberOfHiddenLayers,
                                                         numberOfNeuronInHiddenLayers,
                                                         learningRate);
    debugError();

    #pragma unroll
    for(int l = numberOfHiddenLayers-1; l >= 2; l--)
    {
        trainPerceptronForOtherHiddenLayer<<<1, numberOfNeuronInHiddenLayers>>>(weightsInHiddenLayers,
                                                                                outputsInHiddenLayers,
                                                                                errorsInHiddenLayers,
                                                                                numberOfNeuronInHiddenLayers,
                                                                                l,
                                                                                learningRate);
        debugError();
    }

    trainPerceptronForFirtHiddenLayer<<<1, numberOfNeuronInHiddenLayers>>>(weightsInHiddenLayers,
                                                                           outputsInInputLayer,
                                                                           errorsInHiddenLayers,
                                                                           numberOfNeuronInHiddenLayers,
                                                                           learningRate);
    debugError();

    trainPerceptronForInputLayer<<<1, numberOfNeuronInHiddenLayers>>>(weightsInInputLayer,
                                                                      errorsInInputLayer,
                                                                      numberOfInput,
                                                                      learningRate,
                                                                      inputsDevice);
    debugError();
}

//=====================================================================
//  The compute for all data
//=====================================================================


__forceinline__ float calculateClusteringRate(float* weightsInInputLayer,
                                              float* weightsInHiddenLayers,
                                              float* weightsInOutputLayer,
                                              float* outputsInInputLayer,
                                              float* outputsInHiddenLayers,
                                              float* outputsInOutputLayer,
                                              const int numberOfHiddenLayers,
                                              const int numberOfNeuronInHiddenLayers,
                                              const int numberOfInput,
                                              const int numberOfOutput,
                                              float* inputs,
                                              float* desires,
                                              const int sizeOfTestingSet)
{
    int numberOfResultsClassifiedWell;
    int numberOfResultsMisclassefied;

    for(int i = 0; i < sizeOfTestingSet; i++)
    {
        calculOutputWithoutConvertInputs(weightsInInputLayer,
                                         weightsInHiddenLayers,
                                         weightsInOutputLayer,
                                         outputsInInputLayer,
                                         outputsInHiddenLayers,
                                         outputsInOutputLayer,
                                         numberOfHiddenLayers,
                                         numberOfNeuronInHiddenLayers,
                                         numberOfInput,
                                         numberOfOutput,
                                         &inputs[numberOfInput*i]);

        for(int j = 0; j < numberOfOutput; j++)// EXECUTE INSIDE GLOBAL DESIRES IN IN GPU
        {
            if(desires[j] == 0 && outputsInOutputLayer[j] >= 0.5f
            || desires[j] == 1 && outputsInOutputLayer[j] < 0.5f)
            {
                numberOfResultsMisclassefied ++;
                break;
            }
            else if(j == numberOfOutput-1)
            {
               numberOfResultsClassifiedWell ++;
               break;
            }
        }
    }
    return -2.0;//(float)numberOfResultsClassifiedWell/(numberOfResultsClassifiedWell+numberOfResultsMisclassefied);
}

__forceinline__ void shuffleLearningSet(float* array,
                                        const int sizeOfLearningSet,
                                        int random,
                                        float temp)
{
    for(int i = 0; i < sizeOfLearningSet; i++)
    {
        random = rand()%sizeOfLearningSet;
        temp = array[random];
        array[random] = array[i];
        array[i] = temp;
    }
}

__forceinline__ void saveOnCPU()
{

}

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
                          float* inputsLearningCPU,
                          float* inputsTestingCPU,
                          float* desiresLearningCPU,
                          float* desiresTestingCPU,
                          const int sizeOfLearningSet,
                          const int sizeOfTestingSet,
                          float clusteringRate,
                          const int numberOfTrain,
                          const int frequencyOfClusteringRateVerification)
{
    //float newClusteringRate = 0;
    float* inputsLearningDevice;
    float* desiresLearningDevice;
    float* inputsTestingDevice;
    float* desiresTestingDevice;

    srand(time(NULL));

    cudaMalloc((void**)&inputsLearningDevice, sizeof(float) * numberOfInput * sizeOfLearningSet);
    cudaMemcpy(inputsLearningDevice, inputsLearningCPU, sizeof(float) * numberOfInput * sizeOfLearningSet, cudaMemcpyHostToDevice);

    cudaMalloc((void**)&desiresLearningDevice, sizeof(float) * numberOfInput* sizeOfLearningSet);
    cudaMemcpy(desiresLearningDevice, desiresLearningCPU, sizeof(float) * numberOfInput * sizeOfLearningSet, cudaMemcpyHostToDevice);

    cudaMalloc((void**)&inputsTestingDevice, sizeof(float) * numberOfInput * sizeOfTestingSet);
    cudaMemcpy(inputsTestingDevice, inputsTestingCPU, sizeof(float) * numberOfInput * sizeOfTestingSet, cudaMemcpyHostToDevice);

    cudaMalloc((void**)&desiresTestingDevice, sizeof(float) * numberOfInput * sizeOfTestingSet);
    cudaMemcpy(desiresTestingDevice, desiresTestingCPU, sizeof(float) * numberOfInput * sizeOfTestingSet, cudaMemcpyHostToDevice);

    int i, j, index;

    int randomForShuffle;
    float tempForShuffle;
    float* arrayForShuffle = (float*)malloc(sizeof(float) * sizeOfLearningSet);
    for(int i = 0; i < sizeOfLearningSet; i++)
    {
        arrayForShuffle[i] = i;
    }

    for(i = 0; i < numberOfTrain;)
    {
        for(j = 0; j < frequencyOfClusteringRateVerification; i++, j++)
        {
            if(i%sizeOfLearningSet == 0)
            {
                shuffleLearningSet(arrayForShuffle,
                                   sizeOfLearningSet,
                                   randomForShuffle,
                                   tempForShuffle);
            }
            index = arrayForShuffle[i%sizeOfLearningSet];

            backpropagation(weightsInInputLayer,
                            weightsInHiddenLayers,
                            weightsInOutputLayer,
                            outputsInInputLayer,
                            outputsInHiddenLayers,
                            outputsInOutputLayer,
                            errorsInInputLayer,
                            errorsInHiddenLayers,
                            errorsInOutputLayer,
                            numberOfHiddenLayers,
                            numberOfNeuronInHiddenLayers,
                            numberOfInput,
                            numberOfOutput,
                            learningRate,
                            &inputsLearningDevice[index*numberOfInput],
                            &desiresLearningDevice[index*numberOfOutput]);
        }

        calculateClusteringRate(weightsInInputLayer,
                                weightsInHiddenLayers,
                                weightsInOutputLayer,
                                outputsInInputLayer,
                                outputsInHiddenLayers,
                                outputsInOutputLayer,
                                numberOfHiddenLayers,
                                numberOfNeuronInHiddenLayers,
                                numberOfInput,
                                numberOfOutput,
                                &inputsTestingDevice[0],
                                &desiresTestingDevice[0],
                                sizeOfTestingSet);

        /*if(newClusteringRate > clusteringRate)
        {
            clusteringRate = newClusteringRate;
            saveOnCPU();
            printf("Clustering rate = %f", clusteringRate);
        }*/
    }
}


//=====================================================================
//  Tool functions
//=====================================================================

__host__ void returnNetworkOnCPU(float* weightsInInputLayerCPU,
                                 float* weightsInHiddenLayersCPU,
                                 float* weightsInOutputLayerCPU,
                                 float* weightsInInputLayerGPU,
                                 float* weightsInHiddenLayersGPU,
                                 float* weightsInOutputLayerGPU,
                                 int numberOfHiddenLayers,
                                 int numberOfNeuronsInHiddenLayers,
                                 int numberOfInput,
                                 int numberOfOutput)
{
    cudaMemcpy(weightsInInputLayerCPU, weightsInInputLayerGPU, sizeof(float) * numberOfHiddenLayers * numberOfInput ,cudaMemcpyDeviceToHost);
    cudaMemcpy(weightsInHiddenLayersCPU, weightsInHiddenLayersGPU, sizeof(float) * numberOfNeuronsInHiddenLayers * numberOfNeuronsInHiddenLayers * (numberOfHiddenLayers-1), cudaMemcpyDeviceToHost);
    cudaMemcpy(weightsInOutputLayerCPU, weightsInOutputLayerGPU, sizeof(float) * numberOfOutput * numberOfNeuronsInHiddenLayers, cudaMemcpyDeviceToHost);
}

__host__ void returnOutputOnCPU(float* outputsInOutputLayerCPU,
                                float* outputsInOutputLayerGPU,
                                int numberOfOutput)
{
    cudaMemcpy(outputsInOutputLayerCPU, outputsInOutputLayerGPU, sizeof(float) * numberOfOutput, cudaMemcpyDeviceToHost);
    debugError();
}


}
