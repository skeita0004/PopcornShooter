#include "Transform.hpp"



Transform::Transform(): pParent_(nullptr)
{
	position_ = XMFLOAT3(0, 0, 0);
	rotate_ = XMFLOAT3(0, 0, 0);
	scale_ = XMFLOAT3(1, 1, 1);
	matTranslate_ = XMMatrixIdentity();
	matRotate_ = XMMatrixIdentity();
	matScale_ = XMMatrixIdentity();
}


Transform::~Transform()
{
}

void Transform::Calculation()
{
	//移動行列
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

	//回転行列
	XMMATRIX rotateX, rotateY, rotateZ;
	rotateX = XMMatrixRotationX(XMConvertToRadians(rotate_.x));
	rotateY = XMMatrixRotationY(XMConvertToRadians(rotate_.y));
	rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
	matRotate_ = rotateZ * rotateX * rotateY;

	//拡大縮小
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
}

const XMMATRIX Transform::GetWorldMatrix()
{
	Calculation();
	if (pParent)
	{
		return  matScale * matRotate * matTranslate * pParent->GetWorldMatrix();
	}

	return  matScale * matRotate * matTranslate;
}

const XMMATRIX Transform::GetNormalMatrix()
{
    Calculation();
    if (pParent != nullptr)
    {
        return matRotate * XMMatrixInverse(nullptr, matScale) * pParent->GetNormalMatrix();
    }

    return matRotate * XMMatrixInverse(nullptr, matScale);
}

const XMMATRIX Transform::GetTranslateMatrix()
{
    Calculation();
    if (pParent)
    {
        return matTranslate * pParent->GetTranslateMatrix();
    }

    return matTranslate;
}

const XMMATRIX Transform::GetRotateMatrix()
{
	Calculation();
    if (pParent)
	{
        return matRotate * pParent->GetRotateMatrix();
    }
    return matRotate;
	}

const XMMATRIX Transform::GetScaleMatrix()
{
    Calculation();
    if (pParent)
    {
        return matScale * pParent->GetScaleMatrix();
    }
    return matScale;
}

