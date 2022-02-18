#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();

private:
	// GPU쪽데이터 
	// 모든 파란색영역
	ComPtr<ID3D12Resource>	_cbvBuffer;

	// CPU쪽에서 가져오는 버퍼
	// Mesh에서 Map,과 UnMap사이
	BYTE* _mappedBuffer = nullptr;
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	uint32					_currentIndex = 0;
};
