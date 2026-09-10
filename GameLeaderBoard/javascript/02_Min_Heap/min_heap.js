class MinHeap {
    constructor(compare) {
        this.heap = [];
        this.compare = compare;
    }

    push(value) {
        this.heap.push(value);

        let index = this.heap.length - 1;

        while (index > 0) {
            let parentIndex = Math.floor((index - 1) / 2);

            if (this.compare(this.heap[index], this.heap[parentIndex]) >= 0) {
                break;
            }

            [this.heap[index], this.heap[parentIndex]] =
            [this.heap[parentIndex], this.heap[index]];

            index = parentIndex;
        }
    }

    peek() {
        if (this.heap.length === 0) {
            return null;
        }

        return this.heap[0];
    }

    pop() {
        if (this.heap.length === 0) {
            return null;
        }

        const removed = this.heap[0];

        this.heap[0] = this.heap[this.heap.length - 1];
        this.heap.pop();

        let index = 0;

        while (true) {
            const leftChild = 2 * index + 1;
            const rightChild = 2 * index + 2;

            let smallest = index;

            if (
                leftChild < this.heap.length &&
                this.compare(
                    this.heap[leftChild],
                    this.heap[smallest]
                ) < 0
            ) {
                smallest = leftChild;
            }

            if (
                rightChild < this.heap.length &&
                this.compare(
                    this.heap[rightChild],
                    this.heap[smallest]
                ) < 0
            ) {
                smallest = rightChild;
            }

            if (smallest === index) {
                break;
            }

            [this.heap[index], this.heap[smallest]] =
            [this.heap[smallest], this.heap[index]];

            index = smallest;
        }

        return removed;
    }

    size() {
        return this.heap.length;
    }
}

export default MinHeap;