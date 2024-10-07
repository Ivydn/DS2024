#include <algorithm>  // 为了使用 std::swap, std::make_heap, std::pop_heap
#include <cstdlib>    // 为了使用 rand()

typedef int Rank;  // 定义Rank类型

#define DEFAULT_CAPACITY 3  // 默认的初始容量

template <typename T>
class Vector {
protected:
    Rank _size;       // 规模
    int _capacity;    // 容量
    T* _elem;         // 数据区

    // 从数组区间 A[lo, hi) 复制内容到当前向量
    void copyFrom(T const* A, Rank lo, Rank hi) {
        _elem = new T[_capacity = 2 * (hi - lo)];  // 为新向量申请空间
        _size = 0;
        while (lo < hi) {
            _elem[_size++] = A[lo++];  // 复制元素
        }
    }

    // 当空间不足时扩展容量
    void expand() {
        if (_size < _capacity) return;  // 空间尚足，无需扩容
        _capacity = std::max(_capacity, DEFAULT_CAPACITY);  // 确保最小容量为 DEFAULT_CAPACITY
        T* oldElem = _elem;
        _elem = new T[_capacity <<= 1];  // 容量加倍
        for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];  // 复制原向量内容
        delete[] oldElem;  // 释放原空间
    }

    // 当装填因子过小时收缩容量
    void shrink() {
        if (_capacity < DEFAULT_CAPACITY << 1) return;  // 不致收缩到 DEFAULT_CAPACITY 以下
        if (_size << 2 > _capacity) return;  // 装填因子高于 25% 时不收缩
        T* oldElem = _elem;
        _elem = new T[_capacity >>= 1];  // 容量减半
        for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];  // 复制原向量内容
        delete[] oldElem;  // 释放原空间
    }

    // 冒泡排序中的一趟冒泡操作
    bool bubble(Rank lo, Rank hi) {
        bool sorted = true;  // 标志位：判断是否已完全有序
        while (++lo < hi)
            if (_elem[lo - 1] > _elem[lo]) {  // 若逆序，则交换
                std::swap(_elem[lo - 1], _elem[lo]);
                sorted = false;  // 若存在逆序则无法确定有序
            }
        return sorted;
    }

    // 冒泡排序算法
    void bubbleSort(Rank lo, Rank hi) {
        while (!bubble(lo, hi--));  // 逐趟扫描直到完全有序
    }

    // 选取区间中最大元素的秩
    Rank max(Rank lo, Rank hi) {
        Rank max = lo;
        while (++lo < hi)
            if (_elem[max] < _elem[lo]) max = lo;  // 记录更大元素的位置
        return max;
    }

    // 选择排序算法
    void selectionSort(Rank lo, Rank hi) {
        while (lo < --hi)
            std::swap(_elem[max(lo, hi)], _elem[hi]);  // 将最大元素交换至末尾
    }

    // 归并算法
    void merge(Rank lo, Rank mi, Rank hi) {
        T* A = _elem + lo;  // 合并后的数组
        int lb = mi - lo;
        T* B = new T[lb];  // 辅助数组 B
        for (Rank i = 0; i < lb; B[i] = A[i++]);  // 复制前半部分
        int lc = hi - mi;
        T* C = _elem + mi;  // 后半部分的数组
        for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
            if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
            if ((k < lc) && (!(j < lb) || (C[k] < B[j]))) A[i++] = C[k++];
        }
        delete[] B;  // 释放辅助数组
    }

    // 归并排序算法
    void mergeSort(Rank lo, Rank hi) {
        if (hi - lo < 2) return;  // 单元素区间自然有序
        Rank mi = (lo + hi) / 2;
        mergeSort(lo, mi);  // 对前半部分排序
        mergeSort(mi, hi);  // 对后半部分排序
        merge(lo, mi, hi);  // 归并
    }

    // 枢点构造算法（用于快速排序）
    Rank partition(Rank lo, Rank hi) {
        std::swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);  // 随机选择一个轴点并与首元素交换
        T pivot = _elem[lo];  // 以第一个元素为轴点
        while (lo < hi) {
            while ((lo < hi) && (pivot <= _elem[--hi]));
            _elem[lo] = _elem[hi];
            while ((lo < hi) && (_elem[++lo] <= pivot));
            _elem[hi] = _elem[lo];
        }
        _elem[lo] = pivot;  // 轴点归位
        return lo;  // 返回轴点秩
    }

    // 快速排序算法
    void quickSort(Rank lo, Rank hi) {
        if (hi - lo < 2) return;  // 单元素区间自然有序
        Rank mi = partition(lo, hi);  // 构造轴点
        quickSort(lo, mi);  // 对轴点左侧排序
        quickSort(mi + 1, hi);  // 对轴点右侧排序
    }

    // 堆排序算法
    void heapSort(Rank lo, Rank hi) {
        std::make_heap(_elem + lo, _elem + hi);  // 将区间转换为堆
        while (hi - lo > 1) {
            std::pop_heap(_elem + lo, _elem + hi--);  // 将最大元素移动到末尾
        }
    }

