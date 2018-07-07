//---------------------------------------------------------
//	Kikyu!
//
//		©2013,2018 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_NANOVG
#define CATGL_IMPLEMENTATION
#define SCREEN_TITLE		"Kikyu!"
#define SCREEN_WIDTH		854
#define SCREEN_HEIGHT		480
#include "catgl.h"
#include "font.h"
#include <time.h>

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

CATGL_SPRITE enemy[ENEMY_MAX];
int enemy_frame[ENEMY_MAX];	// アニメーション
int enemy_time[ENEMY_MAX];	// 存在時間

CATGL_SPRITE item[ITEM_MAX];
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
	// 背景
	caSpriteRender(&s[6]);

	// タイトル
	s[7].x = width/2-196/2;
	s[7].y = height/2-278/2;
	caSpriteRender(&s[7]);

	drawStringCenter("Kikyu!", 64, 0, 0, 0);

	// 消す
/*	Init();
	// タイトル
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
	s[stage+2].x = -bg_scroll*(480./768*2048);
	caSpriteRender(&s[stage+2]);
	s[stage+2].x = -bg_scroll*(480./768*2048) +480./768*2048 -1;
	caSpriteRender(&s[stage+2]);

	// プレイヤー処理
	player_frame = game_frame/4 % 24;	// フレームアニメーション
	player_vy += PLAYER_GRAVIRY;		// 重力
	player_y += player_vy;
	s[0].x = 70;
	s[0].y = player_y;
	s[0].sx = (player_frame % (288/PLAYER_WIDTH)) *PLAYER_WIDTH;
	s[0].sy = (player_frame % (256/PLAYER_HEIGHT)) *PLAYER_HEIGHT;
	caSpriteRender(&s[0]);	// 移動

	// 画面からはみ出た
	if (player_y + PLAYER_HEIGHT < -SCREEN_HEIGHT/2 || player_y > SCREEN_HEIGHT/*/2*/) {
		//ckSndMgr::play(TRACK_BGM1, ckID_(BGM_GAMEOVER), BGM_VOL, false);
		Scene = SceneGameOver;
	}

	// 敵を生成
	if (game_frame % enemy_freq == 0) {
		for (int i=0; i<ENEMY_MAX; i++) {
			if (enemy[i].x < -ENEMY_WIDTH-SCREEN_WIDTH/2) {
				enemy_frame[i] = enemy_time[i] = 0;
				enemy[i].x = SCREEN_WIDTH+30;
				enemy[i].y = rand() % SCREEN_HEIGHT/3 +SCREEN_HEIGHT/3;
				break;
			}
		}
	}
	for (int i=0; i<ENEMY_MAX; i++) {
		if (enemy[i].x < -ENEMY_WIDTH-SCREEN_WIDTH/2) continue;

		// 移動
		enemy[i].x += ENEMY_SPEED;
		enemy[i].y += cos(enemy_time[i]*4 % 180);

		// フレームアニメーション
		if (enemy_time[i] % 5 == 0) {
			enemy_frame[i] += 1;
			enemy_frame[i] %= 3;
		}
		enemy[i].sx = ((enemy_frame[i]%3)*1.0/3) *96;
		caSpriteRender(&enemy[i]);

		// プレイヤーとの衝突判定
/*		if (intersect(&enemy_sprt.dataPos(i),
			enemy_sprt.dataW(i)/3, enemy_sprt.dataH(i)/4,
			  &player_sprt.dataPos(0),
			      player_sprt.dataW(0), player_sprt.dataH(0))) {
			// SE 再生
			ckSndMgr::play(TRACK_SE2, ckID_(SE_PYUU), SE_VOL, false);
			//ckSndMgr::fadeTrackVolume(TRACK_BGM1, 0, 40);
			ckSndMgr::play(TRACK_BGM1, ckID_(BGM_GAMEOVER), BGM_VOL, false);
			Scene = &Game::SceneGameOver;
		}*/

		// タイムを進める
		enemy_time[i]++;
	}

	// アイテムを生成
	if (game_frame % 60 == 0) {
		for (int i=0; i<ITEM_MAX; i++) {
			if (item[i].x < -ITEM_WIDTH-SCREEN_WIDTH/2) {
				int r = rand() % 100;
				if (r<10) {
					item_frame[i] = DIAMOND_FRAME;
				} else {
					item_frame[i] = COIN_FRAME;
				}
				item[i].sx = ((item_frame[i]%16)*1.0/16) *256;
				item[i].sy = ((item_frame[i]/16)*1.0/5) *80;
				item[i].x = SCREEN_WIDTH+30;
				item[i].y = rand() % SCREEN_HEIGHT/3 +SCREEN_HEIGHT/3;
				break;
			}
		}
	}
	for (int i=0; i<ITEM_MAX; i++) {
		if (item[i].x < -ITEM_WIDTH-SCREEN_WIDTH/2) continue;

		// 移動
		item[i].x += ITEM_SPEED;
		caSpriteRender(&item[i]);

		// 衝突判定
		/*if (intersect(&item_sprt.dataPos(i),
			item_sprt.dataW(i), item_sprt.dataH(i),
			  &player_sprt.dataPos(0),
			      player_sprt.dataW(0), player_sprt.dataH(0))) {
			// 削除
			item_sprt.dataPos(i).x = -ITEM_WIDTH-SCREEN_WIDTH/2-1;
			if (item_frame[i] == COIN_FRAME) {
				// スコア加算
				score += COIN_POINT -20;
				score_plus += 20;
				font.DrawEString(player_sprt.dataPos(0).x-PLAYER_WIDTH/2, player_sprt.dataPos(0).y, (char*)"+100", 50);
			} else {
				// スコア加算
				score += DIAMOND_POINT -40;
				score_plus += 40;
				font.DrawEString(player_sprt.dataPos(0).x-PLAYER_WIDTH/2, player_sprt.dataPos(0).y, (char*)"+1000", 50);
			}
			// SE 再生
			ckSndMgr::play(TRACK_SE2, ckID_(SE_ITEM_GET), SE_VOL, false);
		}*/
	}

	// スコア表示
	if (score_plus>0) {
		score_plus--;
		score++;
	}
