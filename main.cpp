#include <iostream>
#include <memory>
#include <string>
template<class T>
class Iterator {
public:
    T *const_it = nullptr;
    T *def_it = nullptr;

    void adr(T *arr) {
        def_it = arr;
    }

    Iterator() = default;

    //explicit Iterator(const T* tmp): const_it(tmp){}
    Iterator(const Iterator<T> &tmp) : def_it(tmp.def_it) {}

    explicit Iterator(T *a) : def_it(a) {}
//    Iterator<T> operator = (const Iterator<T>& tmp){
//        return Iterator(tmp);
//    }


//    bool operator != (const Iterator<T>& tmp){
//        if(this->def_it==tmp.def_it){
//            return 0;
//        }
//        return 1;
//    }
    bool operator==(const Iterator<T> &tmp) {
        return def_it == tmp.def_it;
    }

    Iterator<T> operator++() {
        if (const_it != nullptr) {
            return Iterator<T>(const_it);
        }
        return Iterator<T>(def_it++);
    };

    T &operator*() {
        if (const_it != nullptr) {
            re();
        }
        return *def_it;
    }

    int operator-(T *tmp) {
        return this->def_it - tmp;
    }

    int operator-(Iterator<T> tmp) {
        return this->def_it - tmp.def_it;
    }

    Iterator<T> operator-(const int &tmp) {
        this->def_it -= tmp;
        return *this;
    }

    int operator+(T *tmp) {
        return this->def_it + tmp;
    }

    Iterator<T> operator+(const int &tmp) {
        this->def_it += tmp;
        return *this;
    }

    T re() {
        return *const_it;
    }

};

template<class T>
class const_iterator : public Iterator<T> {
public:
    T *const_it = nullptr;

    const_iterator() = default;

    explicit const_iterator(T *arr) : const_it(arr) {}

    explicit const_iterator(const Iterator<T> &tmp) : const_it(tmp.def_it) {}

    const T &operator*() {
        return *const_it;
    }
};

template<class T>
class reverse_it : public const_iterator<T> {
public:
    T *rev_it = nullptr;

    reverse_it() = default;

    explicit reverse_it(const Iterator<T> &tmp) : rev_it(tmp.def_it) {}

    explicit reverse_it(T *a) : rev_it(a) {}

    bool operator==(const reverse_it<T> &tmp) {
        return rev_it == tmp.rev_it;
    }

    reverse_it<T> operator++() {
        return reverse_it<T>(rev_it--);
    };

    T &operator*() {
        return *rev_it;
    }

    int operator-(T *tmp) {
        return this->rev_it + tmp;
    }

    int operator-(reverse_it<T> tmp) {
        return this->rev_it + tmp.rev_it;
    }

    Iterator<T> operator-(const int &tmp) {
        this->rev_it += tmp;
        return *this;
    }

    int operator+(T *tmp) {
        return this->rev_it - tmp;
    }

    Iterator<T> operator+(const int &tmp) {
        this->def_it -= tmp;
        return *this;
    }


};

template<class T>
class vector : public const_iterator<T> {
private:
    size_t cap = 1;
    size_t sz = 0;

public:
    T *arr;

    vector() {
        cap = 1;
        sz = 0;
        arr = new T[1];
        Iterator<T>::adr(arr);
        //Iterator<T> def_it = arr;
    }

    vector(const vector<T> &tmp) : cap(tmp.cap), sz(tmp.sz),
                                   arr(reinterpret_cast<T *>(new int8_t[sizeof(T) * tmp.cap])) {
        std::uninitialized_copy(tmp.arr, tmp.arr + sz, arr);
    }

    explicit vector(size_t n) : cap(n), sz(n), arr(new T[n]) {
        Iterator<T>::adr(arr);
    }

