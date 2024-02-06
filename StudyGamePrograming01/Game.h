/*
/ ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
// struct�̓f�t�H���g�̃A�N�Z�V�r���e�B�� public
// class�̓f�t�H���g�̃A�N�Z�V�r���e�B�� private
struct Vector2
{
public: //��struct�����炠���Ă������Ă�����͓����B
	float x;
	float y;
};

// Game class
class Game
{
public:
	// �R���X�g���N�^
	// Initialize the game
	Game();
	// Runs the game loop until the game is over
	bool Initialize();
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();	// 1.���͂��󂯕t����
	void UpdateGame();		// 2.�Q�[������������
	void GenerateOutput();	// 3.�X�V

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// Pong specific
	// Direction of paddle
	int mPaddleDir;
	// Position of paddle
	Vector2 mPaddlePos;
	// Position of Ball
	Vector2 mBallPos;
	// Velocity of Ball
	Vector2 mBallVel;

	
};z
*/