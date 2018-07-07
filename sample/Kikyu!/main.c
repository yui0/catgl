//---------------------------------------------------------
//	Kikyu!
//
//		©2013,2018 Yuichiro Nakada
//---------------------------------------------------------

#include <time.h>

#define CATGL_NANOVG
#define CATGL_IMPLEMENTATION
#define SCREEN_TITLE		"Kikyu!"
#define SCREEN_WIDTH		854
#define SCREEN_HEIGHT		480
#include "catgl.h"

// プレイヤー
#define PLAYER_WIDTH		48	// 幅
#define PLAYER_HEIGHT		64	// 高さ
#define PLAYER_JUMP		-5	// ジャンプの強さ
#define PLAYER_GRAVIRY		0.25	// 重力
// 敵
#define ENEMY_WIDTH		96/3	// 幅
#define ENEMY_HEIGHT		32	// 高さ
#define ENEMY_SPEED		-5	// 移動スピード
#define ENEMY_HIT_LENGTH	20	// 衝突判定の領域サイズ
#define ENEMY_MAX		20	// 最大出現数
// アイテム
#define ITEM_WIDTH		16	// 幅
#define ITEM_HEIGHT		16	// 高さ
#define ITEM_SPEED		-4	// アイテムのスピード
#define ITEM_MAX		20	// 最大出現数
#define COIN_POINT		100	// コインのポイント
#define COIN_FRAME		14	// コイン画像のフレームインデックス
#define DIAMOND_POINT		1000	// ダイアモンドのポイント
#define DIAMOND_FRAME		64	// ダイアモンドのフレームインデックス
// 背景
#define BACKGROUND_WIDTH	1320
#define BACKGROUND_HEIGHT	320
#define SCROLL_SPEED		 0.004
// 画像
#define IMAGE_PLAYER		"Balloon.png"	// http://korcs.info/material.html
#define IMAGE_ENEMY		"enemy.png"
#define IMAGE_ITEM		"icon0.png"
#define IMAGE_BACKGROUND	"side01.jpg"
#define IMAGE_BACKGROUND2	"side02.jpg"
#define IMAGE_BACKGROUND3	"side03.jpg"
#define IMAGE_BACKGROUND4	"side04.jpg"
//#define IMAGE_BACKGROUND	"side01.png"
#define IMAGE_TITLE		"0627a.png"
// 音
#define TRACK_BGM1		0
#define TRACK_BGM2		1
#define TRACK_SE1		2
#define TRACK_SE2		3
#define BGM_VOL			240
#define BGM_STAGE1		"bgm_maoudamashii_4_vehicle03.mp3"	// http://maoudamashii.jokersounds.com
#define BGM_STAGE2		"bgm_maoudamashii_4_vehicle01.mp3"
#define BGM_STAGE3		"bgm_maoudamashii_4_vehicle02.mp3"
#define BGM_STAGE4		"bgm_maoudamashii_3_theme09.mp3"
//#define BGM_STAGE1		"arasuji_03.mp3"
//#define BGM_GAMESTART		"bgm_gamestart_1.wav"
#define BGM_GAMEOVER		"bgm_gameover_1.mp3"
#define SE_VOL			240
//#define SE_GAMESTART		"se_maoudamashii_system46.mp3"
#define SE_GAMESTART		"se_kusano_06_girl_ikimasu.mp3"	// http://www.otonomori.info/
#define SE_JUMP			"se_jump_short.mp3"	// ユウラボ8bitサウンド工房
//#define SE_PYUU		"se_pyuuuuu.mp3"	// ユウラボ8bitサウンド工房
#define SE_PYUU			"se_kusano_09_girl_u.mp3"
#define SE_ITEM_GET		"itemget.wav"

int game_frame;			// ゲームフレーム
int score, score_plus;		// スコア
int stage;			// ステージ
int enemy_freq;			// 敵発生周期

float bg_scroll;

int player_frame;		// アニメーション
float /*player_vx,*/ player_vy;	// 移動速度
float player_y;

int enemy_frame[ENEMY_MAX];	// アニメーション
int enemy_time[ENEMY_MAX];	// 存在時間

int item_frame[ITEM_MAX];		// アニメーション

NVGcontext* vg;
int width, height;
float pixelRatio;

CATGL_SPRITE s[10];
void (*Scene)();

