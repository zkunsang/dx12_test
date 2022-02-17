#pragma once

// 인력 사무소
class Device
{
public:
	void Init();

	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }
private:

	// Com(Component Object Model)
	// 세부사항은 우리한테 숨겨짐
	// 일종의 스마트 포인터
	ComPtr<ID3D12Debug> _debugController;
	ComPtr<IDXGIFactory> _dxgi;
	ComPtr<ID3D12Device> _device;
};

