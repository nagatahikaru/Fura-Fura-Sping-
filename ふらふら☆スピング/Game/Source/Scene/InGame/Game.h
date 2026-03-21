#pragma once
#include "Source/Source.h"
class GameCamera;
class Background;
class InGameUI;
class Batter;
class Pitcher;
class Catcher;
class Ball;

enum CameraMode
{
	Camera_Catcher,
	Camera_Replay,
	Camera_Ball
};

class Game : public Source
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	bool m_isPaused = false;
private:
	GameCamera* m_gameCamera;	//ゲームカメラ。
	Background* m_background;	//背景。
	Batter* m_batter;			//バッター。
	Pitcher* m_pitcher;			//ピッチャー。
	Catcher* m_catcher;			//キャッチャー。
	Ball* m_ball;				//ボール。
	SkyCube* m_skyCube;			//スカイキューブ。
	InGameUI* m_InGameUI;		//インゲームUI。
	CameraMode m_cameraMode = Camera_Catcher;//初期カメラ
	int m_guruguru = 0;
	int m_km = 0;
	

};

