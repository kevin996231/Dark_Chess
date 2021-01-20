#include "float.h"
#include "newAI.h"
#include <time.h> 
#define DEPTH_LIMIT 5
#define FLIP_LIMIT 3

using namespace std;
int count1 = 0;
int count2 = 0;
MyAI::MyAI(void){}
hashvalue::hashvalue(void){}

MyAI::~MyAI(void){}

bool MyAI::protocol_version(const char* data[], char* response){
	strcpy(response, "1.0.0");
  return 0;
}

bool MyAI::name(const char* data[], char* response){
	strcpy(response, "MyAI");
	return 0;
}

bool MyAI::version(const char* data[], char* response){
	strcpy(response, "1.0.0");
	return 0;
}

bool MyAI::known_command(const char* data[], char* response){
  for(int i = 0; i < COMMAND_NUM; i++){
		if(!strcmp(data[0], commands_name[i])){
			strcpy(response, "true");
			return 0;
		}
	}
	strcpy(response, "false");
	return 0;
}

bool MyAI::list_commands(const char* data[], char* response){
  for(int i = 0; i < COMMAND_NUM; i++){
		strcat(response, commands_name[i]);
		if(i < COMMAND_NUM - 1){
			strcat(response, "\n");
		}
	}
	return 0;
}

bool MyAI::quit(const char* data[], char* response){
  fprintf(stderr, "Bye\n"); 
	return 0;
}

bool MyAI::boardsize(const char* data[], char* response){
  fprintf(stderr, "BoardSize: %s x %s\n", data[0], data[1]); 
	return 0;
}

bool MyAI::reset_board(const char* data[], char* response){
	this->Red_Time = -1; // unknown
	this->Black_Time = -1; // unknown
	this->initBoardState();
	return 0;
}

bool MyAI::num_repetition(const char* data[], char* response){
  return 0;
}

bool MyAI::num_moves_to_draw(const char* data[], char* response){
  return 0;
}

bool MyAI::move(const char* data[], char* response){
  char move[6];
	sprintf(move, "%s-%s", data[0], data[1]);

	this->MakeMove((long long int)0, this->Board, &this->Red_Chess_Num, &this->Black_Chess_Num, this->CoverChess, this->RemainChess, this->ChessPos, move);
	return 0;
}

bool MyAI::flip(const char* data[], char* response){
  char move[6];
	sprintf(move, "%s(%s)", data[0], data[1]);
	this->MakeMove((long long int)0, this->Board, &this->Red_Chess_Num, &this->Black_Chess_Num, this->CoverChess, this->RemainChess, this->ChessPos, move);
	return 0;
}

bool MyAI::genmove(const char* data[], char* response){
	// set color
	if(!strcmp(data[0], "red")){
		this->Color = RED;
	}else if(!strcmp(data[0], "black")){
		this->Color = BLACK;
	}else{
		this->Color = 2;
	}
	// genmove
  char move[6];
	this->generateMove(move);
	sprintf(response, "%c%c %c%c", move[0], move[1], move[3], move[4]);
	return 0;
}

bool MyAI::game_over(const char* data[], char* response){
  fprintf(stderr, "Game Result: %s\n", data[0]); 
	return 0;
}

bool MyAI::ready(const char* data[], char* response){
  return 0;
}

bool MyAI::time_settings(const char* data[], char* response){
  return 0;
}

bool MyAI::time_left(const char* data[], char* response){
  if(!strcmp(data[0], "red")){
		sscanf(data[1], "%d", &(this->Red_Time));
	}else{
		sscanf(data[1], "%d", &(this->Black_Time));
	}
	fprintf(stderr, "Time Left(%s): %s\n", data[0], data[1]); 
	return 0;
}

bool MyAI::showboard(const char* data[], char* response){
  Pirnf_Chessboard();
	return 0;
}


// *********************** AI FUNCTION *********************** //

int MyAI::GetFin(char c)
{
	static const char skind[]={'-','K','G','M','R','N','C','P','X','k','g','m','r','n','c','p'};
	for(int f=0;f<16;f++)if(c==skind[f])return f;
	return -1;
}

