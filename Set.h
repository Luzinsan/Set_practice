#pragma once
#include <iomanip>
// Сортировка Quicksort
template <class T>
 T* const quickSort( T *const numbers, int left, int right)
{
    // Инициализируем разрешающий элемент, левую границу, правую границу
    T pivot = numbers[left], l_hold = left, r_hold = right;

    while (left < right) // пока границы не сомкнутся
    {
        while ((numbers[right] >= pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[left] = numbers[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[right] = numbers[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу вправо
        }
    }
    numbers[left] = pivot; // ставим разрешающий элемент на место
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
        quickSort(numbers, left, pivot - 1);
    if (right > pivot)
        quickSort(numbers, pivot + 1, right);
    return numbers;
}

template <class T>
class Set
{
private:
    int m_length;
    T* m_item;

public:
    
    Set(int length = 0, T* const item = nullptr) : m_length{ length }
    {
        if (m_length < 0) throw std::invalid_argument("Размер множества не может быть отрицательным");
        if (item)
        {
            m_item = (T*)malloc(m_length * sizeof(T));
            for (unsigned i = 0; i < (unsigned)m_length; i++)
                m_item[i] = item[i];

            quickSort(m_item, 0, m_length - 1); // быстрая сортировка
            unsigned new_lenght = 1; // фактическая длина множества
           
            for (unsigned i = 1; i < (unsigned)m_length; i++)
                // если в упорядоченном массиве текущий элемент отличается от предыдущего 
                // - то добавляем этот элемент в множество, и увеличиваем длину множества +1
                if (m_item[i] != m_item[i - 1]) m_item[new_lenght++] = m_item[i]; 
            
            if (m_length != new_lenght) // если уникальных элементов меньше, чем всего элементов было в массиве
            { // то обрезаем массив
                m_length = new_lenght;
                if (!(m_item = (T*)realloc(m_item, m_length * sizeof(T)))) throw std::bad_alloc();
            }
        }
        else if (m_length) 
             if(!(m_item = (T*)malloc(m_length * sizeof(T)))) throw std::bad_alloc();
            
    }

    Set(const Set& fromSet)
        : Set(fromSet.m_length, fromSet.m_item)  { }

    
    ~Set() 
    { 
        if(m_item) 
            delete[] m_item; 
    }

    static bool isProperSubset(const Set& set, const Set& withSet)
    {
        return set.m_length && !(set == withSet);  
    }
    static bool isNONProperSubset(const Set& set, const Set& withSet)
    {
        return !isProperSubset(set, withSet);
    }
    const Set& operator=(const Set& fromSet) 
    {
        if (this == &fromSet)
            return *this;

        if (m_length != fromSet.m_length)
        {
            m_length = fromSet.m_length;
            delete[] m_item;
            m_item = (T*)malloc(m_length * sizeof(T));
        }
        for (unsigned i = 0; i < (unsigned)m_length; i++)
            m_item[i] = fromSet.m_item[i];
        return *this;
    }
    Set& operator=(Set&& fromSet) noexcept
    {
        if (this == &fromSet)
            return *this;

        std::swap(m_length, fromSet.m_length);
        std::swap(m_item, fromSet.m_item);
        delete[] fromSet.m_item;
        fromSet.m_item = nullptr;
        return *this;
    }
    T& operator[](int index)
    {
        if (index >= m_length) throw std::invalid_argument("Значение индекса выходит за пределы размера множества");
        return m_item[index];
    }
    const T& operator[](int index) const
    {
        if (index >= m_length) throw std::invalid_argument("Значение индекса выходит за пределы размера множества");
        return m_item[index];
    }
    int getLength() { return m_length; };
    bool isEmpty() { return !m_length; }
    Set(Set&& fromSet) noexcept : m_length{ fromSet.m_length }
    {
        std::swap(m_item, fromSet.m_item);
        fromSet.m_item = nullptr;
    }
    friend bool operator==(const Set& set1, const Set& set2)
    {
        if (set1.m_length != set2.m_length) return false;
        for (unsigned i = 0; i < (unsigned)set1.m_length; i++)
            if (set1.m_item[i] != set2.m_item[i]) return false;
        return true;
    }
    friend bool operator!=(const Set& set1, const Set& set2)
    {
        return !(set1 == set2);
    }

    // конъюнкция
    const Set& operator&=(const Set& set)
    {
        int temp_size = 0;
        T* temp_set = (T*)malloc(set.m_length * sizeof(T));
        int left_border = 0;
        bool flag;
        for (unsigned i = 0; i < (unsigned)m_length; i++)
        {
            flag = true;
            for (unsigned j = left_border; set.m_item[j] <= m_item[i]; j++)
                if (m_item[i] == set.m_item[j])
                {
                    temp_set[temp_size++] = set.m_item[j];
                    left_border = j + 1;
                    flag = false;
                }
            if (m_item[i] >= set.m_item[left_border] && flag) left_border++;
        }
        m_length = temp_size;
        std::swap(m_item, temp_set);
        return *this;
    }
    friend Set operator&&(const Set& set1, const Set& set2)
    {
        return Set(set1) &= set2;
    }
    const Set& operator*=(const Set& set)
    {
        *this &= set;
        return *this;
    }
    friend Set operator*(const Set& set1, const Set& set2) 
    {
        return Set(set1) &= set2;
    }
    

    // дизъюнкция
    const Set& operator|=(const Set& set) 
    {
        Set temp_set;
        unsigned left_border = 0; // левая граница второго множества
        
        bool flag;
        for (unsigned i = 0; i < (unsigned)m_length; i++)
        {
            temp_set.m_length++;
            temp_set.m_item = (T*)realloc(temp_set.m_item, temp_set.m_length * sizeof(T));
            temp_set.m_item[temp_set.m_length - 1] = m_item[i]; // заносим текущий элемент из первого множества
            flag = true;
            // и проходимся по элементам второго множества от левой границы занесённых элементов,
            // пока не дойдём до текущего элемента первого множества
            for (unsigned j = left_border;  set.m_item[j] <= m_item[i]; j++) 
            {
                temp_set += set.m_item[j]; // append в нужное место
                left_border = j+1;  
                flag = false;
            }
            if (m_item[i] >= set.m_item[left_border] && flag) left_border++;
        }
        for (unsigned j = left_border; j < (unsigned)set.m_length; j++)
        {// заполняем оставшиеся элементы из второго множества
            temp_set.m_length++;
            temp_set.m_item = (T*)realloc(temp_set.m_item, temp_set.m_length * sizeof(T));
            temp_set.m_item[temp_set.m_length - 1] = set.m_item[j]; 
        }

        m_item = temp_set.m_item;
        m_length = temp_set.m_length;
        temp_set.m_item = nullptr;
        return *this;
    }
    friend Set operator||(const Set& set1,  const Set& set2)
    {
        return Set(set1)|=set2;
    }
    const Set& operator+=(const Set& set)
    {
        return  *this |= set;
    }
    friend Set operator+(const Set& set1,  const Set& set2)
    {
        return Set(set1) |= set2;
    }
    
    
    // Добавление элемента в множество
    const Set& operator|=(const int& element) 
    {
        if (!m_length) 
        {
            m_length = 1;
            m_item = (T*)realloc(m_item, sizeof(T));
            return *this;
        }

        for (unsigned i = 0; i < (unsigned)m_length; i++)
        {
            int shift_element = 0;
            if ( element == m_item[i] ) return *this;
            if ( element < m_item[i])
            {
                shift_element = m_item[i];
                m_item[i] = element;
                for (unsigned j = i + 1; j < (unsigned)m_length-1; j++)
                {
                    m_item[j] = shift_element;
                    shift_element = m_item[j + 1];
                }
                m_length++;
                m_item = (T*)realloc(m_item, m_length * sizeof(T));
                m_item[m_length - 1] = shift_element;
                return *this;
            }
        }
        m_length++;
        m_item = (T*)realloc(m_item, m_length * sizeof(T));
        m_item[m_length - 1] = element;
        return *this;
    }
    friend Set operator||(const Set& set, const int& element)
    {
        return Set(set) |= element;
    }
    const Set& operator+=(const T& element)
    {
        *this |= element;
        return *this;
    }
    friend Set operator+(const Set& set, const T& element)
    {
        return Set(set) |= element;
    }

    friend std::ostream& operator<<(std::ostream& out, const Set& set)
    {
        std::streamsize size = std::cout.width();
        if (!size) size = 10;
        
        for (unsigned i = 0; i < (unsigned)set.m_length; i++)
            out << std::fixed << std::setprecision(2) << std::setw(size) << set.m_item[i];
        out << "\n";
        return out;
    }
    friend std::istream& operator>>(std::istream& in, Set& set) 
    {
        T element;
        for (unsigned i = 0; i < (unsigned)set.m_length; i++)
        {
            in >> element;
            set |= element;
        }
        return in;
    }
};