public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);  // 初始化数组
    }

    Vector(T const* A, Rank n) { copyFrom(A, 0, n); }

    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }

    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }

    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

    // 析构函数
    ~Vector() { delete[] _elem; }

    // 只读接口
    Rank size() const { return _size; }  // 返回向量的规模

    bool empty() const { return !_size; }  // 判断向量是否为空

    // 判断向量是否已排序
    int disordered() const {
        int n = 0;
        for (Rank i = 1; i < _size; i++)
            if (_elem[i - 1] > _elem[i]) n++;
        return n;
    }

    // 无序向量整体查找
    Rank find(T const& e) const { return find(e, 0, _size); }

    // 无序向量区间查找
    Rank find(T const& e, Rank lo, Rank hi) const {
        while ((lo < hi--) && (_elem[hi] != e));
        return hi;
    }

    // 有序向量整体查找
    Rank search(T const& e) const { return (0 >= _size) ? -1 : search(e, 0, _size); }

    // 有序向量区间查找（二分查找实现）
    Rank search(T const& e, Rank lo, Rank hi) const {
        while (lo < hi) {
            Rank mi = (lo + hi) >> 1;
            if (e < _elem[mi]) hi = mi;
            else lo = mi + 1;
        }
        return --lo;
    }

    // 可写访问接口
    T& operator[](Rank r) const { return _elem[r]; }  // 重载下标操作符，可以类似于数组形式引用元素

    // 重载赋值操作符
    Vector<T>& operator=(Vector<T> const& V) {
        if (_elem) delete[] _elem;  // 释放原有内容
        copyFrom(V._elem, 0, V._size);  // 复制新的内容
        return *this;
    }

    // 删除秩为 r 的元素
    T remove(Rank r) {
        T e = _elem[r];
        remove(r, r + 1);
        return e;
    }

    // 删除区间 [lo, hi) 之内的元素
    int remove(Rank lo, Rank hi) {
        if (lo == hi) return 0;
        while (hi < _size) _elem[lo++] = _elem[hi++];
        _size = lo;
        shrink();  // 缩容
        return hi - lo;
    }

    // 插入元素
    Rank insert(Rank r, T const& e) {
        expand();  // 如有必要，扩容
        for (Rank i = _size; i > r; i--) _elem[i] = _elem[i - 1];
        _elem[r] = e;
        _size++;
        return r;
    }
    Rank insert(T const& e) { return insert(_size, e); }  // 默认作为末元素插入
    
    // 对 [lo, hi) 排序
    void sort(Rank lo, Rank hi) { mergeSort(lo, hi); }

    // 整体排序
    void sort() { sort(0, _size); }

    // 对 [lo, hi) 置乱
    void unsort(Rank lo, Rank hi) {
        for (Rank i = hi - lo; i > 0; i--) std::swap(_elem[lo + i - 1], _elem[lo + rand() % i]);
    }

    // 整体置乱
    void unsort() { unsort(0, _size); }

    // 无序去重
    int deduplicate() {
        int oldSize = _size;
        Rank i = 1;
        while (i < _size)
            (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
        return oldSize - _size;
    }

    // 有序去重
    int uniquify() {
        Rank i = 0, j = 0;
        while (++j < _size)
            if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
        _size = ++i;
        shrink();
        return j - i;
    }

    // 遍历（使用函数指针，只读或局部性修改）
    void traverse(void (*visit)(T&)) {
        for (Rank i = 0; i < _size; i++) visit(_elem[i]);
    }

    // 遍历（使用函数对象，可全局性修改）
    template <typename VST>
    void traverse(VST& visit) {
        for (Rank i = 0; i < _size; i++) visit(_elem[i]);
    }
};
