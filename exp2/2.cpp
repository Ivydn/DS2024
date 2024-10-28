#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <ctime>
#include <cstdlib>

// ����ֱ��ͼ������������ĺ���
int largestArea(const std::vector<int>& heights) {
    std::stack<int> s;  // ���ڴ������������ջ
    int maxArea = 0;    // ��������ʼ��Ϊ 0
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

// ��������߶�����
std::vector<int> generateRandomHeights(int length, int maxHeight) {
    std::vector<int> heights(length);
    for (int i = 0; i < length; i++) {
        heights[i] = rand() % (maxHeight + 1);
    }
    return heights;
}

// ��ӡ�߶�����
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
    // �������������
    srand(static_cast<unsigned>(time(0)));
    // ���� 10 ������������ݲ��������������
    std::cout << "�����������:" << std::endl;
    for (int i = 1; i <= 10; i++) {
        int length = rand() % 100 + 1;         // ��������� 1 �� 100 ֮��
        int maxHeight = 104;                   // ���߶�����Ϊ 104
        std::vector<int> randomHeights = generateRandomHeights(length, maxHeight);
        std::cout << "���� " << i << ": heights = ";
        print(randomHeights);
        std::cout << "���: " << largestArea(randomHeights) << std::endl;
    }
    return 0;
}

