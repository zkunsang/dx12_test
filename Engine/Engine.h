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
	shared_ptr<class CommandQueue> _cmdQueue;	// Ŀ�ǵ� ����, �ϳ��ϳ� �����°� �ƴ϶� ���� ������
	shared_ptr<class SwapChain> _swapChain;		// ���� ���۸�
	shared_ptr<class DescriptorHeap> _descHeap; // ��� �׸� ������

};

