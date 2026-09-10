// in c++ unordered map is implemented using hash table. In javascript, we can use object to implement hash map.

import MinHeap from "../02_Min_Heap/min_heap.js";

class GameSystem{
        constructor(){
                this.players = new Map();
        }

        addPlayer(id,name,rating){
                if(this.players.has(id)){
                        console.log("Player already exists");
                }

                const player = {
                        id: id,
                        name: name,
                        rating: rating,
                        score: 0,
                        wins: 0,
                        losses: 0
                };

                this.players.set(id, player);
        }


        findPlayer(id){
                if(!this.players.has(id)){
                        return null;
                }
                return this.players.get(id);
        }

        showPlayer(id){
                const player = this.findPlayer(id);

                if(player == null){
                        console.log("Player does not exist");
                        return;
                }

                console.log("Player id: ", player.id);
                console.log("Player name: ", player.name);
                console.log("Player rating: ", player.rating);
                console.log("Player score: ", player.score);
                console.log("Player wins: ", player.wins);
                console.log("Player losses: ", player.losses);
        }

        updateScore(id,score){
                const player = this.findPlayer(id);
                if(player){
                        player.score += score;
                        return;
                }

                else{
                        console.log("Player does not exist");
                }
        }

        removePlayer(id){
                const player = this.findPlayer(id);
                if(!player){
                        console.log("Player does not exist");
                        return;
                }

                this.players.delete(id);
                console.log("Player remove successfully");
        }


        getTopK(k) {

                const heap = new MinHeap((a, b) => a.rating - b.rating);

                for (const player of this.players.values()) {

                        heap.push(player);

                        if (heap.size() > k) {
                        heap.pop();
                        }
                }

                const topKPlayers = [];

                while (heap.size() > 0) {
                        topKPlayers.push(heap.pop());
                }

                return topKPlayers.reverse();
                }

}


const game = new GameSystem();

game.addPlayer(101,"ShadowX",1900);
game.addPlayer(102,"DragonSlayer",2000);

console.log(game.players);

import readline from "readline";

const r1 = readline.createInterface({
        input: process.stdin,
        output: process.stdout
});

game.showPlayer(101);

r1.question("Enter player id to check if player exists: ", function(id){
        id = Number(id);

        const player = game.findPlayer(id);
        if(player){
                console.log("Player exists");
                console.log(player);
        } else {
                console.log("Player does not exist");
        }
        r1.close();
});


game.updateScore(101, 500);

console.log(game.getTopK(2));