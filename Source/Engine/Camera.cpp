#include "Camera.hpp"
#include "Direct3D.hpp"
#include <imgui.h>

XMFLOAT3 _position;
XMFLOAT3 _target;
float    fovAngle;
XMMATRIX _view;
XMMATRIX _proj;
XMMATRIX _billBoard;

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
    const float VIEW_ANGLE_DEG = 120.f;
	_position = XMFLOAT3(0, 3, -10);	//カメラの位置
	_target = XMFLOAT3( 0, 0, 0);	//カメラの焦点

    fovAngle = VIEW_ANGLE_DEG;
	//プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(VIEW_ANGLE_DEG), (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000000.0f);
}

//更新（ビュー行列作成）
void Camera::Update()
{
    _proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngle), (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000000.0f);

	//ビュー行列
	_view = XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, 0),
		XMVectorSet(_target.x, _target.y, _target.z, 0), XMVectorSet(0, 1, 0, 0));


	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);
}

//焦点を設定
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }

//float Camera::Zoom(float _fovAngle)
//{
//    return _fovAngle = ;
//}

//位置を設定
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }

//焦点を取得
XMFLOAT3 Camera::GetTarget() { return _target; }

//位置を取得
XMFLOAT3 Camera::GetPosition() { return _position; }

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return _view; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }
