#ifndef _DRAW_STATE_H_
#define _DRAW_STATE_H_

//=================================================================
//Data to be passed by game to all Game Objects via Draw 
//=================================================================

#include "CommonStates.h"

#include "Cam.h"

using namespace DirectX;

//class Camera; now using cool ecs camera B)
class Light;

struct DrawData
{
	ID3D11DeviceContext* m_pd3dImmediateContext;
	CommonStates* m_states;
	Engine::Camera* m_cam;
	Light* m_light;
	
	//Camera* m_cam;
};

#endif