/*	char msg[256];
	sprintf(msg, "SCORE: %d", score);
	font.clear();
	font.DrawString(-SCREEN_WIDTH/2+16, SCREEN_HEIGHT/2-16, msg); // 0,0
	sprintf(msg, "%dm", game_frame*2);
	font.DrawStringRight(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-16, msg);*/

	// ステージ処理
	if (game_frame*2 >= 10000) {
		game_frame = 0;
		stage++;
		switch (stage) {
		case 2:
			enemy_freq = 50;
			break;
		case 3:
			enemy_freq = 40;
			break;
		case 4:
			enemy_freq = 30;
		}
	}
}
void SceneGameInit()
{
//	Init();
	bg_scroll = 0;

	// プレイヤー
	player_frame = 0;
	player_vy = 0;
	player_y = 0;

	// 敵
	for (int i=0; i<ENEMY_MAX; i++) {
		enemy[i] = s[1];
		enemy_frame[i] = 0;
	}

	// アイテム
	for (int i=0; i<ITEM_MAX; i++) {
		item[i] = s[2];
		item_frame[i] = 0;
	}

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
	caFontInit(vg);
	caSpriteLoad(&s[0], IMAGE_PLAYER, vg);
	s[0].w = s[0].sw = PLAYER_WIDTH;
	s[0].h = s[0].sh = PLAYER_HEIGHT;
	caSpriteLoad(&s[1], IMAGE_ENEMY, vg);
	s[1].x = -ENEMY_WIDTH-SCREEN_WIDTH/2;
	s[1].w = s[1].sw = ENEMY_WIDTH;
	s[1].h = s[1].sh = ENEMY_HEIGHT;
	caSpriteLoad(&s[2], IMAGE_ITEM, vg);
	s[2].x = -ITEM_WIDTH-SCREEN_WIDTH/2;
	s[2].w = s[2].sw = ITEM_WIDTH;
	s[2].h = s[2].sh = ITEM_HEIGHT;
	caSpriteLoad(&s[3], IMAGE_BACKGROUND, vg);
	s[3].width  = s[3].w = s[3].sw = 480./768*2048;
	s[3].height = s[3].h = s[3].sh = SCREEN_HEIGHT;
	caSpriteLoad(&s[4], IMAGE_BACKGROUND2, vg);
	s[4].width  = s[4].w = s[4].sw = 480./768*2048;
	s[4].height = s[4].h = s[4].sh = SCREEN_HEIGHT;
	caSpriteLoad(&s[5], IMAGE_BACKGROUND3, vg);
	s[5].width  = s[5].w = s[5].sw = 480./768*2048;
	s[5].height = s[5].h = s[5].sh = SCREEN_HEIGHT;
	caSpriteLoad(&s[6], IMAGE_BACKGROUND4, vg);
	s[6].width  = s[6].w = s[6].sw = 480./768*2048;
	s[6].height = s[6].h = s[6].sh = SCREEN_HEIGHT;
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
	nvgEndFrame(vg);
}

void caEnd()
{
	for (int i=0; i<8; i++) caSpriteDelete(&s[i]);
	nvgDelete(vg);
}
