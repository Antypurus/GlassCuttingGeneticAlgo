#pragma once

#include "piece.h"

#include <vector>
#include <set>

using std::vector;
using std::set;
using iart::Piece;

namespace iart
{
	class Board;
}

class SimulatedAnnealing
{
public:
	vector<double> popVal;
private:
	set<vector<Piece>> &GenerateAllPossibleSwapCombinations(const vector<Piece> &pieces);
	set<vector<Piece>> &GenerateAllPossibleMoveCombinations(const vector<Piece> &pieces);

public:
	SimulatedAnnealing()=default;
	~SimulatedAnnealing()=default;
	vector<Piece> run(double temFunc(double),const vector<Piece>& initialState,const iart::Board& board);
	void testGenerateAllPossibleSwapCombinations();
	void testGenerateAllPossibleMoveCombinations();
};