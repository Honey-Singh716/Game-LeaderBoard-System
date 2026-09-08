#include <bits/stdc++.h>

using namespace std;

struct Players {
    int id;
    string name;
    int score;
    int rating;
    int wins;
    int losses;
};

struct HeapEntry {
    int playerId;
    int score;
};

struct CompareScore {
    bool operator()(const HeapEntry& a, const HeapEntry& b) {
        if (a.score != b.score) {
            return a.score > b.score;
        }

        return a.playerId > b.playerId;
    }
};

class GameSystem {
private:
    unordered_map<int, Players> players;

public:

    void addPlayer(int id, string name, int rating) {
        if (players.find(id) != players.end()) {
            cout << "Player already exists\n";
            return;
        }

        Players p;

        p.id = id;
        p.name = name;
        p.score = 0;
        p.rating = rating;
        p.wins = 0;
        p.losses = 0;

        players[id] = p;
    }

    Players* findPlayer(int id) {
        auto it = players.find(id);

        if (it == players.end()) {
            return nullptr;
        }

        return &it->second;
    }

    void updateScore(int id, int score) {
        Players* player = findPlayer(id);

        if (player == nullptr) {
            cout << "Player not found\n";
            return;
        }

        player->score += score;
    }

    void showPlayer(int id) {
        Players* player = findPlayer(id);

        if (player == nullptr) {
            cout << "Player not found\n";
            return;
        }

        cout << "ID: " << player->id << '\n';
        cout << "Name: " << player->name << '\n';
        cout << "Score: " << player->score << '\n';
        cout << "Rating: " << player->rating << '\n';
        cout << "Wins: " << player->wins << '\n';
        cout << "Losses: " << player->losses << '\n';
    }

    vector<Players> getTopK(int k) {
        vector<Players> result;

        if (k <= 0) {
            return result;
        }

        priority_queue<
            HeapEntry,
            vector<HeapEntry>,
            CompareScore
        > minHeap;

        for (const auto& [id, player] : players) {
            minHeap.push({id, player.score});

            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }

        while (!minHeap.empty()) {
            HeapEntry entry = minHeap.top();
            minHeap.pop();

            result.push_back(players.at(entry.playerId));
        }

        reverse(result.begin(), result.end());

        return result;
    }

    void showTopK(int k) {
        vector<Players> topPlayers = getTopK(k);

        cout << "############ Top " << k << " Players ############\n";

        int rank = 1;

        for (const auto& player : topPlayers) {
            cout << "Rank " << rank++
                 << ", Name: " << player.name
                 << ", Score: " << player.score << '\n';
        }
    }
    void removePlayer(int id) {

        auto it = players.find(id);

        if (it == players.end()) {
            cout << "Player not found\n";
            return;
        }

        cout << it->second.name << " removed successfully\n";

        players.erase(it);
    }


    int findRank(int id) {

        Players* target = findPlayer(id);

        if (target == nullptr) {
            return -1;
        }

        int rank = 1;

        for (const auto& [playerId, player] : players) {

            if (player.score > target->score) {
                rank++;
            }
        }

        return rank;
    }


    void showRank(int id) {

        Players* player = findPlayer(id);

        if (player == nullptr) {
            cout << "Player not found\n";
            return;
        }

        int rank = findRank(id);

        cout << "Player: " << player->name
            << " | Score: " << player->score
            << " | Rank: " << rank
            << '\n';
    }

};

int main() {

    GameSystem game;

    game.addPlayer(101, "ShadowX", 1500);
    game.addPlayer(102, "Blaze", 1600);
    game.addPlayer(103, "Nova", 1450);
    game.addPlayer(104, "Ghost", 1700);
    game.addPlayer(105, "Dragon", 1550);

    game.updateScore(101, 900);
    game.updateScore(102, 500);
    game.updateScore(103, 1200);
    game.updateScore(104, 700);
    game.updateScore(105, 1500);

    game.showTopK(3);

    cout << "\n===== AFTER UPDATE =====\n";

    game.updateScore(101, 1000);
    game.updateScore(104, 1000);

    game.showTopK(3);


    game.removePlayer(101);

    cout << "\n===== AFTER REMOVAL =====\n";

    game.showTopK(3);



    game.showRank(101);
    game.showRank(104);
    game.showRank(105);

    return 0;
}