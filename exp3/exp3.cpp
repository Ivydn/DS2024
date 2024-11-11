#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
using namespace std;

// λͼ�� Bitmap������ڣ�3����Ҫ��
class Bitmap {
private:
    unsigned char* M;
    int N, _sz;

protected:
    void init(int n) {
        N = n;
        M = new unsigned char[(N + 7) / 8];  // ÿ8��bit��һ���ֽ�
        memset(M, 0, (N + 7) / 8);  // ��ʼ��Ϊ0
        _sz = N;
    }

public:
    Bitmap(int n = 8) {
        init(n);
    } // ��ʼ��λͼ��С
    ~Bitmap() {
        delete[] M;
    } // �ͷ�λͼ�ռ�

    void set(int k) { // ���õ� k λΪ 1
        if (k >= N) {
            expand(k);
        }
        M[k >> 3] |= (0x80 >> (k & 0x07));  // ��Ӧ��bitλ��Ϊ1
    }

    void clear(int k) { // ����� k λΪ 0
        if (k >= N) {
            expand(k);
        }
        M[k >> 3] &= ~(0x80 >> (k & 0x07));  // ��Ӧ��bitλ����
    }

    bool test(int k) { // ���Ե� k λ�Ƿ�Ϊ 1
        if (k >= N) {
            expand(k);
        }
        return M[k >> 3] & (0x80 >> (k & 0x07));  // ���bitλ�Ƿ�Ϊ1
    }

    void dump() { // ���λͼ�Ķ����ƴ�
        for (int i = 0; i < _sz; ++i) {
            cout << (test(i) ? '1' : '0');
        }
        cout << endl;
    }

    void expand(int k) { // ��չλͼ��С
        if (k < N) return;  // ���kС�ڵ�ǰ��С������Ҫ��չ
        int oldN = N;
        unsigned char* oldM = M;
        init(2 * k);  // ��չ��2����С
        memcpy(M, oldM, (oldN + 7) / 8);  // ���ƾ�����
        delete[] oldM;
    }
};

// ���������Ľڵ�ṹ��
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left, *right;

    HuffmanNode(char character, int frequency)
        : ch(character), freq(frequency), left(NULL), right(NULL) {}
};

// ���������ıȽϽṹ���������ȶ���
struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->freq > right->freq;
    }
};

// ���������� BinTree������ڣ�1����Ҫ��
template <typename T>
class BinTree {
public:
    T* root;

    BinTree() : root(NULL) {}
    virtual ~BinTree() {}
};

// �������� HuffTree������ڣ�2����Ҫ��
class HuffTree : public BinTree<HuffmanNode> {
public:
    HuffTree(const map<char, int>& freqMap) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

        // ����Ҷ�ڵ㣬���������ȶ���
        map<char, int>::const_iterator it;
        for (it = freqMap.begin(); it != freqMap.end(); ++it) {
            pq.push(new HuffmanNode(it->first, it->second));
        }

        // ������������
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            HuffmanNode* node = new HuffmanNode('\0', left->freq + right->freq);
            node->left = left;
            node->right = right;
            pq.push(node);
        }

        root = pq.top();
    }

    // �����������ͷŹ��������ڵ���ڴ�
    ~HuffTree() {
        deleteTree(root);
    }

private:
    void deleteTree(HuffmanNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};

// �������������� HuffCode������ڣ�3����Ҫ��
class HuffCode {
private:
    map<char, string> huffTable;

    // �ݹ����ɹ����������
    void buildTable(HuffmanNode* node, const string& str) {
        if (!node) return;
        if (node->ch != '\0') { // ���ڲ��ڵ㣬�����ַ��Ĺ���������
            huffTable[node->ch] = str;
        }
        buildTable(node->left, str + "0");
        buildTable(node->right, str + "1");
    }

public:
    HuffCode(HuffTree& tree) {
        buildTable(tree.root, "");
    }

    // ����һ���ַ���
    string encode(const string& word) {
        string encodedStr;
        for (size_t i = 0; i < word.size(); ++i) {
            encodedStr += huffTable[word[i]];
        }
        return encodedStr;
    }

    // ��������������
    void printTable() {
        map<char, string>::iterator it;
        for (it = huffTable.begin(); it != huffTable.end(); ++it) {
            cout << it->first << ": " << it->second << endl;
        }
    }
};

// ��ȡ�ļ���ͳ����ĸƵ��
string readFileContent(const string& fileName) {
    ifstream file(fileName.c_str());
    string content;
    if (file.is_open()) {
        char c;
        while (file.get(c)) {  // ʹ�ô�ͳ�������ַ���ȡ
            content += c;
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }
    return content;
}

// ��������ʵ�ֹ����������㷨���Ե��ʽ��б��룬����ڣ�4����Ҫ��
int main() {
    string fileName = "D:\\I have a dream.txt";  // �����ļ���Ϊ I_have_a_dream.txt
    string inputText = readFileContent(fileName);  // ��ȡ�ļ�����

    if (inputText.empty()) {
        cerr << "The file is empty or could not be read." << endl;
        return -1; // �����ȡ�ļ�ʧ�ܻ��ļ�Ϊ�յ����
    }

    map<char, int> freqMap;

    // ͳ����ĸƵ�ʣ�ֻ������ĸ�ַ��������ִ�Сд
    for (size_t i = 0; i < inputText.size(); ++i) {
        char c = inputText[i];
        if (isalpha(c)) {
            freqMap[tolower(c)]++;
        }
    }

    // ������������
    HuffTree huffTree(freqMap);

    // ��������������
    cout << "Huffman Code Table:" << endl;
    HuffCode huffCode(huffTree);
    huffCode.printTable(); // ��������������

    // �Ե��� "dream" ���б���
    string word = "dream";
    string encodedWord = huffCode.encode(word);
    cout << "Encoded '" << word << "': " << encodedWord << endl;

    // ʹ�� Bitmap �洢������
    Bitmap bitmap(encodedWord.size());
    for (size_t i = 0; i < encodedWord.size(); ++i) {
        if (encodedWord[i] == '1') {
            bitmap.set(i);
        }
    }

    // ��� Bitmap λͼ�Ķ����ƴ���ʾ
    cout << "Bitmap representation of '" << word << "': ";
    bitmap.dump();

    // ���������� "freedom" ���б���
    word = "freedom";
    encodedWord = huffCode.encode(word);
    cout << "Encoded '" << word << "': " << encodedWord << endl;

    // ʹ�� Bitmap �洢 "freedom" ������
    bitmap = Bitmap(encodedWord.size());
    for (size_t i = 0; i < encodedWord.size(); ++i) {
        if (encodedWord[i] == '1') {
            bitmap.set(i);
        }
    }

    // ��� Bitmap λͼ�Ķ����ƴ���ʾ
    cout << "Bitmap representation of '" << word << "': ";
    bitmap.dump();

    return 0;
}

