#pragma once
class DepthStencilBuffer
{
public:
	// depth용도로 4바이트 플롯을 사용할거에요
	// DXGI_FORMAT_D24_UNORM_S8_UINT
	// 24비트를 depth 스텡실을 8비트
	void Init(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

	D3D12_CPU_DESCRIPTOR_HANDLE	GetDSVCpuHandle() { return _dsvHandle; }
	DXGI_FORMAT GetDSVFormat() { return _dsvFormat; }

private:
	ComPtr<ID3D12Resource>				_dsvBuffer;
	ComPtr<ID3D12DescriptorHeap>		_dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_dsvHandle = {};
	DXGI_FORMAT							_dsvFormat = {};
};

