//=============================================================================
//
// シャドウマップ処理 [shadow_renderer.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSM(void);
void UninitSM(void);

void SetSMRenderer(void);

ID3D11ShaderResourceView** SMResource(void);
ID3D11SamplerState** SMSampler(void);