int MyAI::ConvertChessNo(int input)
{
	switch(input)
	{
	case 0:
		return CHESS_EMPTY;
		break;
	case 8:
		return CHESS_COVER;
		break;
	case 1:
		return 6;
		break;
	case 2:
		return 5;
		break;
	case 3:
		return 4;
		break;
	case 4:
		return 3;
		break;
	case 5:
		return 2;
		break;
	case 6:
		return 1;
		break;
	case 7:
		return 0;
		break;
	case 9:
		return 13;
		break;
	case 10:
		return 12;
		break;
	case 11:
		return 11;
		break;
	case 12:
		return 10;
		break;
	case 13:
		return 9;
		break;
	case 14:
		return 8;
		break;
	case 15:
		return 7;
		break;
	}
	return -1;
}


void MyAI::initBoardState()
{	
	int iPieceCount[14] = {5, 2, 2, 2, 2, 2, 1, 5, 2, 2, 2, 2, 2, 1};
	memcpy(this->CoverChess,iPieceCount,sizeof(int)*14);
	memcpy(this->RemainChess,iPieceCount,sizeof(int)*14);

	Red_Chess_Num = 16; Black_Chess_Num = 16;
	for(int i=0;i<14;i++)
		for(int j=0;j<5;j++)
			this->ChessPos[i][j] = -1;
	//convert to my format
	int Index = 0;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<4;j++)
		{
			this->Board[Index] = CHESS_COVER;
			Index++;
		}
	}
	srand (time(NULL));
	for(int i=0; i<32; i++)
		for(int j=0; j<16; j++){
			this->randnum[i][j] = (((long long int)rand()) << 32) + rand();
			if(j == 0)
				fprintf(stderr, "%lld\n", this->randnum[i][j]);
		}

	// for color
	this->randnum[32][0] = (((long long int)rand()) << 32) + rand();

	
	this->Pirnf_Chessboard();
}

void MyAI::generateMove(char move[6])
{
	/* generateMove Call by reference: change src,dst */
	this->node = 0;
	int startPoint = 0;
	int EndPoint = 0;
	int cover_count = 0;
	int best_move = 0;
	long long int key = 0;
	for(int i=0; i<32; i++)
		if(this->Board[i] == CHESS_COVER)
			cover_count++;
	for(int i=0; i<32; i++)
		key ^= this->randnum[i][this->Board[i]];
	key ^= this->randnum[32][0];
	int depth = DEPTH_LIMIT;
	// if(cover_count < 20)
	// 	depth +=1;
	// if(cover_count < 10)
	// 	depth +=1;
	if(cover_count < 5)
		depth +=1;
	double t = Nega_max(key, -DBL_MAX, DBL_MAX, this->Board, &best_move, this->Red_Chess_Num, this->Black_Chess_Num, this->CoverChess, this->RemainChess, this->ChessPos, this->Color, 0, depth, 0);
	fprintf(stderr, "cut = %d %d %d\n", count1, count2, this->node);

	startPoint = best_move/100;
	EndPoint   = best_move%100;
	sprintf(move, "%c%c-%c%c",'a'+(startPoint%4),'1'+(7-startPoint/4),'a'+(EndPoint%4),'1'+(7-EndPoint/4));

	char chess_Start[4]="";
	char chess_End[4]="";
	Pirnf_Chess(Board[startPoint],chess_Start);
	Pirnf_Chess(Board[EndPoint],chess_End);
	printf("My result: \n--------------------------\n");
	printf("Nega max: %lf (node: %d)\n", t, this->node);
	printf("(%d) -> (%d)\n",startPoint,EndPoint);
	printf("<%s> -> <%s>\n",chess_Start,chess_End);
	printf("move:%s\n",move);
	printf("--------------------------\n");
	this->Pirnf_Chessboard();
}

