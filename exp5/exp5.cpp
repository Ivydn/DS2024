#include <iostream>
#include <vector>
#include <algorithm> 
#include <chrono>

using namespace std;

// 1. �������� (Bubble Sort)
template <typename T>
void bubbleSort(T& arr) {
    int n = arr.size();
    bool sorted = false; 

    while (!sorted) {
        sorted = true; 
        for (int i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) { 
                swap(arr[i - 1], arr[i]); 
                sorted = false;
            }
        }
        n--; 
    }
}

// 2. �������� (Insertion Sort)
template <typename T>
void insertionSort(T& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 3. ѡ������ (Selection Sort)
template <typename T>
void selectionSort(T& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

// 4. �鲢���� (Merge Sort)
template <typename T>
void merge(T& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge(arr, left, mid);
    merge(arr, mid+1, right);
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    for (int i = 0; i < temp.size(); i++) {
        arr[left + i] = temp[i];
    }
}

template <typename T>
void mergeSort(T& arr) {
    merge(arr, 0, arr.size() - 1);
}

// 5. �������� (Quick Sort)
template <typename T>
int partition(T& arr, int low, int high) {
    auto pivot = arr[high]; 
    int i = low - 1;        
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]); 
        }
    }
    swap(arr[i + 1], arr[high]); 
    return i + 1;                
}

template <typename T>
void quickSort(T& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); 
        quickSort(arr, low, pi - 1);       
        quickSort(arr, pi + 1, high);       
    }
}

// 6. ������ (Heap Sort)
template <typename T>
void heapify(T& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template <typename T>
void heapSort(T& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

template <typename T>
void printArray(const vector<T>& arr) {
    for (const T& element : arr) { // ʹ��ģ������ T ����Ԫ��
        cout << element << " ";
    }
    cout << endl;
}

// ���Ժ���
template <typename Func, typename T>
void testSort(Func sortFunc, T arr, const string& sortName, const string& type) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << sortName << " ��" << type << "�����ϵ�ִ��ʱ��: " << duration.count() << " ms" << endl;
}

// �������ݵĺ���
template <typename T>
vector<T> generateSequence(int n, const string& type) {
    vector<T> arr(n);
    if (type == "˳��") {
        for (int i = 0; i < n; i++) arr[i] = static_cast<T>(i + 1);
    } else if (type == "����") {
        for (int i = 0; i < n; i++) arr[i] = static_cast<T>(n - i);
    } else {
        for (int i = 0; i < n; i++) arr[i] = static_cast<T>(rand() % 10000);
    }
    return arr;
}

int main() {
    const int n = 10000;
    vector<string> types = {"˳��", "����", "���"};

    // ʹ�ó�ʼ���б�ֱ�ӳ�ʼ�������㷨
    vector<pair<string, void(*)(vector<int>&)>> sortAlgorithms = {
        {"��������", bubbleSort<vector<int>>},
        {"��������", insertionSort<vector<int>>},
        {"ѡ������", selectionSort<vector<int>>},
        {"�鲢����", mergeSort<vector<int>>},
        {"��������", [](vector<int>& arr) { quickSort(arr, 0, arr.size() - 1); }},
        {"������", heapSort<vector<int>>}
    };

    // ���Դ���
    for (const string& type : types) {
        cout << "���� " << type << " ���ݣ�" << endl;
        for (size_t i = 0; i < sortAlgorithms.size(); i++) {
            string name = sortAlgorithms[i].first;
            auto sortFunc = sortAlgorithms[i].second;
            vector<int> arr = generateSequence<int>(n, type);
            testSort(sortFunc, arr, name, type);
        }
        cout << endl;
    }

    return 0;
}

