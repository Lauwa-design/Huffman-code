#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum size for the characters
#define MAX_SIZE 256

// A node of the Huffman Tree
typedef struct HuffmanNode {
    char data;
    int freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

// MinHeap structure
typedef struct MinHeap {
    int size;
    int capacity;
    HuffmanNode **array;
} MinHeap;

// Function to create a new node
HuffmanNode* createNode(char data, int freq) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// Function to create a MinHeap
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
minHeap->array = (HuffmanNode **)malloc(minHeap->capacity * sizeof(HuffmanNode *));
    return minHeap;
}

// Swap two nodes
void swapNodes(HuffmanNode **a, HuffmanNode **b) {
    HuffmanNode *temp = *a;
    *a = *b;
    *b = temp;
}

// MinHeapify function
void minHeapify(MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest!= idx) {
swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Extract the minimum node
HuffmanNode* extractMin(MinHeap* minHeap) {
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert node in a MinHeap
void insertMinHeap(MinHeap* minHeap, HuffmanNode* node) {
    minHeap->size++;
    int i = minHeap->size - 1;

    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
minHeap->array[i] = node;
}
// Function to build a MinHeap
void buildMinHeap(MinHeap *minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n-1)/2; i >= 0; i--)
        minHeapify(minHeap, i);
}
// Check if size of heap is one
int isSizeOne(MinHeap *minHeap) {
    return minHeap->size == 1;
}

// Create a priority queue with characters and their frequencies.
MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);
    for(int i = 0; i < size; ++i)
        minHeap->array[i] = createNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// Build the Huffman Tree
HuffmanNode* buildHuffmanTree(char data[], int freq[], int size) {
HuffmanNode* left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = createNode(',', left->freq + right->freq);
        top->left = left;
top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

// Print the codes
void printCodes(HuffmanNode *root, int arr[], int top, int *compressedSize) {
    if (root->left) {
        arr[top] = 0;
printCodes(root->left, arr, top + 1, compressedSize);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1, compressedSize);
    }

    if (!root->left &&!root->right) {
        printf("%c: ", root->data);
for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf(" ");
        *compressedSize += top * root->freq;
    }
}

// Huffman encoding
void huffmanCodes(char data[], int freq[], int size) {
HuffmanNode* root = buildHuffmanTree(data, freq, size);

    int arr[MAX_SIZE], top = 0;
    int compressedSize = 0;

    printf("Huffman Codes:");
    printCodes(root, arr, top, &compressedSize);
    printf("\nCompressed size (in bits): %d", compressedSize);
}

int main() {
    char text[1024];
    printf("Enter text to compress: ");
    fgets(text, sizeof(text), stdin);
text[strcspn(text, "")] = '\0'; // Remove newline

    int freq[MAX_SIZE] = {0};
    for (int i = 0; text[i]!= '\0'; ++i)
        freq[(unsigned char)text[i]]++;

    char data[MAX_SIZE];
    int freqData[MAX_SIZE], index = 0;

    for (int i = 0; i < MAX_SIZE; ++i) {
if(freq[i] > 0) {
            data[index] = (char)i;
            freqData[index] = freq[i];
            index++;
        }
    }

int originalSize = strlen(text) * 8; // Size in bits
    printf("Original size (in bits): \n %d", originalSize,"\n");

    huffmanCodes(data, freqData, index);

    return 0;
}
