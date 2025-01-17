#ifndef MYAI_INCLUDED
#define MYAI_INCLUDED 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unordered_map>

#include <time.h>

#define RED 0
#define BLACK 1
#define CHESS_COVER -1
#define CHESS_EMPTY -2
#define COMMAND_NUM 18
class hashvalue
{
	public:
		hashvalue(void);
		double m;
		int depth;
		int exact;
		int move;
};
class MyAI  
{
	const char* commands_name[COMMAND_NUM] = {
		"protocol_version",
		"name",
		"version",
		"known_command",
		"list_commands",
		"quit",
		"boardsize",
		"reset_board",
		"num_repetition",
		"num_moves_to_draw",
		"move",
		"flip",
		"genmove",
		"game_over",
		"ready",
		"time_settings",
		"time_left",
  	"showboard"
	};
public:
	MyAI(void);
	~MyAI(void);

	// commands
	bool protocol_version(const char* data[], char* response);// 0
	bool name(const char* data[], char* response);// 1
	bool version(const char* data[], char* response);// 2
	bool known_command(const char* data[], char* response);// 3
	bool list_commands(const char* data[], char* response);// 4
	bool quit(const char* data[], char* response);// 5
	bool boardsize(const char* data[], char* response);// 6
	bool reset_board(const char* data[], char* response);// 7
	bool num_repetition(const char* data[], char* response);// 8
	bool num_moves_to_draw(const char* data[], char* response);// 9
	bool move(const char* data[], char* response);// 10
	bool flip(const char* data[], char* response);// 11
	bool genmove(const char* data[], char* response);// 12
	bool game_over(const char* data[], char* response);// 13
	bool ready(const char* data[], char* response);// 14
	bool time_settings(const char* data[], char* response);// 15
	bool time_left(const char* data[], char* response);// 16
	bool showboard(const char* data[], char* response);// 17

private:
	int Color;
	int Red_Time, Black_Time;
	int Board[32];
	int CoverChess[14];
	int RemainChess[14];
	int ChessPos[14][5];
	long long int randnum[33][16];
	// long long int key;
	// long long int tmp_key;

	std::unordered_map<long long int, hashvalue> table;
	std::unordered_map<long long int, int> table2;

	int Red_Chess_Num, Black_Chess_Num;
	int node;

	// Utils
	int GetFin(char c);
	int ConvertChessNo(int input);

	// Board
	void initBoardState();
	void generateMove(char move[6]);
	long long int MakeMove(long long int key, int* board, int* red_chess_num, int* black_chess_num, int* cover_chess, int* remain_chess, int chess_pos[][5], const int move, const int chess);
	long long int MakeMove(long long int key, int* board, int* red_chess_num, int* black_chess_num, int* cover_chess, int* remain_chess, int chess_pos[][5], const char move[6]);
	bool Referee(const int* board, const int Startoint, const int EndPoint, const int color);
	int Expand(const int* board, const int color, int *Result, const int chess_pos[][5]);
	double Evaluate(const int red_chess_num, const int black_chess_num, const int* board, const int* remain_chess, const int chess_pos[][5]);
	double Nega_max(long long int key, double alpha, double beta, const int* board, int* move, const int red_chess_num, const int black_chess_num, const int* cover_chess, const int* remain_chess, const int chess_pos[][5], const int color, const int depth, const int remain_depth, const int flip_time);
	double Get_vmax(double score, const int* cover_chess, const int* flip_chess, const int remain_depth);
	double Get_vmin(double score, const int* cover_chess, const int* flip_chess, const int remain_depth);
	// Display
	void Pirnf_Chess(int chess_no,char *Result);
	void Pirnf_Chessboard();
	
};

#endif

