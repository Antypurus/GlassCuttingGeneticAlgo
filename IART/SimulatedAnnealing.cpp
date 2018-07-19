#define _USE_MATH_DEFINES

#include "SimulatedAnnealing.h"
#include <iostream>
#include <algorithm>
#include <utility>
#include <cmath>

#include "RandomNumberGenerator.h"
#include "Board.h"
#include "PiecesPlacement.h"
#include <memory>

using namespace std;

vector<Piece> SimulatedAnnealing::run(double temFunc(double), const vector<Piece> &initialState,const iart::Board &board)
{
	vector<Piece> initState = initialState;
	vector<Piece>& currstate = initState;
	double currValue = 0;
	PiecesPlacement place(currstate, board);
	place.place();
	double valueGA = place.evaluatePlacementGA();
	currValue = place.evaluatePlacementSA();
	double temp = temFunc(0);
	unsigned int i = 0;
	while(temp>0)
	{
		popVal.emplace_back(valueGA);
		temp = temFunc(i);
		++i;
		set<vector<Piece>> &neighboorhood = this->GenerateAllPossibleMoveCombinations(currstate);
		set<vector<Piece>> &temp_set = this->GenerateAllPossibleSwapCombinations(currstate);
		for(auto& n:temp_set)
		{
			neighboorhood.insert(n);
		}
		for(auto&& n:neighboorhood)
		{
			const iart::Board brd = board;
			std::unique_ptr<PiecesPlacement> placer = std::make_unique<PiecesPlacement>(n, board);
			placer->place();
			const double newValue = placer->evaluatePlacementSA();
			const double deltaE = newValue - currValue;

			//this state grants and imediate improvement
			if(deltaE < 0)
			{
				currstate = n;
				currValue = newValue;
				valueGA = placer->evaluatePlacementGA();
				break;
			}
			else {
				//this state does not grant an imediate improvement
				const double limit = pow(M_E, (deltaE / temp));
				iart::utils::RandomNumberGenerator gen(0.0, 1.0);
				const double prob = gen(0.0);
				if (prob >= limit)
				{
					currstate = n;
					currValue = newValue;
					valueGA = placer->evaluatePlacementGA();
					break;
				}
			}
		}
		neighboorhood.clear();
		delete (&neighboorhood);
		delete (&temp_set);
	}
	return currstate;
}

set<vector<Piece>> &SimulatedAnnealing::GenerateAllPossibleSwapCombinations(const vector<Piece> &pieces)
{
	vector<Piece> currCombination;
	set<vector<Piece>> &combinations = *(new set<vector<Piece>>);
	size_t pieces_size = pieces.size();
	for (size_t i = 0; i < pieces_size; i++)
	{
		for (size_t j = i + 1; j < pieces_size; j++)
		{
			currCombination = pieces; //create a copy of the vector to store the swaping on
			iter_swap(currCombination.begin() + i, currCombination.begin() + j);
			combinations.insert(currCombination);
		}
	}

	return combinations;
}

set<vector<Piece>> &SimulatedAnnealing::GenerateAllPossibleMoveCombinations(const vector<Piece> &pieces)
{
	vector<Piece> currCombination;
	set<vector<Piece>> &combinations = *(new set<vector<Piece>>);
	size_t pieces_size = pieces.size();
	for (size_t i = 0; i < pieces_size; i++)
	{
		for (size_t j = 0; j < pieces_size + 1; j++)
		{
			if (i == j)
				continue;
			currCombination = pieces; //create a copy of the vector to store the swaping on
			Piece piece = pieces[i];
			currCombination.insert(currCombination.begin() + j, piece);
			vector<Piece>::iterator eraseIt;
			if (i < j)
				eraseIt = currCombination.begin() + i;
			else
				eraseIt = currCombination.begin() + i + 1;
			currCombination.erase(eraseIt);
			combinations.insert(currCombination);
		}
	}

	return combinations;
}

void SimulatedAnnealing::testGenerateAllPossibleSwapCombinations()
{
	vector<Piece> pieces;
	pieces.emplace_back(10, 10, Piece::PIECE_TYPE::quad);
	pieces.emplace_back(20, 20, Piece::PIECE_TYPE::quad);
	pieces.emplace_back(30, 30, Piece::PIECE_TYPE::quad);
	pieces.emplace_back(40, 40, Piece::PIECE_TYPE::quad);

	set<vector<Piece>> &combinations = GenerateAllPossibleSwapCombinations(pieces);
	for (auto it = combinations.begin(); it != combinations.end(); it++)
	{
		vector<Piece> curr_combination = *it;
		for (size_t i = 0; i < curr_combination.size(); i++)
		{
			cout << curr_combination[i].get_heigth() << endl;
			cout << curr_combination[i].get_length() << endl;
			cout << curr_combination[i].get_type() << endl;
			cout << endl;
		}
		cout << endl << endl << endl;
	}
}

void SimulatedAnnealing::testGenerateAllPossibleMoveCombinations()
{
	vector<Piece> pieces;
	pieces.emplace_back(10, 10, Piece::PIECE_TYPE::quad);
	pieces.emplace_back(20, 20, Piece::PIECE_TYPE::quad);
	pieces.emplace_back(30, 30, Piece::PIECE_TYPE::quad);
	pieces.emplace_back(40, 40, Piece::PIECE_TYPE::quad);

	set<vector<Piece>> &combinations = GenerateAllPossibleMoveCombinations(pieces);
	for (auto it = combinations.begin(); it != combinations.end(); it++)
	{
		vector<Piece> curr_combination = *it;
		for (size_t i = 0; i < curr_combination.size(); i++)
		{
			cout << curr_combination[i].get_heigth() << endl;
			cout << curr_combination[i].get_length() << endl;
			cout << curr_combination[i].get_type() << endl;
			cout << endl;
		}
		cout << endl << endl << endl;
	}
}