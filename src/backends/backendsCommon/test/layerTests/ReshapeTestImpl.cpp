//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "ReshapeTestImpl.hpp"

#include <backendsCommon/test/DataTypeUtils.hpp>
#include <backendsCommon/test/TensorCopyUtils.hpp>
#include <backendsCommon/test/WorkloadTestUtils.hpp>

#include <test/TensorHelpers.hpp>

namespace
{

template<typename T, size_t NumDims>
LayerTestResult<T, NumDims> SimpleReshapeTestImpl(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager,
    armnn::TensorInfo inputTensorInfo,
    armnn::TensorInfo outputTensorInfo,
    const std::vector<T>& inputData,
    const std::vector<T>& outputExpectedData)
{
    boost::ignore_unused(memoryManager);
    auto input = MakeTensor<T, NumDims>(inputTensorInfo, inputData);

    LayerTestResult<T, NumDims> ret(outputTensorInfo);
    ret.outputExpected = MakeTensor<T, NumDims>(outputTensorInfo, outputExpectedData);

    std::unique_ptr<armnn::ITensorHandle> inputHandle = workloadFactory.CreateTensorHandle(inputTensorInfo);
    std::unique_ptr<armnn::ITensorHandle> outputHandle = workloadFactory.CreateTensorHandle(outputTensorInfo);

    armnn::ReshapeQueueDescriptor data;
    armnn::WorkloadInfo info;
    AddInputToWorkload(data, info, inputTensorInfo, inputHandle.get());
    AddOutputToWorkload(data, info, outputTensorInfo, outputHandle.get());

    std::unique_ptr<armnn::IWorkload> workload = workloadFactory.CreateReshape(data, info);

    inputHandle->Allocate();
    outputHandle->Allocate();

    CopyDataToITensorHandle(inputHandle.get(), input.origin());

    workload->Execute();

    CopyDataFromITensorHandle(ret.output.origin(), outputHandle.get());

    return ret;
}

} // anonymous namespace

template<armnn::DataType ArmnnType, typename T>
LayerTestResult<T, 4> SimpleReshapeTest(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager)
{
    armnn::TensorInfo inputTensorInfo;
    armnn::TensorInfo outputTensorInfo;

    unsigned int inputShape[] = { 2, 2, 3, 3 };
    unsigned int outputShape[] = { 2, 2, 9, 1 };

    inputTensorInfo = armnn::TensorInfo(4, inputShape, ArmnnType);
    inputTensorInfo.SetQuantizationScale(1.0f);
    outputTensorInfo = armnn::TensorInfo(4, outputShape, ArmnnType);
    outputTensorInfo.SetQuantizationScale(1.0f);

    auto input = ConvertToDataType<ArmnnType>(
        {
            0.0f, 1.0f, 2.0f,
            3.0f, 4.0f, 5.0f,
            6.0f, 7.0f, 8.0f,

            9.0f, 10.0f, 11.0f,
            12.0f, 13.0f, 14.0f,
            15.0f, 16.0f, 17.0f,

            18.0f, 19.0f, 20.0f,
            21.0f, 22.0f, 23.0f,
            24.0f, 25.0f, 26.0f,

            27.0f, 28.0f, 29.0f,
            30.0f, 31.0f, 32.0f,
            33.0f, 34.0f, 35.0f,
        },
        inputTensorInfo);

    auto outputExpected = ConvertToDataType<ArmnnType>(
        {
            0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,

            9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f,

            18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f,

            27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f,
        },
        outputTensorInfo);

    return SimpleReshapeTestImpl<T, 4>(
        workloadFactory, memoryManager, inputTensorInfo, outputTensorInfo, input, outputExpected);
}

template<armnn::DataType ArmnnType, typename T>
LayerTestResult<T, 5> Reshape5dTest(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager)
{
    armnn::TensorInfo inputTensorInfo;
    armnn::TensorInfo outputTensorInfo;

    unsigned int inputShape[] = { 2, 2, 8, 1, 1 };
    unsigned int outputShape[] = { 2, 2, 2, 2, 2 };

    inputTensorInfo = armnn::TensorInfo(5, inputShape, ArmnnType);
    inputTensorInfo.SetQuantizationScale(1.0f);
    outputTensorInfo = armnn::TensorInfo(5, outputShape, ArmnnType);
    outputTensorInfo.SetQuantizationScale(1.0f);

    auto input = ConvertToDataType<ArmnnType>(
        {
            0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f,
            8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f,

            16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f,
            24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f,
        },
        inputTensorInfo);

    auto outputExpected = ConvertToDataType<ArmnnType>(
        {
            0.0f, 1.0f,
            2.0f, 3.0f,

            4.0f, 5.0f,
            6.0f, 7.0f,


            8.0f, 9.0f,
            10.0f, 11.0f,

            12.0f, 13.0f,
            14.0f, 15.0f,



            16.0f, 17.0f,
            18.0f, 19.0f,

            20.0f, 21.0f,
            22.0f, 23.0f,


            24.0f, 25.0f,
            26.0f, 27.0f,

            28.0f, 29.0f,
            30.0f, 31.0f,
        },
        outputTensorInfo);

    return SimpleReshapeTestImpl<T, 5>(
        workloadFactory, memoryManager, inputTensorInfo, outputTensorInfo, input, outputExpected);
}

//
// Explicit template specializations
//

template LayerTestResult<armnn::ResolveType<armnn::DataType::Float32>, 4>
SimpleReshapeTest<armnn::DataType::Float32>(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager);

template LayerTestResult<armnn::ResolveType<armnn::DataType::QAsymmU8>, 4>
SimpleReshapeTest<armnn::DataType::QAsymmU8>(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager);

template LayerTestResult<armnn::ResolveType<armnn::DataType::QSymmS16>, 4>
SimpleReshapeTest<armnn::DataType::QSymmS16>(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager);

template LayerTestResult<armnn::ResolveType<armnn::DataType::Float32>, 5>
Reshape5dTest<armnn::DataType::Float32>(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager);

template LayerTestResult<armnn::ResolveType<armnn::DataType::QAsymmU8>, 5>
Reshape5dTest<armnn::DataType::QAsymmU8>(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager);

template LayerTestResult<armnn::ResolveType<armnn::DataType::QSymmS16>, 5>
Reshape5dTest<armnn::DataType::QSymmS16>(
    armnn::IWorkloadFactory& workloadFactory,
    const armnn::IBackendInternal::IMemoryManagerSharedPtr& memoryManager);
