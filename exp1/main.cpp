#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <string>
#include <iomanip> 
#include <random>     

class Complex {
public:
    double real;  // 实部
    double imag;  // 虚部

    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // 计算复数的模
    double modulus() const {
        return std::sqrt(real * real + imag * imag);
    }

    // 重载相等运算符，用于查找
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    // 用于打印复数
    void print() const {
        std::cout << real << " + " << imag << "i";
    }
};

// 用于比较两个复数的模，若模相同则比较实部
bool compareByModulus(const Complex& a, const Complex& b) {
    double modA = a.modulus();
    double modB = b.modulus();
    if (modA == modB) {
        return a.real < b.real;
    }
    return modA < modB;
}

std::vector<Complex> generateRandomComplexVector(int n) {
    std::vector<Complex> vec;
    srand((unsigned)time(0));  // 设置随机数种子
    for (int i = 0; i < n; ++i) {
        double real = (rand() % 200 - 100) / 10.0;  // 随机生成 -10 到 10 的实数
        double imag = (rand() % 200 - 100) / 10.0;  // 随机生成 -10 到 10 的虚数
        vec.emplace_back(real, imag);
    }
    return vec;
}

// 置乱复数向量
void shuffleVector(std::vector<Complex>& vec) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(vec.begin(), vec.end(), rng);
}

// 测试向量操作
void testVectorOperations(std::vector<Complex>& vec) {
    // 使用 shuffleVector 置乱向量
    shuffleVector(vec);

    // 查找复数 (假设查找 1.0 + 2.0i)
    Complex target(1.0, 2.0);
    auto it = std::find(vec.begin(), vec.end(), target);
    if (it != vec.end()) {
        std::cout << "Found ";
        it->print();
        std::cout << " in the vector.\n";
    } else {
        std::cout << "Complex number not found.\n";
    }

    // 插入复数
    vec.emplace_back(3.0, 4.0);
    std::cout << "Inserted 3.0 + 4.0i into the vector.\n";

    // 删除复数 (删除第一个元素)
    if (!vec.empty()) {
        vec.erase(vec.begin());
        std::cout << "Deleted the first element of the vector.\n";
    }

    // 唯一化
    std::sort(vec.begin(), vec.end(), compareByModulus);
    vec.erase(std::unique(vec.begin(), vec.end(), [](const Complex& a, const Complex& b) {
        return a == b;
    }), vec.end());
    std::cout << "Vector has been uniquified.\n";
}

void bubbleSort(std::vector<Complex>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = 0; j < vec.size() - i - 1; ++j) {
            if (compareByModulus(vec[j + 1], vec[j])) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

void mergeSort(std::vector<Complex>& vec, int lo, int hi) {
    if (hi - lo < 2) return;  // 单元素区间
    int mi = (lo + hi) / 2;
    mergeSort(vec, lo, mi);
    mergeSort(vec, mi, hi);
    std::inplace_merge(vec.begin() + lo, vec.begin() + mi, vec.begin() + hi, compareByModulus);
}

void testSortingEfficiency(const std::vector<Complex>& vec, const std::string& description) {
    std::vector<Complex> vec1 = vec, vec2 = vec;

    // 冒泡排序计时
    clock_t start = clock();
    bubbleSort(vec1);
    clock_t end = clock();
    std::cout << description << " - Bubble Sort time: " << std::fixed << std::setprecision(8)
              << (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";

    // 归并排序计时
    start = clock();
    mergeSort(vec2, 0, vec2.size());
    end = clock();
    std::cout << description << " - Merge Sort time: " << std::fixed << std::setprecision(8)
              << (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
}

std::vector<Complex> findInRange(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& c : vec) {
        double mod = c.modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    return result;
}

int main() {
    // 增加向量的规模到 10,000
    std::vector<Complex> vec = generateRandomComplexVector(10000);

    // 顺序向量
    std::vector<Complex> sortedVec = vec;
    std::sort(sortedVec.begin(), sortedVec.end(), compareByModulus);

    // 逆序向量
    std::vector<Complex> reverseVec = sortedVec;
    std::reverse(reverseVec.begin(), reverseVec.end());

    // 乱序向量
    std::vector<Complex> shuffledVec = vec;
    shuffleVector(shuffledVec);

    // 测试各种操作
    std::cout << "Testing vector operations:\n";
    testVectorOperations(vec);

    // 测试排序效率 - 顺序向量
    std::cout << "\nTesting sorting efficiency for sorted vector:\n";
    testSortingEfficiency(sortedVec, "Sorted Vector");

    // 测试排序效率 - 乱序向量
    std::cout << "\nTesting sorting efficiency for shuffled vector:\n";
    testSortingEfficiency(shuffledVec, "Shuffled Vector");

    // 测试排序效率 - 逆序向量
    std::cout << "\nTesting sorting efficiency for reversed vector:\n";
    testSortingEfficiency(reverseVec, "Reversed Vector");

    // 区间查找
    double m1 = 1.0, m2 = 5.0;
    std::vector<Complex> result = findInRange(sortedVec, m1, m2);
    std::cout << "\nComplex numbers with modulus in range [" << m1 << ", " << m2 << "):\n";
    for (const auto& c : result) {
        c.print();
        std::cout << "\n";
    }

    return 0;
}
