#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Shader.h"
#include "Mesh.h"

class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

	void ResizeWindow(int32 width, int32 height);

public:
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }	// Ŀ�ǵ� ����, �ϳ��ϳ� �����°� �ƴ϶� ���� ������
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }		// ���� ���۸�
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }

public:
	void RenderBegin();
	void RenderEnd();
	

private:
	WindowInfo _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<Device> _device;
	shared_ptr<CommandQueue> _cmdQueue;	// Ŀ�ǵ� ����, �ϳ��ϳ� �����°� �ƴ϶� ���� ������
	shared_ptr<SwapChain> _swapChain;		// ���� ���۸�
	shared_ptr<RootSignature> _rootSignature;
};

