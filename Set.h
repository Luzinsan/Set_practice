#pragma once

// Сортировка Quicksort
//template <class T>
const int* quickSort(int* numbers, int left, int right)
{
    // Инициализируем разрешающий элемент, левую границу, правую границу
    int pivot = numbers[left], l_hold = left, r_hold = right;

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

//template <class T>
class Set
{
private:
    int m_length;
    int* m_item;

public:
    
    Set(int length = 0, int* item = nullptr) : m_length{ length }, m_item{ item }
    {
        if (m_length < 0) throw std::invalid_argument("Размер множества не может быть отрицательным");
        if (m_item)
        {
            quickSort(m_item, 0, m_length - 1);
            int new_lenght = 1;
            int* new_item = new int[m_length];
            new_item[0] = m_item[0];
            for (unsigned i = 1; i < (unsigned)m_length; i++)
                if (m_item[i] != m_item[i - 1]) new_item[new_lenght++] = m_item[i];
            m_length = new_lenght;
            
            delete[] m_item;
            m_item = new_item;
        }
        else if (m_length) m_item = (int*)malloc(m_length * sizeof(int));

        for (int i = 0; i < m_length; i++)
            std::cout << m_item[i] << "\t";
    }

    
    Set(const Set& fromSet)
        : m_length{ fromSet.m_length },
          m_item{ fromSet.m_item } { }
    Set(Set&& fromSet) 
    {
        std::swap(m_length, fromSet.m_length);
        std::swap(m_item, fromSet.m_item);
    }
    ~Set() { delete[] m_item; }
    static bool isProperSubset(const Set& set, const Set& withSet)
    {
        return set.m_length && !(set == withSet);  
    }
    static bool isNONProperSubset(const Set& set, const Set& withSet)
    {
        return !isProperSubset(set, withSet);
    }
    Set& operator=(const Set& fromSet) 
    {
        if (this == &fromSet)
            return *this;

        delete[] m_item;
        m_length = fromSet.m_length;
        m_item = (int*)malloc( m_length * sizeof(int));
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
        return *this;
    }
    int& operator[](int index)
    {
        if (index >= m_length) throw std::invalid_argument("Значение индекса выходит за пределы размера множества");
        return m_item[index];
    }
    const int& operator[](int index) const
    {
        if (index >= m_length) throw std::invalid_argument("Значение индекса выходит за пределы размера множества");
        return m_item[index];
    }
    int getLength() { return m_length; };
    bool isEmpty() { return !m_length; }

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
    
    
    friend Set operator&&(const Set& set1, const Set& set2)
    {
        return Set(set1) &= set2;
    }
   
    
    
    const Set& append(int element) 
    {
        for (unsigned i = 0; i < (unsigned)m_length-1; i++)
        {
            int shift_element = 0;
            if (m_item[i] = element) return *this;
            else if (m_item[i] > element)
            {
                shift_element = m_item[i];
                m_item[i] = element;
                for (unsigned j = i + 1; j < (unsigned)m_length-2; j++)
                {
                    m_item[j] = shift_element;
                    shift_element = m_item[j + 1];
                }

                m_item = (int*)realloc(m_item, (m_length + 1) * sizeof(int));
                m_item[m_length - 1] = shift_element;
                return *this;
            }
        }
    }
    const Set& operator&=(const Set& set) // конъюнкция
    {
        int temp_size = 0;
        int* temp_set = (int*)malloc( set.m_length * sizeof(int));
        int left_border = 0;
        for (unsigned i = 0; i < (unsigned)set.m_length; i++)
            for (unsigned j = left_border; m_item[j] <= set.m_item[i]; j++)
                if (m_item[j] == set.m_item[i]) 
                { 
                    temp_set[temp_size++] = set.m_item[i];
                    left_border = j;
                }
        m_length = temp_size;
        std::swap(m_item, temp_set);
        return set;
    }
    friend Set operator||(const Set& set1, const Set& set2)
    {
        return Set(set1)|=set2;
    }
    const Set& operator|=(const Set& set) //дизъюнкция
    {
        unsigned temp_size = 0;
        int* temp_set = (int*)malloc( set.m_length * sizeof(int));
        unsigned left_border = 0;
        for (unsigned i = 0; i < (unsigned)set.m_length; i++)
        {
            temp_set[temp_size] = set.m_item[i];
            for (unsigned j = left_border; m_item[j] < set.m_item[i]; j++)
            {
                temp_set[temp_size++] = set.m_item[i];
                left_border = j;
            }
        }
        
        m_length = temp_size;
        std::swap(m_item, temp_set);
        return set;
    }
};