long long int MyAI::MakeMove(long long int key, int* board, int* red_chess_num, int* black_chess_num, int* cover_chess, int* remain_chess, int chess_pos[][5], const int move, const int chess){
	static const int iPieceCount[14] = {5, 2, 2, 2, 2, 2, 1, 5, 2, 2, 2, 2, 2, 1};
	
	int src = move/100, dst = move%100;
	if(src == dst){ 
		board[src] = chess;
		chess_pos[chess][iPieceCount[chess]-cover_chess[chess]] = src;
		cover_chess[chess]--;
		key ^= this->randnum[src][14];
		key ^= this->randnum[src][chess];
	}else { 
		if(board[dst] != CHESS_EMPTY){
			if(board[dst] / 7 == 0){
				(*red_chess_num)--;
			}else{
				(*black_chess_num)--;
			}
			remain_chess[board[dst]]--;
			for(int i=0; i<5; i++){
				if(chess_pos[board[dst]][i] == dst){
					chess_pos[board[dst]][i] = -1;
					break;
				}
			}
		}
		for(int i=0; i<5; i++){
			if(chess_pos[board[src]][i] == src){
				chess_pos[board[src]][i] = dst;
				break;
			}
		}



		key ^= this->randnum[src][board[src]];
		key ^= this->randnum[src][15];
		key ^= this->randnum[dst][board[dst]];
		key ^= this->randnum[dst][board[src]];

		board[dst] = board[src];
		board[src] = CHESS_EMPTY;
	}
	return key;
}

long long int MyAI::MakeMove(long long int key, int* board, int* red_chess_num, int* black_chess_num, int* cover_chess, int* remain_chess, int chess_pos[][5], const char move[6])
{ 
	static const int iPieceCount[14] = {5, 2, 2, 2, 2, 2, 1, 5, 2, 2, 2, 2, 2, 1};
	int src, dst;
	src = ('8'-move[1])*4+(move[0]-'a');
	if(move[2]=='('){ 
		printf("# call flip(): flip(%d,%d) = %d\n",src, src, GetFin(move[3])); 
		int chess = ConvertChessNo(GetFin(move[3]));
		board[src] = chess;
		chess_pos[chess][iPieceCount[chess]-cover_chess[chess]] = src;
		cover_chess[chess]--;
		key ^= this->randnum[src][14];
		key ^= this->randnum[src][chess];

		Pirnf_Chessboard();
	}else { 
		dst = ('8'-move[4])*4+(move[3]-'a');
		printf("# call move(): move : %d-%d \n",src,dst); 
		if(board[dst] != CHESS_EMPTY){
			if(board[dst] / 7 == 0){
				(*red_chess_num)--;
			}else{
				(*black_chess_num)--;
			}
			remain_chess[board[dst]]--;			
			for(int i=0; i<5; i++){
				if(chess_pos[board[dst]][i] == dst){
					chess_pos[board[dst]][i] = -1;
					break;
				}
			}
		}
		for(int i=0; i<5; i++){
			if(chess_pos[board[src]][i] == src){
				chess_pos[board[src]][i] = dst;
				break;
			}
		}
		key ^= this->randnum[src][board[src]];
		key ^= this->randnum[src][15];
		key ^= this->randnum[dst][board[dst]];
		key ^= this->randnum[dst][board[src]];
		board[dst] = board[src];
		board[src] = CHESS_EMPTY;
		Pirnf_Chessboard();
	}
	return key;
	/* init time */
}