/*int intersect(ckVec *a, r32 aw, r32 ah, ckVec *b, r32 bw, r32 bh) {
	aw /= 2;
	ah /= 2;
	bw /= 2;
	bh /= 2;
	if (b->x-bw <= a->x+aw && b->y-bh <= a->y+ah && b->x+bw >= a->x-aw && b->y+bh >= a->y-ah) {
	//if (b->x <= a->x+aw && b->y <= a->y+ah && b->x+bw >= a->x && b->y+bh >= a->y) {
//		printf("(%f,%f)-(%f,%f) (%f,%f)-(%f,%f)\n", a->x, a->y, aw, ah, b->x, b->y, bw, bh);
		return 1;
	} else {
		return 0;
	}
}*/

void SceneTitle()
{
	caSpriteRender(&s[6], 0, 0, 0, 0);
	caSpriteRender(&s[7], width/2-196/2, height/2-278/2, 0, 0);

/*	// 消す
	Init();
	player_sprt.dataPos(0).set(-PLAYER_WIDTH-SCREEN_WIDTH, 0);
	//player_sprt.init(1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
	//enemy_sprt.init(ENEMY_MAX, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
	//item_sprt.init(ITEM_MAX, ckDrawMgr::DEFAULT_2D_SCREEN_ID);

	// 背景
	bg_sprt.init(2, m_scr->getID());
	bg_sprt.setTextureID(ckID_(IMAGE_BACKGROUND4));
	bg_sprt.setBlendMode(ckDraw::BLEND_HALF, true);

	bg_sprt.dataPos(1).set(0, 0);	// 真ん中・スプライトの中央の座標
	bg_sprt.setDataSize(1, SCREEN_WIDTH, SCREEN_HEIGHT);
	bg_sprt.setDataUV(1, 0.0f, 0.0f, (float)SCREEN_WIDTH/BACKGROUND_WIDTH, 1);

	// タイトル
	title_sprt.init(1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
	title_sprt.setTextureID(ckID_(IMAGE_TITLE));
	title_sprt.setBlendMode(ckDraw::BLEND_HALF, true);
	//title_sprt.dataPos(0).set(SCREEN_WIDTH/2-196/2, -SCREEN_HEIGHT/2+278/2);
	title_sprt.dataPos(0).set(0, 0);
	title_sprt.setDataSize(0, 196, 278);
	title_sprt.setDataUV(0, 0.0f, 0.0f, 1, 1);

	font.clear();
	font.DrawStringCenter(64, (char*)"Kikyu!");
	//font.DrawPString(-80, 84, (char*)"♥キキュウ♥Aeronaut!");
	//font.DrawPString(-120, -SCREEN_HEIGHT/2+80, (char*)"Press Return to Embark");
	//font.DrawPString(-100, -SCREEN_HEIGHT/2+40, (char*)"(C)2013 YUICHIRO NAKADA");
	font.DrawPStringCenter(84, (char*)"♥キキュウ♥Aeronaut!");
	font.DrawPStringCenter(-SCREEN_HEIGHT/2+80, (char*)"Press Return to Embark");
	font.DrawPStringCenter(-SCREEN_HEIGHT/2+40, (char*)"(C)2013 YUICHIRO NAKADA");

	Scene = &Game::SceneTitle;*/
}

void SceneGameOver()
{
/*	font.clear();
	font.DrawStringCenter(16, (char*)"Game Over", 32, 32);
	char msg[256];
	sprintf(msg, "SCORE: %d", score);
	font.DrawString(-SCREEN_WIDTH/2+16, SCREEN_HEIGHT/2-16, msg); // 0,0
	sprintf(msg, "%dm", game_frame*2);
	font.DrawStringRight(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-16, msg);

	if (ckKeyMgr::isPressed(ckKeyMgr::KEY_ENTER) || ckKeyMgr::isPressed(ckKeyMgr::KEY_LBUTTON)) {
		SceneTitleInit();
	}*/

	Scene = SceneTitle;
}

