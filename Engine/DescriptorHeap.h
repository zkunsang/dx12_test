#pragma once
// [��ȼ�], ������
// ���ָ� �ñ� �� �̷����� �������� ���� �Ѱ���� �ϴµ�,
// �ƹ� ���·γ� ��û�ϸ� �� �˾ƸԴ´�.
// - ���� ���ҽ��� � �뵵�� ����ϴ��� �Ĳ��ϰ� ��� �Ѱ��ִ� �뵵.
// GPU���� �˾� �԰Բ�
class DescriptorHeap //View
{
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int32 idx) { return _rtvHandle[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE		GetBackBufferView();
private:
	// DH [[View][View]] <-> [Resource]
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	uint32							_rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	shared_ptr<class SwapChain>		_swapChain;
};