int MyAI::Expand(const int* board, const int color,int *Result, const int chess_pos[][5])
{
	static const int order[7] = {6,5,1,4,3,2,0};

	int tmp_ResultCount = 0;
	int eat[2000] = {0}, tmp_Result[2000];
	for(int t=0;t<7;t++)
	{
		int j = order[t];
		if(color)
			j += 7;
		for(int k=0; k<5; k++){
			if(chess_pos[j][k] == -1 )
				continue;
		
			int i = chess_pos[j][k];
			if(board[i] >= 0 && board[i] < 14 && board[i]/7 == color)
			{
			//Gun
				if(board[i] % 7 == 1)
				{
					int row = i/4;
					int col = i%4;
					for(int rowCount=row*4;rowCount<(row+1)*4;rowCount++)
					{
						if(Referee(board,i,rowCount,color))
						{
							if(board[rowCount] != CHESS_COVER && board[rowCount] != CHESS_EMPTY && board[rowCount]/7 != board[i]/7)
								eat[tmp_ResultCount] = 1;
							tmp_Result[tmp_ResultCount] = i*100+rowCount;
							tmp_ResultCount++;
						}
					}
					for(int colCount=col; colCount<32;colCount += 4)
					{
					
						if(Referee(board,i,colCount,color))
						{						
							if(board[colCount] != CHESS_COVER && board[colCount] != CHESS_EMPTY && board[colCount]/7 != board[i]/7)
								eat[tmp_ResultCount] = 1;
							tmp_Result[tmp_ResultCount] = i*100+colCount;
							tmp_ResultCount++;
						}
					}
				}
				else
				{
					int Move[4] = {i-4,i+1,i+4,i-1};
					for(int k=0; k<4;k++)
					{
						
						if(Move[k] >= 0 && Move[k] < 32 && Referee(board,i,Move[k],color))
						{
							if(board[Move[k]] != CHESS_COVER && board[Move[k]] != CHESS_EMPTY && board[Move[k]]/7 != board[i]/7)
								eat[tmp_ResultCount] = 1;

							tmp_Result[tmp_ResultCount] = i*100+Move[k];
							tmp_ResultCount++;
						}
					}
				}
			}
		}
		
		
	}
	int ResultCount = 0;
	for(int i=0; i<tmp_ResultCount; i++)
		if(eat[i]){
			Result[ResultCount] = tmp_Result[i];
			ResultCount++;
		}
	for(int i=0; i<tmp_ResultCount; i++)
		if(!eat[i]){
			Result[ResultCount] = tmp_Result[i];
			ResultCount++;
		}
	// Result = newResult;
	return ResultCount;
}


// Referee
bool MyAI::Referee(const int* chess, const int from_location_no, const int to_location_no, const int UserId)
{
	int MessageNo = 0;
	bool IsCurrent = true;
	int from_chess_no = chess[from_location_no];
	int to_chess_no = chess[to_location_no];
	int from_row = from_location_no / 4;
	int to_row = to_location_no / 4;
	int from_col = from_location_no % 4;
	int to_col = to_location_no % 4;

	if(from_chess_no < 0 || ( to_chess_no < 0 && to_chess_no != CHESS_EMPTY) )
	{  
		MessageNo = 1;
		//strcat(Message,"**no chess can move**");
		//strcat(Message,"**can't move darkchess**");
		IsCurrent = false;
	}
	else if (from_chess_no >= 0 && from_chess_no / 7 != UserId)
	{
		MessageNo = 2;
		//strcat(Message,"**not my chess**");
		IsCurrent = false;
	}
	else if((from_chess_no / 7 == to_chess_no / 7) && to_chess_no >= 0)
	{
		MessageNo = 3;
		//strcat(Message,"**can't eat my self**");
		IsCurrent = false;
	}
	//check attack
	else if(to_chess_no == CHESS_EMPTY && abs(from_row-to_row) + abs(from_col-to_col)  == 1)//legal move
	{
		IsCurrent = true;
	}	
	else if(from_chess_no % 7 == 1 ) //judge gun
	{
		int row_gap = from_row-to_row;
		int col_gap = from_col-to_col;
		int between_Count = 0;
		//slant
		if(from_row-to_row == 0 || from_col-to_col == 0)
		{
			//row
			if(row_gap == 0) 
			{
				for(int i=1;i<abs(col_gap);i++)
				{
					int between_chess;
					if(col_gap>0)
						between_chess = chess[from_location_no-i] ;
					else
						between_chess = chess[from_location_no+i] ;
					if(between_chess != CHESS_EMPTY)
						between_Count++;
				}
			}
			//column
			else
			{
				for(int i=1;i<abs(row_gap);i++)
				{
					int between_chess;
					if(row_gap > 0)
						between_chess = chess[from_location_no-4*i] ;
					else
						between_chess = chess[from_location_no+4*i] ;
					if(between_chess != CHESS_EMPTY)
						between_Count++;
				}
				
			}
			
			if(between_Count != 1 )
			{
				MessageNo = 4;
				//strcat(Message,"**gun can't eat opp without between one piece**");
				IsCurrent = false;
			}
			else if(to_chess_no == CHESS_EMPTY)
			{
				MessageNo = 5;
				//strcat(Message,"**gun can't eat opp without between one piece**");
				IsCurrent = false;
			}
		}
		//slide
		else
		{
			MessageNo = 6;
			//strcat(Message,"**cant slide**");
			IsCurrent = false;
		}
	}
	else // non gun
	{
		//judge pawn or king

		//distance
		if( abs(from_row-to_row) + abs(from_col-to_col)  > 1)
		{
			MessageNo = 7;
			//strcat(Message,"**cant eat**");
			IsCurrent = false;
		}
		//judge pawn
		else if(from_chess_no % 7 == 0)
		{
			if(to_chess_no % 7 != 0 && to_chess_no % 7 != 6)
			{
				MessageNo = 8;
				//strcat(Message,"**pawn only eat pawn and king**");
				IsCurrent = false;
			}
		}
		//judge king
		else if(from_chess_no % 7 == 6 && to_chess_no % 7 == 0)
		{
			MessageNo = 9;
			//strcat(Message,"**king can't eat pawn**");
			IsCurrent = false;
		}
		else if(from_chess_no % 7 < to_chess_no% 7)
		{
			MessageNo = 10;
			//strcat(Message,"**cant eat**");
			IsCurrent = false;
		}
	}
	return IsCurrent;
}
double MyAI::Evaluate(const int* board){
	// total score
	double score = 1943; // 1*5+180*2+6*2+18*2+90*2+270*2+810*1
	// static material values
	// cover and empty are both zero
	static const double values[14] = {1,180,6,18,90,270,810,1,180,6,18,90,270,810};
	for(int i = 0; i < 32; i++){
		if(!(board[i] == CHESS_EMPTY || board[i] == CHESS_COVER)){
			if(board[i] / 7 == this->Color){
				score += values[board[i]];
                // score+=60;
			}else{
				score -= values[board[i]];
                // score -= 60;
			}
		}
	}

	return score;
}

