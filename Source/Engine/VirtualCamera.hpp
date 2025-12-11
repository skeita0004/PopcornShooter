#pragma once
#include "Transform.hpp"

class VirtualCamera
{
public:
    VirtualCamera(const XMFLOAT3&  _upDir,
                  const float      _fovAngleDeg,
                  const float      _nearClip,
                  const float      _farClip);

    void Init();
    void Update();

    Transform& GetTransform();
    Transform& GetTargetTransform();
    float& GetFovAngleDeg();
    float& GetNearClip();
    float& GetFarClip();
    const XMMATRIX& GetProjectionMat() const;
    const XMMATRIX& GetViewMat() const;
    const XMMATRIX& GetBillboardMat() const;

private:
    Transform transform_;
    Transform targetTransform_;
    XMFLOAT3  upDir_;
    float     fovAngleDeg_;
    float     nearClip_;
    float     farClip_;
    XMMATRIX  matProjection_;
    XMMATRIX  matView_;
    XMMATRIX  matBillboard_;

    inline static const float DEFAULT_NEAR_CLIP{ 0.1f };
    inline static const float DEFAULT_FAR_CLIP{1000000.f};
};