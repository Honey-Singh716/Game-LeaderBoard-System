class MinHeap{
    constructor(){
        this.heap = [];
    }
    
    push(value){
        this.heap.push(value);

        let index = this.heap.length - 1;

        while(index > 0){
            let parentIndex =- Math.floor((index - 1)/2);

            if(this.heap[index] >= this.heap[parentIndex]){
                break;
            }

            [this.heap[index],this.heap[parentIndex]] = [this.heap[parentIndex], this.heap[index]];
            index = parentIndex; //bubbleup
        }
    }


    peek(){ 
        if(this.heap.length === 0){
            return null;
        }
        return this.heap[0];
    }

    pop(){
        if(this.heap.length === 0){
            return null;
        }

        this.heap[0] = this.heap[this.heap.length - 1];
        this.heap.pop();

        let index = 0;

        while(true){
            let leftChild = 2*index + 1;
            let rightChild = 2*index + 2;

            let smallest = index;

            if(leftChild < this.heap.length && this.heap[leftChild] < this.heap[[smallest]]){
                smallest = leftChild;
            }

            if(rightChild < this.heap.length && this.heap[rightChild] < this.heap[smallest]){
                smallest = rightChild;
            }

            if(smallest === index){
                break;
            }

            [this.heap[index], this.heap[smallest]] = [this.heap[smallest], this.heap[index]];
            index = smallest; //bubble down
        }

    }
}


const minHeap = new MinHeap();

minHeap.push(5);
minHeap.push(3);
minHeap.push(8);

console.log(minHeap.peek());
minHeap.pop();

console.log(minHeap.peek());