double MyAI::Get_vmax(double score, const int* cover_chess, const int* flip_chess, const int remain_depth){
	// return 3886;
    int num = remain_depth/2 + 1;
	// total score
    int order0[7] = {6,5,1,4,3,2,0};
    int order1[7] = {13,12,8,11,10,9,7};
    int chess_count[7];

    if(this->Color)
        for(int i = 0 ; i < 7; i++)
            chess_count[i] = cover_chess[order1[i]] + flip_chess[order0[i]] + cover_chess[order0[i]];
    else
        for(int i = 0 ; i < 7; i++)
            chess_count[i] = cover_chess[order0[i]] + flip_chess[order1[i]] + cover_chess[order1[i]];
	// static const double values[14] = {1,180,6,18,90,270,810,1,180,6,18,90,270,810};
	static const double values[7] = {810, 270, 180, 90, 18, 6, 1};


    // return score*2;
    for(int i = 0; i < 7; i++){
        if(num <= 0)
            break;
        if(num >= chess_count[i])
            score += values[i]*chess_count[i];
        else 
            score += values[i]*num;
        num -= chess_count[i];
    }
	return score;
}

double MyAI::Get_vmin(double score, const int* cover_chess, const int* flip_chess, const int remain_depth){
	// return 0;
    int num = remain_depth/2 + 1;
	// total score
    int order0[7] = {6,5,1,4,3,2,0};
    int order1[7] = {13,12,8,11,10,9,7};
    int chess_count[7];

    if(this->Color)
        for(int i = 0 ; i < 7; i++)
            chess_count[i] = cover_chess[order0[i]] + flip_chess[order1[i]] + cover_chess[order1[i]];
    else
        for(int i = 0 ; i < 7; i++)
            chess_count[i] = cover_chess[order1[i]] + flip_chess[order0[i]] + cover_chess[order0[i]];
	// static const double values[14] = {1,180,6,18,90,270,810,1,180,6,18,90,270,810};
	static const double values[7] = {810, 270, 180, 90, 18, 6, 1};


    // return score*2;
    for(int i = 0; i < 7 && num > 0; i++){
        if(num >= chess_count[i])
            score -= values[i]*chess_count[i];
        else 
            score -= values[i]*num;
        num -= chess_count[i];
    }
	return score;
}       

