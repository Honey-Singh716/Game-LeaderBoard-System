// in c++ unordered map is implemented using hash table. In javascript, we can use object to implement hash map.

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
}


const game = new GameSystem();

game.addPlayer(101,"ShadowX",1900);
game.addPlayer(102,"DragonSlayer",2000);

console.log(game.players);

const readline = require('readline');

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