    vector(std::initializer_list<T> t) : cap(t.size()), sz(t.size()),
                                         arr(reinterpret_cast<T *>(new int8_t[sizeof(T) * t.size()])) {
        ///edit
        //std::uninitialized_copy(t.begin(),t.end(),arr);
        for (int i = 0; i < t.size(); i++) {
            new(arr + i) T(*(t.begin() + i));
        }
        //std::copy(t.begin(), t.end(), arr);
        //std::copy(t.begin(),t.end(),arr);
        this->adr(arr);
    }

    vector(vector<T> &&t) : cap(t.cap), sz(t.sz), arr(reinterpret_cast<T *>(new int8_t[sizeof(T) * t.cap])) {
        for (int i = 0; i < sz; ++i) {
            new(arr + i) T(t[i]);
        }
        //.~vector();
    }

    ~vector() {
        for (int i = 0; i < sz; ++i) {
            arr[i].~T();
        }
        sz = 0;
        cap = 0;
        arr = nullptr;
    }

    T &operator[](size_t index) {
        return this->at(index);
    }

    void resize(size_t new_size) {
        if (new_size < sz) {
            for (int i = sz - 1; i >= new_size - 1; --i) {
                arr[i].~T();
            }
            sz = new_size;
            return;
        }
        if (new_size == sz) {
            return;
        }
        reserve(new_size);
        for (int i = sz; i < new_size; i++) {
            //arr[i] = T();
            new(arr + i) T();
        }
        sz = new_size;
    }

    T &at(size_t index) {
        return arr[index];
    }

    T *data() {
        return arr;
    }

    int size() {
        return sz;
    }

    int capacity() {
        return cap;
    }

    Iterator<T> begin() {
        return Iterator<T>(arr);
    }

    Iterator<T> end() {
        return Iterator<T>(arr + sz);
    }

    const_iterator<T> cbegin() {
//        const T* tmp = const_cast<const T *>(Iterator<T>::def_it);
//        return Iterator<T>(tmp);
        return const_iterator<T>(arr);
    }

    const_iterator<T> cend() {
        return const_iterator<T>(arr + size());
    }

    reverse_it<T> rend() {
        return reverse_it<T>(arr);
    }

    reverse_it<T> rbegin() {
        return reverse_it<T>(arr + size());
    }

    T &front() {
        return arr[0];
    }

    T &back() {
        return arr[sz - 1];
    }

    void reserve(size_t arg) {
//        if(arg<0 ){
//            return;
//        }
        if (arg == cap) {
            return;
        }
        if (arg < cap) {
            cap = arg;
        }
        //int8_t * temp = new int8_t [sizeof(T)*arg];
        T *temp2 = reinterpret_cast<T *>(new int8_t[sizeof(T) * arg]);
        //delete[] temp;
        //edit
        std::uninitialized_copy(arr, arr + size(), temp2);
        arr = temp2;
        //delete[] temp2;
        cap = arg;
        Iterator<T>::adr(arr);
    };

    void push_back(const T &arg) {
//        if(sz==cap){
//            reserve(cap*2);
//            std::cout<<"\nreserved, cap = "<<cap<<"\n";
//        }
        //arr[sz] = arg;
        //resize(sz+1);
        emplace_back(arg);
        //new(arr+sz) T(arg);
        // sz++;
    }

    void clear() {
        resize(0);
    }

    void push_front(const T &arg) {
        //int8_t * temp = new int8_t [sizeof(T)*(cap+1)];
        /*   T * temp2 = reinterpret_cast<T*>(new int8_t [sizeof(T)*(cap+1)]);
           std::uninitialized_move(arr,arr+size(),temp2+1);
           temp2[0] = arg;
           arr = temp2;
           sz++;
           cap++;
           Iterator<T>::adr(arr);*/
        emplace(0, arg);
    }

    void pop_back() {
        if (sz == 0) {
            return;
        }
        arr[sz - 1].~T();
        sz--;
    }

    void swap(vector<T> &tmp) {
        std::swap(arr, tmp.arr);
        std::swap(cap, tmp.cap);
        std::swap(sz, tmp.sz);
    }