double MyAI::Nega_max(long long int key, double alpha, double beta, const int* board, int* move, const int red_chess_num, const int black_chess_num, const int* cover_chess, const int* remain_chess, const int chess_pos[][5], const int color, const int depth, const int remain_depth, const int flip_time){
    if(remain_depth == 0 || flip_time >= FLIP_LIMIT){ // reach limit of depth
		this->node++;
		return Evaluate(board) * (2*((depth&1)^1)-1); // odd: *-1, even: *1
	}else if(red_chess_num == 0 || black_chess_num == 0){ // terminal node (no chess type)
		this->node++;
		if((red_chess_num == 0 && color == 1) || (black_chess_num == 0 && color == 0))
		 	return 10000;
		else 
			return -10000;
	}

	hashvalue h, newh;
	double m = -DBL_MAX;

	if (this->table.find(key) != this->table.end()){
		h = this->table.at(key);
		if(h.depth >= remain_depth){
			if(h.exact == 0){
				if(depth != 0 || this->table2.find(key) == this->table2.end() || this->table2[key] != h.move){
					*move = h.move;
					return h.m;
				}else
				{
					fprintf(stderr, "123123\n");
				}
				
			}else if(h.exact == 1){
				alpha = alpha>h.m?alpha:h.m;
			}else{
				beta = beta>h.m?h.m:beta;
			}
		}else{
			if(h.exact == 0){
				if(depth != 0 || this->table2.find(key) == this->table2.end() || this->table2[key] != h.move){
					m = h.m;
					*move = h.move;
				}
				if(depth == 0)
                	fprintf(stderr, "test %f\n", h.m);

			}
		}
	}

	
	
	static const int order[14] = {6,5,1,4,3,2,0,7,9,10,11,8,12,13};
    // static const int black_order[7] = {13,12,8,11,10,9,7,};
	int Result[1024];
	// Moves[] = {move} U {flip}, Chess[] = {remain chess}
	int Moves[2048], Chess[2048];
	int flip_count = 0, remain_count = 0, remain_total = 0;
	int move_count = 0;
    int flip_chess[14] = {0};

	// move
	move_count = Expand(board, color, Result, chess_pos);
	memcpy(Moves, Result, sizeof(int)*move_count);
	// memcpy(alive_chess, cover_chess, sizeof(int)*14);

	// flip
	if(color == RED)
		for(int j = 0; j < 14; j++){ // find remain chess
			if(cover_chess[order[j]] > 0){
				Chess[remain_count] = order[j];
				remain_count++;
				remain_total += cover_chess[order[j]];
			}
		}
	else
		for(int j = 13; j >= 0; j--){ // find remain chess
			if(cover_chess[order[j]] > 0){
				Chess[remain_count] = order[j];
				remain_count++;
				remain_total += cover_chess[order[j]];
			}
		}

	for(int i = 0; i < 32; i++)
		if(!(board[i] == CHESS_EMPTY || board[i] == CHESS_COVER))
            flip_chess[board[i]] ++;

	for(int i = 0; i < 32; i++){ // find cover
		if(board[i] == CHESS_COVER){
			Moves[move_count + flip_count] = i*100+i;
			flip_count++;
		}
	}
    if(flip_count == 32){
        fprintf(stderr, "hahaha %f\n", Evaluate(board));
        fprintf(stderr, "hahaha %f\n", 3*3.3);
        *move = 909;
        return 0;
    }

	if(move_count + flip_count == 0){ // terminal node (no move type)
		this->node++;
		return Evaluate(board) * (2*((depth&1)^1)-1);
	}else{
        double n = beta, t;

		int new_board[32], new_cover[14], new_remain[14], new_chess_pos[14][5], new_move, new_red, new_black;
		// search deeper
		long long int tmp_key = key;
		int prevent_repeat = -1;
		if(depth == 0 && this->table2.find(key) != this->table2.end() ){
			prevent_repeat = this->table2[key];
		}

		for(int i = 0; i < move_count; i++){ // move
			if(prevent_repeat == Moves[i])
				continue;
			new_red = red_chess_num, new_black = black_chess_num;

			memcpy(new_board, board, sizeof(int)*32);
			memcpy(new_cover, cover_chess, sizeof(int)*14);
			memcpy(new_remain, remain_chess, sizeof(int)*14);
			memcpy(new_chess_pos, chess_pos, sizeof(int)*70);

			
			key = MakeMove(key, new_board, &new_red, &new_black, new_cover, new_remain, new_chess_pos, Moves[i], -1); // -1: NULL
			// key ^= this->randnum[src][CHESS_COVER];
			// key ^= this->randnum[src][chess];
			t = -Nega_max(key^this->randnum[32][0], -n, -(alpha>m?alpha:m), new_board, &new_move, new_red, new_black, new_cover, new_remain, new_chess_pos, color^1, depth+1, remain_depth-1, flip_time);
			key = tmp_key;
            if(t > m){ 
                if(n == beta || remain_depth < 3 || t >= beta){
                    m = t;
				    *move = Moves[i];
                } else{
                    memcpy(new_board, board, sizeof(int)*32);
                    memcpy(new_cover, cover_chess, sizeof(int)*14);
                	memcpy(new_remain, remain_chess, sizeof(int)*14);
					memcpy(new_chess_pos, chess_pos, sizeof(int)*70);
                    key = MakeMove(key, new_board, &new_red, &new_black, new_cover, new_remain, new_chess_pos, Moves[i], -1); // -1: NULL
                    m = -Nega_max(key^this->randnum[32][0], -beta, -t, new_board, &new_move, new_red, new_black, new_cover, new_remain, new_chess_pos, color^1, depth+1, remain_depth-1, flip_time);
					key = tmp_key;
					
					*move = Moves[i];
                    // if(m < t)
                    //     m = t;
                }
			}
            // else if(t == m){
			// 	bool r = rand()%2;
			// 	if(r) *move = Moves[i];
			// }
            if(depth == 0){
                fprintf(stderr, "Move: %d, score: %f m: %f %d\n", Moves[i], t, m, new_move);
            }
            if(m >= beta){
                count1 ++;
				newh.depth = remain_depth;
				newh.exact = 1;
				newh.move = *move;
				newh.m = m;
				this->table[key] = newh;
                return m;

            }

            n = (alpha>m?alpha:m) + 0.01;

		}
        double A0,B0, vmin, vmax;
        if(flip_count > 0){
            double score = Evaluate(board);
            if(depth == 0){
                fprintf(stderr, "score: %f\n", score);
            }
            vmax = Get_vmax(score, cover_chess, flip_chess, remain_depth) * (2*((depth&1)^1)-1);;
            vmin = Get_vmin(score, cover_chess, flip_chess, remain_depth) * (2*((depth&1)^1)-1);;
            if(depth&1){
                double tmp = vmax;
                vmax = vmin;
                vmin = tmp;
            }

            // double m0 = vmin;
            // double M0 = vmax;
        }
       

		for(int i = move_count; i < move_count + flip_count; i++){ // flip
            double sum = 0.0;
			double total = 0;
            double mm = vmin, M = vmax;
            double A, B;
            int flag = 0;
            A = (double)remain_total/cover_chess[Chess[0]] * ((alpha>m?alpha:m) - vmax) + vmax;
            B = (double)remain_total/cover_chess[Chess[0]] * (beta - vmin) + vmin;
			for(int k = 0; k < remain_count; k++){
				new_red = red_chess_num, new_black = black_chess_num;
				memcpy(new_board, board, sizeof(int)*32);
				memcpy(new_cover, cover_chess, sizeof(int)*14);
				memcpy(new_remain, remain_chess, sizeof(int)*14);
				memcpy(new_chess_pos, chess_pos, sizeof(int)*70);
				key = MakeMove(key, new_board, &new_red, &new_black, new_cover, new_remain, new_chess_pos, Moves[i], Chess[k]);
                // if(remain_depth > 3)
				//     t = -Nega_max(-(B>vmax?vmax:B), -(A>vmin?A:vmin), new_board, &new_move, new_red, new_black, new_cover, color^1, depth+1, 2);
                // else 
				t = -Nega_max(key^this->randnum[32][0], -(B>vmax?vmax:B), -(A>vmin?A:vmin), new_board, &new_move, new_red, new_black, new_cover, new_remain, new_chess_pos, color^1, depth+1, remain_depth-1, flip_time+1);
				key = tmp_key;				
				mm = mm + (t-vmin)/remain_total * cover_chess[Chess[k]];
				M = M + (t-vmax)/remain_total * cover_chess[Chess[k]];
                if(t >= B) {
                    flag = 1;
                    sum = mm;
                    break;
                }
                if(t <= A) {
                    flag = 1;
                    sum = M;
                    break;
                }
                sum += t * cover_chess[Chess[k]];
                if( k != remain_count - 1){ 
                    A =  ((double)cover_chess[Chess[k]]/cover_chess[Chess[k+1]]) * (A - t) + vmax;
                    B =  ((double)cover_chess[Chess[k]]/cover_chess[Chess[k+1]]) * (B - t) + vmin;
                }
                // A = A + (vmax - t) * cover_chess[Chess[k]];
                // B = B + (vmin - t) * cover_chess[Chess[k]];
            }
            if(!flag)
                sum /= remain_total;
            if(sum > m){ 
                m = sum;
                *move = Moves[i];
			} 
            // else if(sum == m){
			// 	bool r = rand()%2;
			// 	if(r) *move = Moves[i];
			// }
            if(depth == 0){
                fprintf(stderr, "Move: %d, score: %f m: %f\n", Moves[i], sum, m);
            }
            if(m >= beta){
				newh.depth = remain_depth;
				newh.exact = 1;
				newh.move = *move;
				newh.m = m;
				this->table[key] = newh;
                count2++;
                return m;

            }

		}
		newh.depth = depth;
		newh.move = *move;
		newh.m = m;
		if(m > alpha){
			newh.exact = 0;
		}else {
			newh.exact = 2;
		}
		this->table[key] = newh;
		if(depth == 0)
			this->table2[key] = *move;
		return m;
	}
}

