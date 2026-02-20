#include "VirtualCamera.hpp"
#include "Screen.hpp"
//#include <windows.h>
#include <imgui.h>

VirtualCamera::VirtualCamera(const XMFLOAT3& _upDir,
                             const float     _fovAngleDeg,
                             const float     _nearClip,
                             const float     _farClip) :
                             upDir_      (_upDir),
                             fovAngleDeg_(_fovAngleDeg),
                             nearClip_   (_nearClip),
                             farClip_    (_farClip)
{
}

void VirtualCamera::Init()
{
    matProjection_ = XMMatrixIdentity();
    matView_       = XMMatrixIdentity();
    matBillboard_  = XMMatrixIdentity();

    transform_.position = XMFLOAT3(0, 0, 0);
    targetTransform_.position = XMFLOAT3(0, 1, 0);

    nearClip_ = 0.1f;
    farClip_  = 1000000.0f;
}

void VirtualCamera::Update()
{
    matProjection_ = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleDeg_),
                                              Screen::GetAspectRatio(),
                                              nearClip_,
                                              farClip_);

    // ここで回転行列をかけてやればいい?
    // カメラに回転行列をかけてはいけない！
    XMVECTOR vTargetPos{XMLoadFloat3(&targetTransform_.position)};
    vTargetPos = XMVector3TransformCoord(vTargetPos, targetTransform_.GetCameraMatrix());
    XMStoreFloat3(&targetTransform_.position, vTargetPos);
    matView_       = XMMatrixLookAtLH(XMLoadFloat3(&transform_.position),
                                      XMLoadFloat3(&targetTransform_.position),
                                      XMLoadFloat3(&upDir_));

    //ビルボード行列
    //（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
    http://marupeke296.com/DXG_No11_ComeOnBillboard.html
    matBillboard_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0),
                                     XMLoadFloat3(&targetTransform_.position) - XMLoadFloat3(&transform_.position),
                                     XMLoadFloat3(&upDir_));
    matBillboard_ = XMMatrixInverse(nullptr, matBillboard_);

    ImGui::Begin("CameraProperty");

    ImGui::InputFloat("X: ", &transform_.position.x);
    ImGui::InputFloat("Y: ", &transform_.position.y);
    ImGui::InputFloat("Z: ", &transform_.position.z);
    ImGui::InputFloat("Near Clip:", &nearClip_);
    ImGui::InputFloat("Far Clip:", &farClip_);

    ImGui::End();

}

Transform& VirtualCamera::GetTransform()
{
    return transform_;
}

Transform& VirtualCamera::GetTargetTransform()
{
    return targetTransform_;
}

float& VirtualCamera::GetFovAngleDeg()
{
    return fovAngleDeg_;
}

float& VirtualCamera::GetNearClip()
{
    return nearClip_;
}

float& VirtualCamera::GetFarClip()
{
    return farClip_;
}

const XMMATRIX& VirtualCamera::GetProjectionMat() const
{
    return matProjection_;
}

const XMMATRIX& VirtualCamera::GetViewMat() const
{
    return matView_;
}

const XMMATRIX& VirtualCamera::GetBillboardMat() const
{
    return matBillboard_;
}