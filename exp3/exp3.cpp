#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
using namespace std;

// 位图类 Bitmap，满足第（3）个要求
class Bitmap {
private:
    unsigned char* M;
    int N, _sz;

protected:
    void init(int n) {
        N = n;
        M = new unsigned char[(N + 7) / 8];  // 每8个bit存一个字节
        memset(M, 0, (N + 7) / 8);  // 初始化为0
        _sz = N;
    }

public:
    Bitmap(int n = 8) {
        init(n);
    } // 初始化位图大小
    ~Bitmap() {
        delete[] M;
    } // 释放位图空间

    void set(int k) { // 设置第 k 位为 1
        if (k >= N) {
            expand(k);
        }
        M[k >> 3] |= (0x80 >> (k & 0x07));  // 对应的bit位设为1
    }

    void clear(int k) { // 清除第 k 位为 0
        if (k >= N) {
            expand(k);
        }
        M[k >> 3] &= ~(0x80 >> (k & 0x07));  // 对应的bit位清零
    }

    bool test(int k) { // 测试第 k 位是否为 1
        if (k >= N) {
            expand(k);
        }
        return M[k >> 3] & (0x80 >> (k & 0x07));  // 检查bit位是否为1
    }

    void dump() { // 输出位图的二进制串
        for (int i = 0; i < _sz; ++i) {
            cout << (test(i) ? '1' : '0');
        }
        cout << endl;
    }

    void expand(int k) { // 扩展位图大小
        if (k < N) return;  // 如果k小于当前大小，不需要扩展
        int oldN = N;
        unsigned char* oldM = M;
        init(2 * k);  // 扩展到2倍大小
        memcpy(M, oldM, (oldN + 7) / 8);  // 复制旧数据
        delete[] oldM;
    }
};

// 哈夫曼树的节点结构体
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left, *right;

    HuffmanNode(char character, int frequency)
        : ch(character), freq(frequency), left(NULL), right(NULL) {}
};

// 哈夫曼树的比较结构，用于优先队列
struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->freq > right->freq;
    }
};

// 二叉树基类 BinTree，满足第（1）个要求
template <typename T>
class BinTree {
public:
    T* root;

    BinTree() : root(NULL) {}
    virtual ~BinTree() {}
};

// 哈夫曼树 HuffTree，满足第（2）个要求
class HuffTree : public BinTree<HuffmanNode> {
public:
    HuffTree(const map<char, int>& freqMap) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

        // 创建叶节点，并插入优先队列
        map<char, int>::const_iterator it;
        for (it = freqMap.begin(); it != freqMap.end(); ++it) {
            pq.push(new HuffmanNode(it->first, it->second));
        }

        // 构建哈夫曼树
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

    // 析构函数，释放哈夫曼树节点的内存
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

// 哈夫曼编码类型 HuffCode，满足第（3）个要求
class HuffCode {
private:
    map<char, string> huffTable;

    // 递归生成哈夫曼编码表
    void buildTable(HuffmanNode* node, const string& str) {
        if (!node) return;
        if (node->ch != '\0') { // 非内部节点，保存字符的哈夫曼编码
            huffTable[node->ch] = str;
        }
        buildTable(node->left, str + "0");
        buildTable(node->right, str + "1");
    }

public:
    HuffCode(HuffTree& tree) {
        buildTable(tree.root, "");
    }

    // 编码一个字符串
    string encode(const string& word) {
        string encodedStr;
        for (size_t i = 0; i < word.size(); ++i) {
            encodedStr += huffTable[word[i]];
        }
        return encodedStr;
    }

    // 输出哈夫曼编码表
    void printTable() {
        map<char, string>::iterator it;
        for (it = huffTable.begin(); it != huffTable.end(); ++it) {
            cout << it->first << ": " << it->second << endl;
        }
    }
};

// 读取文件并统计字母频率
string readFileContent(const string& fileName) {
    ifstream file(fileName.c_str());
    string content;
    if (file.is_open()) {
        char c;
        while (file.get(c)) {  // 使用传统方法逐字符读取
            content += c;
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }
    return content;
}

// 主函数：实现哈夫曼编码算法并对单词进行编码，满足第（4）个要求
int main() {
    string fileName = "D:\\I have a dream.txt";  // 假设文件名为 I_have_a_dream.txt
    string inputText = readFileContent(fileName);  // 读取文件内容

    if (inputText.empty()) {
        cerr << "The file is empty or could not be read." << endl;
        return -1; // 处理读取文件失败或文件为空的情况
    }

    map<char, int> freqMap;

    // 统计字母频率，只考虑字母字符，不区分大小写
    for (size_t i = 0; i < inputText.size(); ++i) {
        char c = inputText[i];
        if (isalpha(c)) {
            freqMap[tolower(c)]++;
        }
    }

    // 构建哈夫曼树
    HuffTree huffTree(freqMap);

    // 输出哈夫曼编码表
    cout << "Huffman Code Table:" << endl;
    HuffCode huffCode(huffTree);
    huffCode.printTable(); // 输出哈夫曼编码表

    // 对单词 "dream" 进行编码
    string word = "dream";
    string encodedWord = huffCode.encode(word);
    cout << "Encoded '" << word << "': " << encodedWord << endl;

    // 使用 Bitmap 存储编码结果
    Bitmap bitmap(encodedWord.size());
    for (size_t i = 0; i < encodedWord.size(); ++i) {
        if (encodedWord[i] == '1') {
            bitmap.set(i);
        }
    }

    // 输出 Bitmap 位图的二进制串表示
    cout << "Bitmap representation of '" << word << "': ";
    bitmap.dump();

    // 对其他单词 "freedom" 进行编码
    word = "freedom";
    encodedWord = huffCode.encode(word);
    cout << "Encoded '" << word << "': " << encodedWord << endl;

    // 使用 Bitmap 存储 "freedom" 编码结果
    bitmap = Bitmap(encodedWord.size());
    for (size_t i = 0; i < encodedWord.size(); ++i) {
        if (encodedWord[i] == '1') {
            bitmap.set(i);
        }
    }

    // 输出 Bitmap 位图的二进制串表示
    cout << "Bitmap representation of '" << word << "': ";
    bitmap.dump();

    return 0;
}

