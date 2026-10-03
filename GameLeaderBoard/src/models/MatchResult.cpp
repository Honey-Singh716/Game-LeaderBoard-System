#include "models/MatchResult.h"

std::string toString(MatchResult result) {
    switch (result) {
    case MatchResult::PLAYER_1_WIN:
        return "PLAYER 1 WON";
    case MatchResult::PLAYER_2_WIN:
        return "PLAYER 2 WON";
    case MatchResult::DRAW:
        return "MATCH DRAW";
    }
    return "UNKNOWN";
}
