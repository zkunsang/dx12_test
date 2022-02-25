#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);

		_cbvBuffer = nullptr;
	}
}



void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{
	// 그냥 원칙상임
	// 상수 버퍼는 256 바이트 배수로 만들어야 한다
	// 0 256 512 768
	// 모든 8개의 비트를 다 킴
	// 하위 8비트에 대한 내용들을 싸그리 날려줌
	// 294이면
	// 0000 0001 0100 0100 을 아래와 같이 만들어 버림
	// 0000 0001 0000 0000

	// 255를 더해 주니 반 올림이 된다.
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
	CreateView();
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

void ConstantBuffer::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = _elementCount;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap));

	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < _elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;
		cbvDesc.SizeInBytes = _elementSize;   // CB size is required to be 256-byte aligned.

		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

void ConstantBuffer::PushData(void* buffer, uint32 size)
{
	assert(_currentIndex < _elementCount);
	assert(_elementSize == ((size + 255) & ~255));

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(_currentIndex);
	GEngine->GetTableDescHeap()->SetCBV(cpuHandle, _reg);

	/*
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	*/

	_currentIndex++;

}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(uint32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}

// CB
// CBV Constant Buffer View (Desc.Heap) - CB와 매칭 (즉시)
// Shader Visible (Desc.Heap) - 레지스터랑 매칭 
// [view0, view1, view2, view3][view0, view1, view2, view3][view0, view1, view2, view3]이런식으로 예약
// b0 , b1 , b2 위 두개 는 CmdQueue로 처리 되서 처리 시간이 있다.
// DescHeap을 두개

// SetDescriptorHeap은 엄청나게 느려서 막 남발하면 안됨
// [view0, view1, view2, view3][view0, view1, view2, view3][view0, view1, view2, view3]
// 이렇게 4개씩 있는것을 10개 만드는 것이 아니라
// 40개를 만들어서 나누어 관리