void SceneGame()
{
	game_frame++;

	// スクロール
	bg_scroll += SCROLL_SPEED;
	if (bg_scroll >= 1.0) bg_scroll = 0;
	caSpriteRender(&s[3], -bg_scroll*SCREEN_WIDTH, 0, 0, 0);

	// プレイヤー処理
	player_vy += PLAYER_GRAVIRY;		// 重力
	player_y += player_vy;
	//caSpriteRender(&s[0], 70, player_y);	// 移動
	caSpriteRender(&s[0], 70, player_y, PLAYER_WIDTH, PLAYER_HEIGHT);

	// フレームアニメーション
	player_frame = game_frame/4 % 24;

	// 画面からはみ出た
	if (player_y + PLAYER_HEIGHT < -SCREEN_HEIGHT/2 || player_y > SCREEN_HEIGHT/*/2*/) {
		//ckSndMgr::play(TRACK_BGM1, ckID_(BGM_GAMEOVER), BGM_VOL, false);
		Scene = SceneGameOver;
	}
}
void SceneGameInit()
{
//	Init();
	bg_scroll = 0;
	player_frame = 0;
	player_vy = 0;
	player_y = 0;

	// 全体
	score = 0;
	score_plus = 0;
	game_frame = 0;
	stage = 1;
	enemy_freq = 60;
	Scene = SceneGame;
//	ckSndMgr::play(TRACK_BGM1, ckID_(BGM_STAGE1), BGM_VOL, true);
//	ckSndMgr::fadeTrackVolume(TRACK_BGM1, BGM_VOL, 40);
}
void keyEvent(int key, int action)
{
	if (action == CATGL_ACTION_DOWN) {
		switch (key) {
		case CATGL_KEY_UP:	// ジャンプ
			player_vy = PLAYER_JUMP;
//			ckSndMgr::play(TRACK_SE1, ckID_(SE_JUMP), SE_VOL, false);
			break;

		case CATGL_KEY_P:	// title
			if (Scene == SceneTitle) Scene = SceneGameInit;
			break;
		}
	}
}
/*void keyEventTitle(int key, int action)
{
	if (action == CATGL_ACTION_DOWN) {
		switch (key) {
		case CATGL_KEY_P:
			Scene = SceneGameInit;
			caKeyEvent = keyEvent;
			break;
		}
	}
}*/
void mouseEvent(int button, int action, int x, int y)
{
	if (action == CATGL_ACTION_DOWN) {
		if (Scene == SceneTitle) Scene = SceneGameInit;
		if (Scene == SceneGame) player_vy = PLAYER_JUMP;
	}
/*	static int lx, ly;
	if (action == CATGL_ACTION_DOWN) {
		lx = x;
		ly = y;
	} else if (action == CATGL_ACTION_MOVE && lx>0) {
		x_angle += (y-ly)/8;
		y_angle += (x-lx)/8;
	} else {
		lx = 0;
	}*/
}

// 表示の初期化
void caInit(int w, int h)
{
	// Calculate pixel ration for hi-dpi devices.
	width = w;
	height = h;
	pixelRatio = (float)width / (float)height;

	vg = nvgCreate(NVG_ANTIALIAS);
	caSpriteLoad(&s[0], IMAGE_PLAYER, vg);
	caSpriteLoad(&s[1], IMAGE_ENEMY, vg);
	caSpriteLoad(&s[2], IMAGE_ITEM, vg);
	caSpriteLoad(&s[3], IMAGE_BACKGROUND, vg);
	caSpriteLoad(&s[4], IMAGE_BACKGROUND2, vg);
	caSpriteLoad(&s[5], IMAGE_BACKGROUND3, vg);
	caSpriteLoad(&s[6], IMAGE_BACKGROUND4, vg);
	caSpriteLoad(&s[7], IMAGE_TITLE, vg);

	score = 0;
	score_plus = 0;
	game_frame = 0;
	stage = 1;
	enemy_freq = 60;
	srand(time(NULL));

	Scene = SceneTitle;
	//caKeyEvent = keyEventTitle;
	caKeyEvent = keyEvent;
	caMouseEvent = mouseEvent;
}

// 描画
void caRender()
{
	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	nvgBeginFrame(vg, width, height, pixelRatio);
	Scene();
//	static int x = 0; x++; x = x>width ? 0: x;
//	caSpriteRender(&s[0], x, sin(x/10.)*height/2+height/2, 0, 0);
	nvgEndFrame(vg);
}

void caEnd()
{
	for (int i=0; i<8; i++) caSpriteDelete(&s[i]);
	nvgDelete(vg);
}