//Display chess board
void MyAI::Pirnf_Chessboard()
{	
	char Mes[1024]="";
	char temp[1024];
	char myColor[10]="";
	if(Color == -99)
		strcpy(myColor,"Unknown");
	else if(this->Color == RED)
		strcpy(myColor,"Red");
	else
		strcpy(myColor,"Black");
	sprintf(temp,"------------%s-------------\n",myColor);
	strcat(Mes,temp);
	strcat(Mes,"<8> ");
	for(int i=0;i<32;i++)
	{
		if(i != 0 && i % 4 == 0)
		{
			sprintf(temp,"\n<%d> ",8-(i/4));
			strcat(Mes,temp);
		}
		char chess_name[4]="";
		Pirnf_Chess(this->Board[i],chess_name);
		sprintf(temp,"%5s", chess_name);
		strcat(Mes,temp);
	}
	strcat(Mes,"\n\n     ");
	for(int i=0;i<4;i++)
	{
		sprintf(temp," <%c> ",'a'+i);
		strcat(Mes,temp);
	}
	strcat(Mes,"\n\n");
	printf("%s",Mes);
}


//Print chess
void  MyAI::Pirnf_Chess(int chess_no,char *Result)
{
		// XX -> Empty
		if(chess_no == CHESS_EMPTY)
		{	
			strcat(Result, " - ");
			return;
		}
		//OO -> DarkChess
		else if(chess_no == CHESS_COVER)
		{
			strcat(Result, " X ");
			return;
		}
		/*if(Color == RED )
				strcat(Result, "R");
		else
				strcat(Result, "B");*/
		switch(chess_no)
		{
		case 0:
				strcat(Result, " P ");
				break;
		case 1:
				strcat(Result, " C ");
				break;
		case 2:
				strcat(Result, " N ");
				break;
		case 3:
				strcat(Result, " R ");
				break;
		case 4:
				strcat(Result, " M ");
				break;
		case 5:
				strcat(Result, " G ");
				break;
		case 6:
				strcat(Result, " K ");
				break;
		case 7:
				strcat(Result, " p ");
				break;
		case 8:
				strcat(Result, " c ");
				break;
		case 9:
				strcat(Result, " n ");
				break;
		case 10:
				strcat(Result, " r ");
				break;
		case 11:
				strcat(Result, " m ");
				break;
		case 12:
				strcat(Result, " g ");
				break;
		case 13:
				strcat(Result, " k ");
				break;
	}
}


