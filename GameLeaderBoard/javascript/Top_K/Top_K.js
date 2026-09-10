function getTopK(k) {
    const heap = new MinHeap();

    for(const player of this.players.values()){
        heap.insert(player);

        if(heap.size() > k){
            heap.pop();
        }
    }

    const topKplayer = [];

    while(heap.size() > 0){
        topKplayer.push(heap.pop());
    }

    return topKplayer.reverse();
}

export default getTopK;

