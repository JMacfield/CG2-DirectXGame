#include "Sprite.h"

void Sprite::Initialize(DirectXCommon* directXCommon) {
	directXCommon = directXCommon_;
	CreateVertexData();
	CreateTransform();
}

void Sprite::Draw(Vector4 a, Vector4 b, Transform transform) {
	vertexData_[0].position = { a.x,b.y,0.0f,1.0f };
	vertexData_[1].position = { a.x,a.y,0.0f,1.0f };
	vertexData_[2].position = { b.x,b.y,0.0f,1.0f };
	vertexData_[3].position = { a.x,a.y,0.0f,1.0f };
	vertexData_[4].position = { b.x,a.y,0.0f,1.0f };
	vertexData_[5].position = { b.x,b.y,0.0f,1.0f };
}