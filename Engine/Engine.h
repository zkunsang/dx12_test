#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "DepthStencilBuffer.h"
#include "Input.h"
#include "Timer.h"

class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

	void ResizeWindow(int32 width, int32 height);

public:
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }	// 커맨드 패턴, 하나하나 보내는게 아니라 뭉쳐 보내기
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }		// 더블 버퍼링
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	shared_ptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return _depthStencilBuffer; }

	shared_ptr<Input> GetInput() { return _input; }
	shared_ptr<Timer> GetTimer() { return _timer; }

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }

public:
	void RenderBegin();
	void RenderEnd();

	void Update();

private:
	void ShowFps();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	

private:
	WindowInfo _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<Device> _device;
	shared_ptr<CommandQueue> _cmdQueue;	// 커맨드 패턴, 하나하나 보내는게 아니라 뭉쳐 보내기
	shared_ptr<SwapChain> _swapChain;		// 더블 버퍼링
	shared_ptr<RootSignature> _rootSignature;
	shared_ptr<TableDescriptorHeap> _tableDescHeap;
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer;

	shared_ptr<Input> _input;
	shared_ptr<Timer> _timer;

	vector<shared_ptr<ConstantBuffer>> _constantBuffers;
};

