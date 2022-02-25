#pragma once

enum class CONSTANT_BUFFER_TYPE : uint8 {
	STRANSFORM,
	MATERIAL,
	END
};
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);

	void Clear();
	void PushData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);
private:
	void CreateBuffer();
	void CreateView();

private:
	// GPU쪽데이터 
	// 모든 파란색영역
	ComPtr<ID3D12Resource>	_cbvBuffer;

	// CPU쪽에서 가져오는 버퍼
	// Mesh에서 Map,과 UnMap사이
	BYTE* _mappedBuffer = nullptr;
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {}; // 시작핸들
	uint32								_handleIncrementSize = 0; // 몇칸 넘어갔는지
	

	uint32					_currentIndex = 0;

	CBV_REGISTER _reg = {};

	
};
