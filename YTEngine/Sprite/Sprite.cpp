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

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f,0.0f,(float)directXCommon_->GetWin)
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	*transformationMatrixData = 
}