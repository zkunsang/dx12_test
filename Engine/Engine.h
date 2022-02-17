#pragma once
class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

	void ResizeWindow(int32 width, int32 height);

public:
	void RenderBegin();
	void RenderEnd();
	

private:
	WindowInfo _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<class Device> _device;
	shared_ptr<class CommandQueue> _cmdQueue;	// 커맨드 패턴, 하나하나 보내는게 아니라 뭉쳐 보내기
	shared_ptr<class SwapChain> _swapChain;		// 더블 버퍼링
	shared_ptr<class DescriptorHeap> _descHeap; // 어떻게 그릴 것인지

};