    void erase(size_t l, size_t r) {
        for (size_t i = l; i < r; i++) {
            arr[i].~T();
        }
        std::uninitialized_copy(arr + r, arr + size(), arr + l);
        sz -= (r - l);
    }

    void erase(size_t index) {
        this->erase(index, index + 1);
    }

    bool empty() {
        return this->size() == 0;
    }

    size_t max_size() {
        return cap;
    }

    void assign(size_t n, const T &arg) {
        resize(0);
        resize(n);
        for (int i = 0; i < n; ++i) {
            new(arr + i)  T(arg);
        }
    }

    void assign(Iterator<T> first, Iterator<T> last) {
        size_t n = last - first;
        resize(0);
        resize(n);
        for (int i = 0; i < n; ++i) {
            new(arr + i)  T(*(first.def_it + i));
        }
    }

    template<typename... ARGS>
    void emplace_back(ARGS &&... args) {
//        if(sz+1 == cap){
//            reserve(cap*2);
//        }
//        arr[sz] = T(std::forward<ARGS>(args)...);
//        sz++;
        this->emplace(sz, args...);
    }

    template<typename... ARGS>
    void emplace(Iterator<T> pos, ARGS &&... args) {
        int p = pos - this->begin();
        if (sz == cap) {
            //std::cout<<"reserved for emplace\n";
            reserve(cap * 2);
        }

        //std::cout<<"continue "<<arr[0]<<" "<<p<<"\n";

//        for(int i = size()-1;i>=p;--i){
//            arr[i+1] = arr[i];
//        }
        std::uninitialized_copy(arr + p, arr + sz, arr + p + 1);
        new(arr + p) T(args...);
        sz++;
    }

    template<typename... ARGS>
    void emplace(size_t pos, ARGS &&... args) {
        this->emplace(this->begin() + pos, args...);
    }

    void insert(size_t pos, const T &tmp) {
        this->emplace(pos, tmp);
    }

    void insert(size_t pos, size_t n, const T &tmp) {
        for (int i = 0; i < n; i++) {
            this->emplace(pos + i, tmp);
        }
    }

    void insert(size_t pos, Iterator<T> first, Iterator<T> last) {
        int n = last - first;
        for (int i = 0; i < n; ++i) {
            this->emplace(pos + i, *(first + i));
        }

    }

};

template<class A, class B>
struct pair {
    A f;
    B s;

    pair() = default;

    pair(const A &a, const B &b) : f(a), s(b) {};

    pair(const pair &t) : f(t.f), s(t.s) {};

    pair(A &&a, B &&b) : f(a), s(b) {};

    pair(pair &&tmp) noexcept: f(tmp.f), s(tmp.s) {};

};

int main() {
//    vector<tmp> c;
//    vector<tmp> b;
//    b.resize(5);
//    c.resize(10);
//    for(int i =0;i<c.size();i++){
//        std::cout<<c[i].h<<"x"<<c[i].w<<" ";
//    }
//    c.emplace_back(190,90);
//    tmp bebra(228,1337);
//    c.insert(3,bebra);
//    c.insert(3,12,bebra);
//    c.insert(1,b.begin(),b.end());
//    c.push_back(bebra);
//    std::cout<<"\n";
//    for(int i =0;i<c.size();i++){
//        std::cout<<c[i].h<<"-"<<c[i].w<<" ";
//    }
//    std::cout<<"\nfinish\n";
    //vector<vector<vector<pair<pair<std::string, std::string>, pair<std::string, std::string>>>>> a;
    //a.resize(10);
    //a.push_back({{{{"1", "1"}, {"1", "1"}}}});
//    vector<int> c = {1, 2, 3, 4, 5, 6};
//    std::cout << c.size() << " " << c.capacity() << "\n";
//    c.push_front(3);
//    c.erase(1, 3);
//    for (int i = 0; i < c.size(); ++i) {
//        std::cout << c[i] << " ";
//    }
//    std::cout << "\n" << c.size() << " " << c.capacity() << "\n";
//    return 0;
//vector< pair<vector<int>, vector<pair<int,double>> >> c;
}