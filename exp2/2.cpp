#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <ctime>
#include <cstdlib>

// 计算直方图中最大矩形面积的函数
int largestArea(const std::vector<int>& heights) {
    std::stack<int> s;  // 用于存放柱子索引的栈
    int maxArea = 0;    // 最大面积初始化为 0
    int n = heights.size();
    for (int i = 0; i <= n; i++) {
        int h = (i == n) ? 0 : heights[i];
        while (!s.empty() && h < heights[s.top()]) {
            int height = heights[s.top()];
            s.pop();
            int width = s.empty() ? i : i - s.top() - 1;
            maxArea = std::max(maxArea, height * width);
        }
        s.push(i);
    }
    return maxArea;
}

// 生成随机高度数组
std::vector<int> generateRandomHeights(int length, int maxHeight) {
    std::vector<int> heights(length);
    for (int i = 0; i < length; i++) {
        heights[i] = rand() % (maxHeight + 1);
    }
    return heights;
}

// 打印高度数组
void print(const std::vector<int>& heights) {
    std::cout << "[";
    for (size_t i = 0; i < heights.size(); i++) {
        std::cout << heights[i];
        if (i != heights.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
int main() {
    // 设置随机数种子
    srand(static_cast<unsigned>(time(0)));
    // 生成 10 组随机测试数据并计算最大矩形面积
    std::cout << "随机测试数据:" << std::endl;
    for (int i = 1; i <= 10; i++) {
        int length = rand() % 100 + 1;         // 随机长度在 1 到 100 之间
        int maxHeight = 104;                   // 最大高度设置为 104
        std::vector<int> randomHeights = generateRandomHeights(length, maxHeight);
        std::cout << "测试 " << i << ": heights = ";
        print(randomHeights);
        std::cout << "输出: " << largestArea(randomHeights) << std::endl;
    }
    return 0;
}

