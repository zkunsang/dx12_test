#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	// Default와 데이터를 전달하는 용도
	// 움직이지 않는 돌이나 나무는 Default
	// 
	// Upload를 만들어서 복사 로 
	// 사용해야 하나
	// 공용으로 활용
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);
	// 디바이스를 통해서 작업하는것은 바로 일어남(즉시)

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	// CMD_LIST를 통해 요청해서(나중에)
	// TODO
	// 1) buffer에다가 데이터 세팅
	// 
	// 2) buffer의 주소를 register에다가 전송
	// CMDQueue에 있는것은 바로 일어나는게 아니라
	// Ram에 Buffer하나로 하게 되면
	// 즉시 즉시 되는 것과 나중에 되는것의 차이가 있기 때문에
	// GPU Ram에 버퍼를 여러개를 만든다
	// 삼각형 2개 만들면 버퍼 2개

	// 위에것은 위치 이동 값이고
	// 아래것은 색상 이동 값이다.
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0);
	}
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b1);
	}

	GEngine->GetTableDescHeap()->CommitTable();

	// GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
	// GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform));
	/*CMD_LIST->SetGraphicsRootConstantBufferView(0, &_transform, sizeof(_transform));*/
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);

	// Root Signature는 64 DWORD여서 256바이트가 한계이다.
	// 이걸 넘어서기 위해서 Description table을 여러개 사용 해서 스위칭 해서 새용해준다.
}