#pragma once
class DepthStencilBuffer
{
public:
	// depth�뵵�� 4����Ʈ �÷��� ����Ұſ���
	// DXGI_FORMAT_D24_UNORM_S8_UINT
	// 24��Ʈ�� depth ���޽��� 8��Ʈ
	void Init(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

	D3D12_CPU_DESCRIPTOR_HANDLE	GetDSVCpuHandle() { return _dsvHandle; }
	DXGI_FORMAT GetDSVFormat() { return _dsvFormat; }

private:
	ComPtr<ID3D12Resource>				_dsvBuffer;
	ComPtr<ID3D12DescriptorHeap>		_dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_dsvHandle = {};
	DXGI_FORMAT							_dsvFormat = {};
};

